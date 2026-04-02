#include "fruc.h"
#include <iostream>
#include <vector>
using namespace std;

int main() {
    try {
        fruc f1(7, 3);
        fruc f2(5, 2);

        cout << "\nВведенные дроби" << endl;
        cout << "f1 = " << f1 << " f2 = " << f2 << endl;

        resultfruc r1(f1);
        cout << "\nКласс resultfruc" << endl;
        cout << "f1 -> double = " << r1.getValue() << endl;

        mixedfruc r2(f2);
        cout << "\nКласс mixedfruc" << endl;
        cout << "f2 -> ";
        r2.printMixed();


        mixedfruc r3;
        cout << "\nКопирование" << endl;
        r3 = r2;
        r3.printMixed();

    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }

    cout << "\nКоличество объектов после завершения: " << fruc::getCount() << endl;
    return 0;
}
