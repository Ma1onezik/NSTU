#include "BTreeFile.h"
#include <stdexcept>
#include <iostream>
#include <queue>
#include <algorithm>

using std::cout;

DataFile::DataFile(const char* filename) {
    file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file) {
        file.clear();
        file.open(filename, std::ios::out | std::ios::binary);
        if (!file) {
            throw std::runtime_error("Cannot create file: " + std::string(filename));
        }
        file.close();
        file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
        if (!file) {
            throw std::runtime_error("Cannot reopen file: " + std::string(filename));
        }
    }
}

DataFile::~DataFile() {
    file.close();
}

long DataFile::insert(const Record& rec) {
    file.seekp(0, std::ios::end);
    long pos = file.tellp();
    file.write(reinterpret_cast<const char*>(&rec), sizeof(rec));
    file.flush();
    return pos;
}

bool DataFile::read(long pos, Record& rec) {
    if (pos < 0) return false;
    file.seekg(pos);
    file.read(reinterpret_cast<char*>(&rec), sizeof(rec));
    return file.good();
}

//--------------------------------------------------------------------

template <int T>
BTreeFile<T>::BTreeFile(const char* filename) : pageReads(0) {
    file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file) {
        file.clear();
        file.open(filename, std::ios::out | std::ios::binary);
        if (!file) {
            throw std::runtime_error("Cannot create file: " + std::string(filename));
        }
        root = sizeof(long);
        file.write(reinterpret_cast<char*>(&root), sizeof(long));
        BTreeNode<T> node;
        file.write(reinterpret_cast<char*>(&node), sizeof(node));
        file.close();
        file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    }
    file.seekg(0);
    file.read(reinterpret_cast<char*>(&root), sizeof(long));
}

template <int T>
BTreeFile<T>::~BTreeFile() {
    file.seekp(0);
    file.write(reinterpret_cast<char*>(&root), sizeof(long));
    file.close();
}

template <int T>
void BTreeFile<T>::readNode(long pos, BTreeNode<T>& node) {
    file.seekg(pos);
    file.read(reinterpret_cast<char*>(&node), sizeof(node));
    pageReads++;
}

template <int T>
void BTreeFile<T>::writeNode(long pos, BTreeNode<T>& node) {
    file.seekp(pos);
    file.write(reinterpret_cast<char*>(&node), sizeof(node));
}

template <int T>
long BTreeFile<T>::allocateNode() {
    file.seekp(0, std::ios::end);
    long pos = file.tellp();
    BTreeNode<T> node;
    writeNode(pos, node);
    return pos;
}

template <int T>
void BTreeFile<T>::splitChild(long parentPos, int i, long childPos) {
    BTreeNode<T> parent, child, newChild;
    readNode(parentPos, parent);
    readNode(childPos, child);

    // Создаем новый узел
    newChild.leaf = child.leaf;
    newChild.count = T - 1;

    for (int j = 0; j < T - 1; j++)
        newChild.keys[j] = child.keys[j + T];

    if (!child.leaf)
        for (int j = 0; j < T; j++)
            newChild.children[j] = child.children[j + T];

    child.count = T - 1;

    long newPos = allocateNode();
    writeNode(newPos, newChild);

    for (int j = parent.count; j >= i + 1; j--)
        parent.children[j + 1] = parent.children[j];

    parent.children[i + 1] = newPos;

    for (int j = parent.count - 1; j >= i; j--)
        parent.keys[j + 1] = parent.keys[j];

    parent.keys[i] = child.keys[T - 1];
    parent.count++;

    writeNode(childPos, child);
    writeNode(parentPos, parent);
}

template <int T>
bool BTreeFile<T>::insertNonFull(long pos, const IndexEntry& entry) {
    BTreeNode<T> node;
    readNode(pos, node);

    int i = node.count - 1;
    bool keyExists = false;

    if (node.leaf) {
        while (i >= 0 && entry.key < node.keys[i].key) {
            i--;
        }
        
        i++; // Переходим к позиции вставки
        
        // Проверяем, существует ли ключ
        for (int j = 0; j < node.count; j++) {
            if (node.keys[j].key == entry.key) {
                // Ключ существует - обновляем позицию
                node.keys[j].pos = entry.pos;
                keyExists = true;
                break;
            }
        }
        
        if (!keyExists) {
            // Сдвигаем ключи для вставки нового
            for (int j = node.count; j > i; j--) {
                node.keys[j] = node.keys[j - 1];
            }
            node.keys[i] = entry;
            node.count++;
        }
        
        writeNode(pos, node);
        return !keyExists; // true = вставлен новый, false = обновлен существующий
    } else {
        // Ищем дочерний узел
        while (i >= 0 && entry.key < node.keys[i].key) {
            i--;
        }
        i++;
        
        // Проверяем, не полон ли дочерний узел
        BTreeNode<T> child;
        readNode(node.children[i], child);
        
        if (child.count == 2 * T - 1) {
            splitChild(pos, i, node.children[i]);
            readNode(pos, node);
            
            // Определяем, в какой дочерний узел идти
            if (entry.key > node.keys[i].key) {
                i++;
                readNode(node.children[i], child);
            }
        }
        
        return insertNonFull(node.children[i], entry);
    }
}

template <int T>
bool BTreeFile<T>::insert(int key, long dataPos, bool* wasUpdated) {
    resetCounter();
    
    IndexEntry entry{key, dataPos};
    BTreeNode<T> rootNode;
    readNode(root, rootNode);

    bool result = false;
    
    // Если корень полон, увеличиваем высоту дерева
    if (rootNode.count == 2 * T - 1) {
        long newRoot = allocateNode();
        BTreeNode<T> newNode;
        newNode.leaf = false;
        newNode.count = 0;
        newNode.children[0] = root;
        writeNode(newRoot, newNode);
        
        splitChild(newRoot, 0, root);
        
        // Обновляем корень
        root = newRoot;
        file.seekp(0);
        file.write(reinterpret_cast<char*>(&root), sizeof(long));
        
        // Вставляем в неполный узел
        result = insertNonFull(newRoot, entry);
    } else {
        result = insertNonFull(root, entry);
    }
    
    if (wasUpdated) {
        *wasUpdated = !result; // Если result=false, значит ключ был обновлен
    }
    
    return true;
}

template <int T>
bool BTreeFile<T>::searchRec(long pos, int key, long& dataPos) {
    BTreeNode<T> node;
    readNode(pos, node);

    int i = 0;
    while (i < node.count && key > node.keys[i].key) {
        i++;
    }

    if (i < node.count && key == node.keys[i].key) {
        dataPos = node.keys[i].pos;
        return true;
    }

    if (node.leaf) {
        return false;
    }
    
    return searchRec(node.children[i], key, dataPos);
}

template <int T>
bool BTreeFile<T>::search(int key, long& dataPos) {
    resetCounter();
    return searchRec(root, key, dataPos);
}


template <int T>
void BTreeFile<T>::removeFromLeaf(long pos, int idx) {
    BTreeNode<T> node;
    readNode(pos, node);
    
    for (int i = idx + 1; i < node.count; ++i) {
        node.keys[i - 1] = node.keys[i];
    }
    node.count--;
    
    writeNode(pos, node);
}

template <int T>
void BTreeFile<T>::removeFromNonLeaf(long pos, int idx) {
    BTreeNode<T> node;
    readNode(pos, node);
    
    IndexEntry k = node.keys[idx];
    
    // Если левый дочерний узел имеет хотя бы t ключей
    BTreeNode<T> leftChild;
    readNode(node.children[idx], leftChild);
    
    if (leftChild.count >= T) {
        IndexEntry pred = getPredecessor(pos, idx);
        node.keys[idx] = pred;
        writeNode(pos, node);
        deleteRec(node.children[idx], pred.key);
    } else {
        // Если правый дочерний узел имеет хотя бы t ключей
        BTreeNode<T> rightChild;
        readNode(node.children[idx + 1], rightChild);
        
        if (rightChild.count >= T) {
            IndexEntry succ = getSuccessor(pos, idx);
            node.keys[idx] = succ;
            writeNode(pos, node);
            deleteRec(node.children[idx + 1], succ.key);
        } else {
            // Оба дочерних узла имеют t-1 ключей, объединяем
            merge(pos, idx);
            deleteRec(node.children[idx], k.key);
        }
    }
}

template <int T>
IndexEntry BTreeFile<T>::getPredecessor(long pos, int idx) {
    BTreeNode<T> node;
    readNode(pos, node);
    
    // Двигаемся к крайнему правому узлу левого поддерева
    long cur = node.children[idx];
    BTreeNode<T> curNode;
    readNode(cur, curNode);
    
    while (!curNode.leaf) {
        cur = curNode.children[curNode.count];
        readNode(cur, curNode);
    }
    
    return curNode.keys[curNode.count - 1];
}

template <int T>
IndexEntry BTreeFile<T>::getSuccessor(long pos, int idx) {
    BTreeNode<T> node;
    readNode(pos, node);
    
    // Двигаемся к крайнему левому узлу правого поддерева
    long cur = node.children[idx + 1];
    BTreeNode<T> curNode;
    readNode(cur, curNode);
    
    while (!curNode.leaf) {
        cur = curNode.children[0];
        readNode(cur, curNode);
    }
    
    return curNode.keys[0];
}

template <int T>
void BTreeFile<T>::merge(long pos, int idx) {
    BTreeNode<T> node, child, sibling;
    readNode(pos, node);
    readNode(node.children[idx], child);
    readNode(node.children[idx + 1], sibling);
    
    // Перемещаем ключ из текущего узла в child
    child.keys[T - 1] = node.keys[idx];
    
    // Копируем ключи из sibling в child
    for (int i = 0; i < sibling.count; ++i) {
        child.keys[i + T] = sibling.keys[i];
    }
    
    // Копируем дочерние указатели из sibling в child
    if (!child.leaf) {
        for (int i = 0; i <= sibling.count; ++i) {
            child.children[i + T] = sibling.children[i];
        }
    }
    
    // Сдвигаем ключи и дочерние указатели в текущем узле
    for (int i = idx + 1; i < node.count; ++i) {
        node.keys[i - 1] = node.keys[i];
    }
    for (int i = idx + 2; i <= node.count; ++i) {
        node.children[i - 1] = node.children[i];
    }
    
    child.count += sibling.count + 1;
    node.count--;
    
    writeNode(node.children[idx], child);
    writeNode(pos, node);
}

template <int T>
bool BTreeFile<T>::deleteRec(long pos, int key) {
    BTreeNode<T> node;
    readNode(pos, node);
    
    int idx = 0;
    while (idx < node.count && node.keys[idx].key < key) {
        ++idx;
    }
    
    if (idx < node.count && node.keys[idx].key == key) {
        if (node.leaf) {
            removeFromLeaf(pos, idx);
        } else {
            removeFromNonLeaf(pos, idx);
        }
        return true;
    } else {
        if (node.leaf) {
            return false; // Ключ не найден
        }
        
        // Проверяем, достаточно ли ключей в дочернем узле
        BTreeNode<T> child;
        readNode(node.children[idx], child);
        
        if (child.count < T) {
            // Нужно заполнить дочерний узел
            fill(pos, idx);
            readNode(pos, node);
            
            // После заполнения может измениться idx
            if (idx > node.count) {
                idx = node.count;
            }
        }
        
        return deleteRec(node.children[idx], key);
    }
}

template <int T>
void BTreeFile<T>::fill(long pos, int idx) {
    BTreeNode<T> node;
    readNode(pos, node);
    
    if (idx != 0) {
        BTreeNode<T> leftChild;
        readNode(node.children[idx - 1], leftChild);
        
        if (leftChild.count >= T) {
            borrowFromPrev(pos, idx);
            return;
        }
    }
    
    if (idx != node.count) {
        BTreeNode<T> rightChild;
        readNode(node.children[idx + 1], rightChild);
        
        if (rightChild.count >= T) {
            borrowFromNext(pos, idx);
            return;
        }
    }
    
    if (idx != node.count) {
        merge(pos, idx);
    } else {
        merge(pos, idx - 1);
    }
}

template <int T>
void BTreeFile<T>::borrowFromPrev(long pos, int idx) {
    BTreeNode<T> node, child, sibling;
    readNode(pos, node);
    readNode(node.children[idx], child);
    readNode(node.children[idx - 1], sibling);
    
    // Сдвигаем все ключи в child вправо
    for (int i = child.count - 1; i >= 0; --i) {
        child.keys[i + 1] = child.keys[i];
    }
    
    if (!child.leaf) {
        for (int i = child.count; i >= 0; --i) {
            child.children[i + 1] = child.children[i];
        }
    }
    
    // Перемещаем ключ из sibling в node
    child.keys[0] = node.keys[idx - 1];
    
    if (!child.leaf) {
        child.children[0] = sibling.children[sibling.count];
    }
    
    node.keys[idx - 1] = sibling.keys[sibling.count - 1];
    
    child.count++;
    sibling.count--;
    
    writeNode(node.children[idx], child);
    writeNode(node.children[idx - 1], sibling);
    writeNode(pos, node);
}

template <int T>
void BTreeFile<T>::borrowFromNext(long pos, int idx) {
    BTreeNode<T> node, child, sibling;
    readNode(pos, node);
    readNode(node.children[idx], child);
    readNode(node.children[idx + 1], sibling);
    
    child.keys[child.count] = node.keys[idx];
    
    if (!child.leaf) {
        child.children[child.count + 1] = sibling.children[0];
    }
    
    node.keys[idx] = sibling.keys[0];
    
    for (int i = 1; i < sibling.count; ++i) {
        sibling.keys[i - 1] = sibling.keys[i];
    }
    
    if (!sibling.leaf) {
        for (int i = 1; i <= sibling.count; ++i) {
            sibling.children[i - 1] = sibling.children[i];
        }
    }
    
    child.count++;
    sibling.count--;
    
    writeNode(node.children[idx], child);
    writeNode(node.children[idx + 1], sibling);
    writeNode(pos, node);
}

template <int T>
bool BTreeFile<T>::remove(int key) {
    resetCounter();
    
    long dummy;
    if (!search(key, dummy)) {
        return false;
    }

    bool result = deleteRec(root, key);
    
    // Если корень стал пустым, уменьшаем высоту дерева
    BTreeNode<T> rootNode;
    readNode(root, rootNode);
    
    if (rootNode.count == 0) {
        if (rootNode.leaf) {
            root = -1;
        } else {
            root = rootNode.children[0];
        }
        file.seekp(0);
        file.write(reinterpret_cast<char*>(&root), sizeof(long));
    }
    
    return result;
}

template <int T>
void BTreeFile<T>::showTree() {
    if (root == -1) {
        cout << "Дерево пустое\n";
        return;
    }
    
    std::queue<std::pair<long, int>> q;
    q.push({root, 0});
    
    int currentLevel = 0;
    cout << "\n=== Структура B-дерева (t=" << T << ") ===\n";
    
    while (!q.empty()) {
        auto p = q.front();
        long pos = p.first;
        int level = p.second;

        q.pop();
        
        if (level != currentLevel) {
            cout << "\n";
            currentLevel = level;
        }
        
        BTreeNode<T> node;
        readNode(pos, node);
        
        cout << "[";
        for (int i = 0; i < node.count; i++) {
            cout << node.keys[i].key;
            if (i < node.count - 1) cout << "|";
        }
        cout << "] ";
        
        if (!node.leaf) {
            for (int i = 0; i <= node.count; i++) {
                if (node.children[i] != -1) {
                    q.push({node.children[i], level + 1});
                }
            }
        }
    }
    cout << "\n";
}

template <int T>
long BTreeFile<T>::getPageReads() const {
    return pageReads;
}

template <int T>
void BTreeFile<T>::resetCounter() {
    pageReads = 0;
}

template <int T>
long BTreeFile<T>::getPageReadsWithoutReset() const {
    return pageReads;
}

// Явное инстанцирование для разных значений t
template class BTreeFile<3>;   // M=5
template class BTreeFile<5>;   // M=9
template class BTreeFile<6>;   // M=11
template class BTreeFile<50>;  // M=99
template class BTreeFile<51>;  // M=101
template class BTreeFile<500>; // M=999
template class BTreeFile<501>; // M=1001

