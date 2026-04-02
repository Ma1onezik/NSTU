#include "fruc.h"
#include <iostream>
using namespace std;

int main() {
    try {

        cout << "Тестирование Deque с fruc" << endl;
        Deque<fruc> dq;

        dq.push_back(fruc(1, 2));
        dq.push_back(mixedfruc(7, 3));
        dq.push_back(resultfruc(5, 2));

        cout << "\nСодержимое дека \n\n";
        dq.show_all();

        cout << "\nУдаляем первый элемент...\n\n";
        dq.pop_front();
        dq.show_all();

        cout << "\nВставляем элемент по индексу 1...\n\n";
        dq.insert_at(1, mixedfruc(9, 4));
        dq.show_all();

        cout << "\nУдаляем элемент по индексу 1...\n\n";
        dq.remove_at(1);
        dq.show_all();

        cout << "\nПоиск по значению 2.5...\n";
        fruc found1 = dq.find_by_value(2.5);
        cout << "Найдено: ";
        found1.show();

        cout << "\nВсего объектов fruc: " << fruc::getCount() << endl;


        cout << "\n\nТестирование Deque с int \n" << endl;
        Deque<int> intDeque;
        
        intDeque.push_back(10);
        intDeque.push_back(20);
        intDeque.push_back(30);
        
        cout << "Содержимое int дека: \n";
        intDeque.show_all();
        
        intDeque.push_front(5);
        cout << "\nПосле добавления в начало: \n\n";
        intDeque.show_all();


        cout << "\nТестирование Deque с float \n" << endl;
        Deque<float> floatDeque;
        
        floatDeque.push_back(1.5f);
        floatDeque.push_back(2.7f);
        floatDeque.push_back(3.14f);
        
        cout << "Содержимое float дека: \n\n";
        floatDeque.show_all();

    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }

    return 0;
}