#include <iostream>
#include <stdexcept>
#include <chrono>
#include <cstring>
#include <limits>
#include <cmath>
#include "BTreeFile.h"

using namespace std;

extern template class BTreeFile<3>;
extern template class BTreeFile<5>;
extern template class BTreeFile<50>;
extern template class BTreeFile<500>;

void runTests() {
    //const int N_values[] = {1000, 10000, 100000, 1000000};
    //const int M_values[] = {10, 100, 1000};
    const int N_values[] = {100000, 1000000}; 
    const int M_values[] = {100};

    for (int N : N_values) {
        for (int M : M_values) {
            int t = (M + 1) / 2;

            cout << "\n------------------------------------------------------------\n";
            cout << "ТЕСТ B-ДЕРЕВА: N=" << N << ", M=" << M << ", t=" << t << "\n";

            remove("BTree.bin");
            remove("list.bin");

            auto run = [&](auto& index) {
                DataFile data("list.bin");

                int inserted_count = 0;
                int updated_count = 0;
                auto t1 = chrono::high_resolution_clock::now();
                
                cout << "Вставка " << N << " записей...\n";
                for (int i = 0; i < N; ++i) {
                    Record r;
                    r.key = i;
                    strcpy(r.data, "data");
                    long pos = data.insert(r);
                    bool wasUpdated = false;
                    index.insert(i, pos, &wasUpdated);
                    if (wasUpdated) {
                        updated_count++;
                    } else {
                        inserted_count++;
                    }
                    
                    //прогресс для больших N
                    if (N > 10000 && i % 10000 == 0) {
                        cout << "  Прогресс: " << i << "/" << N << "\n";
                    }
                }
                auto t2 = chrono::high_resolution_clock::now();

                cout << "Поиск " << N << " записей...\n";
                //до удаления
                int found_before = 0;
                long total_reads_before = 0;
                auto t3 = chrono::high_resolution_clock::now();
                for (int i = 0; i < N; ++i) {
                    long pos;
                    if (index.search(i, pos)) {
                        found_before++;
                        total_reads_before += index.getPageReads();
                    }
                    
                    //прогресс для больших N
                    if (N > 10000 && i % 10000 == 0) {
                        cout << "  Прогресс поиска: " << i << "/" << N << "\n";
                    }
                }
                auto t4 = chrono::high_resolution_clock::now();

                cout << "Удаление " << N/2 << " записей...\n";
                // Удаление
                int removed = 0;
                auto t5 = chrono::high_resolution_clock::now();
                for (int i = 0; i < N; i += 2) {
                    if (index.remove(i))
                        removed++;
                        
                    //прогресс для больших N
                    if (N > 10000 && i % 20000 == 0) {
                        cout << "  Прогресс удаления: " << i/2 << "/" << N/2 << "\n";
                    }
                }
                auto t6 = chrono::high_resolution_clock::now();

                cout << "Поиск после удаления...\n";
                int found_after = 0;
                long total_reads_after = 0;
                for (int i = 1; i < N; i += 2) {
                    long pos;
                    if (index.search(i, pos)) {
                        found_after++;
                        total_reads_after += index.getPageReads();
                    }
                }

                cout << "\nРЕЗУЛЬТАТЫ:\n";
                cout << "Вставлено записей:        " << inserted_count << "\n";
                cout << "Обновлено записей:        " << updated_count << "\n";
                cout << "Найдено до удаления:      " << found_before << " / " << N << "\n";
                cout << "Удалено ключей:           " << removed << " / " << (N / 2) << "\n";
                cout << "Найдено после удаления:   " << found_after << " / " << (N / 2) << "\n";

                if (found_before > 0) {
                    cout << "Среднее чтений страниц на поиск (до удаления): " 
                         << (double)total_reads_before / found_before << "\n";
                }
                if (found_after > 0) {
                    cout << "Среднее чтений страниц на поиск (после удаления): " 
                         << (double)total_reads_after / found_after << "\n";
                }

                cout << "Время вставки:            "
                     << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count()
                     << " мс\n";

                cout << "Время поиска:             "
                     << chrono::duration_cast<chrono::milliseconds>(t4 - t3).count()
                     << " мс\n";

                cout << "Время удаления:           "
                     << chrono::duration_cast<chrono::milliseconds>(t6 - t5).count()
                     << " мс\n";

                double expected = 2.0 + log((double)N / M) / log((double)t);
                cout << "Теоретическое число обращений: ~" << expected << "\n";
            };

            try {
                if (t == 5) {  
                    cout << "Создание B-дерева с t=5 (M=10)...\n";
                    BTreeFile<5> index("BTree.bin");
                    run(index);
                }
                else if (t == 50) {  
                    cout << "Создание B-дерева с t=50 (M=100)...\n";
                    BTreeFile<50> index("BTree.bin");
                    run(index);
                }
                else if (t == 500) { 
                    cout << "Создание B-дерева с t=500 (M=1000)...\n";
                    BTreeFile<500> index("BTree.bin");
                    run(index);
                }
                else {
                    cout << "Пропуск: t=" << t << " не поддерживается\n";
                }
            } catch (const std::exception& e) {
                cout << "Ошибка при тестировании: " << e.what() << "\n";
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc == 2 && std::string(argv[1]) == "test") {
        cout << "Запуск тестов...\n";
        runTests();
        cout << "Тесты завершены.\n";
        return 0;
    }
    
    try {
        // Используем M=5 (t=3) по умолчанию для наглядности
        BTreeFile<3> index("BTree.bin");
        DataFile data("list.bin");

        int cmd;
        while (true) {
            try {
                cout << "\n=== B-дерево файла (закрепленные записи) ===\n";
                cout << "1. Insert/Update (вставка/обновление)\n";
                cout << "2. Search (поиск)\n";
                cout << "3. Delete (удаление)\n";
                cout << "4. Show Tree (показать структуру)\n";
                cout << "5. Run Tests (запустить тесты)\n";
                cout << "6. Exit (выход)\n> ";
                cin >> cmd;
                
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    throw runtime_error("Неверный ввод. Пожалуйста, введите число.");
                }

                if (cmd == 1) {
                    Record r;
                    cout << "Key (ключ - целое число): ";
                    cin >> r.key;
                    if (cin.fail()) throw runtime_error("Неверный ключ.");

                    cout << "Data (данные, до 63 символов): ";
                    cin.ignore();
                    cin.getline(r.data, 64);

                    auto start = chrono::high_resolution_clock::now();
                    
                    long pos = data.insert(r);
                    
                    bool inserted = index.insert(r.key, pos);
                    
                    auto end = chrono::high_resolution_clock::now();

                    if (inserted) {
                        cout << "Запись вставлена.\n";
                    } else {
                        cout << "Запись обновлена (ключ уже существовал).\n";
                    }
                    cout << "Прочитано страниц: " << index.getPageReads() << "\n";
                    cout << "Позиция в файле данных: " << pos << "\n";
                    cout << "Время операции: "
                         << chrono::duration_cast<chrono::microseconds>(end - start).count()
                         << " микросек\n";
                }
                else if (cmd == 2) {
                    int key;
                    cout << "Key to search (ключ для поиска): ";
                    cin >> key;
                    if (cin.fail()) throw runtime_error("Неверный ключ.");

                    auto start = chrono::high_resolution_clock::now();
                    long pos;
                    bool found = index.search(key, pos);
                    auto end = chrono::high_resolution_clock::now();

                    if (found) {
                        Record r;
                        if (!data.read(pos, r)) {
                            cout << "Ошибка чтения данных!\n";
                        } else {
                            cout << "Найдено: key=" << r.key << ", data=" << r.data 
                                 << ", pos=" << pos << "\n";
                        }
                    } else {
                        cout << "Не найдено\n";
                    }
                    cout << "Прочитано страниц: " << index.getPageReads() << "\n";
                    cout << "Время поиска: "
                         << chrono::duration_cast<chrono::microseconds>(end - start).count()
                         << " микросек\n";
                }
                else if (cmd == 3) {
                    int key;
                    cout << "Key to delete (ключ для удаления): ";
                    cin >> key;
                    if (cin.fail()) throw runtime_error("Неверный ключ.");

                    auto start = chrono::high_resolution_clock::now();
                    bool removed = index.remove(key);
                    auto end = chrono::high_resolution_clock::now();

                    if (removed) {
                        cout << "Индекс удален из B-дерева.\n";
                        cout << "Примечание: Данные остаются в файле (закрепленные записи).\n";
                    }
                    else {
                        cout << "Ключ не найден\n";
                    }
                    cout << "Прочитано страниц: " << index.getPageReads() << "\n";
                    cout << "Время удаления: "
                         << chrono::duration_cast<chrono::microseconds>(end - start).count()
                         << " микросек\n";
                }
                else if (cmd == 4) {
                    index.showTree();
                }
                else if (cmd == 5) {
                    runTests();
                }
                else if (cmd == 6) {
                    cout << "Выход...\n";
                    break;
                }
                else {
                    cout << "Неизвестная команда. Попробуйте снова.\n";
                }
            }
            catch (const exception& e) {
                cout << "Ошибка: " << e.what() << "\n";
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }
    catch (const exception& e) {
        cout << "Критическая ошибка: " << e.what() << "\n";
        return 1;
    }

    return 0;
}