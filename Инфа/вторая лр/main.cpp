#include "func.h"
#include <iostream>

using namespace std;

int main() {
    try {
        cout << "Создание объектов..." << endl;
        cout << "Количество объектов: " << func::getCount() << endl;
        
        func f1(2,3);
        func f2(3,4);
        func f3(6, 1);  
        func f4;
        
        cout << "Количество объектов: " << func::getCount() << endl;

        cout << "\nобычные объекты" << endl;
        cout << "f1 = " << f1 << endl;
        cout << "f2 = " << f2 << endl;
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
    
        cout << "\nчисло с дробью:" << endl;
        func sum_int_frac = f3 + f1;
        func diff_int_frac = f3 - f1;
        cout << f3 << " + " << f1 << " = " << sum_int_frac << endl;
        cout << f3 << " - " << f1 << " = " << diff_int_frac << endl;

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
        
        cout << "\nдополнительные операции:" << endl;
        func f7 = 5 + f1;  
        func f8 = 2 - f2;  
        func f9 = 3 * f1;  
        func f10 = 4 / f2; 
        
        cout << "5 + " << f1 << " = " << f7 << endl;
        cout << "2 - " << f2 << " = " << f8 << endl;
        cout << "3 * " << f1 << " = " << f9 << endl;
        cout << "4 / " << f2 << " = " << f10 << endl;
        
        cout << "\nприсваивание:" << endl;
        func f11;
        f11 = f1;
        cout << "f11 = f1 = " << f11 << endl;
        
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
    
    cout << "\nКоличество объектов после завершения: " << func::getCount() << endl;
    return 0;
}