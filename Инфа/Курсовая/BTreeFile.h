#ifndef BTREENODE_H
#define BTREENODE_H

#include "Record.h"
#include <fstream>
#include <iostream>
#include <vector>  

class DataFile {
private:
    std::fstream file;

public:
    DataFile(const char* filename);
    ~DataFile();

    long insert(const Record& rec);
    bool read(long pos, Record& rec);
};

template <int T>
struct BTreeNode {
    bool leaf;
    int count;
    IndexEntry keys[2 * T - 1];  // M = 2T - 1
    long children[2 * T];

    BTreeNode() : leaf(true), count(0) {
        for (int i = 0; i < 2 * T; i++)
            children[i] = -1;
    }
};

template <int T>
class BTreeFile {
private:
    std::fstream file;
    long root;
    long pageReads;

    void readNode(long pos, BTreeNode<T>& node);
    void writeNode(long pos, BTreeNode<T>& node);
    long allocateNode();

    void splitChild(long parentPos, int index, long childPos);
    bool insertNonFull(long pos, const IndexEntry& entry);  
    
    bool searchRec(long pos, int key, long& dataPos);
    bool deleteRec(long pos, int key);
    
    // Вспомогательные методы для удаления
    void removeFromLeaf(long pos, int idx);
    void removeFromNonLeaf(long pos, int idx);
    IndexEntry getPredecessor(long pos, int idx);
    IndexEntry getSuccessor(long pos, int idx);
    void fill(long pos, int idx);
    void borrowFromPrev(long pos, int idx);
    void borrowFromNext(long pos, int idx);
    void merge(long pos, int idx);

public:
    BTreeFile(const char* filename);
    ~BTreeFile();


    bool insert(int key, long dataPos, bool* wasUpdated = nullptr);  
    
    bool search(int key, long& dataPos);
    bool remove(int key);
    void showTree();
    long getPageReads() const;
    long getPageReadsWithoutReset() const;  
    void resetCounter();
};

#endif