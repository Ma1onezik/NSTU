#include "func.h"
#include <iostream>

using namespace std;

int main() {
    try {
        cout << "Создание объектов..." << endl;
        cout << "Количество объектов: " << func::getCount() << endl;
        
        func f1(2,3);
        func f2(3,4);
        func f3(6);
        func f4;
        
        cout << "Количество объектов: " << func::getCount() << endl;

        cout << "\nобычные объекты" << endl;
        cout << "f3 = " << f3 << endl;
        cout << "f4 = " << f4 << endl;
        
        func sum = f1 + f2;
        func diff = f1 - f2;
        func product = f1 * f2;
        func quotient = f1 / f2;
        
        cout << "\nарифметические операции:" << endl;
        cout << f1 << " + " << f2 << " = " << sum << endl;
        cout << f1 << " - " << f2 << " = " << diff << endl;
        cout << f1 << " * " << f2 << " = " << product << endl;
        cout << f1 << " / " << f2 << " = " << quotient << endl;

        cout << "\nдесятичное представление" << endl;
        cout << f1 << " = " << f1.todouble() << endl;
        cout << f2 << " = " << f2.todouble() << endl;

        cout << "\nКоличество объектов: " << func::getCount() << endl;

        func f5(4, 8);
        func f6(9, 6);

        cout << "\nКоличество объектов: " << func::getCount() << endl;
        
        cout << "\nсокращение" << endl;
        cout << "4/8" << " => " << f5 << endl;
        cout << "9/6" << " => " << f6 << endl;
        ч
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
        cout << "\nКоличество объектов: " << func::getCount() << endl;
    return 0;
}