#include "fruc.h"
#include <iostream>
using namespace std;

int main() {
    try {
        Deque dq;

        dq.push_back(fruc(1, 2));
        dq.push_back(mixedfruc(7, 3));
        dq.push_back(resultfruc(5, 2));

        cout << "\n--- Содержимое дека ---\n";
        dq.show_all();

        cout << "\nУдаляем первый элемент...\n";
        dq.pop_front();
        dq.show_all();

        cout << "\nВставляем элемент по индексу 1...\n";
        dq.insert_at(1, mixedfruc(9, 4));
        dq.show_all();

        cout << "\nУдаляем элемент по индексу 1...\n";
        dq.remove_at(1);
        dq.show_all();

        cout << "\nПоиск по значению 2.5...\n";
        fruc found1 = dq.find_by_value(2.5);
        cout << "Найдено: ";
        found1.show();

        cout << "\nПоиск по значению 3.0...\n";
        fruc found2 = dq.find_by_value(3.0);
        cout << "Найдено: ";
        found2.show();

        cout << "\nВсего объектов fruc: " << fruc::getCount() << endl;

    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }

    return 0;
}