#include "func.h"
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

int main() {
    try {
        vector<func> fractions;


        int n;
        cout << "Сколько дробей вы хотите ввести? ";
        cin >> n;
        if (n <= 0) {
            cerr << "Количество должно быть положительным!" << endl;
            return 1;
        }

        cout << "Введите " << n << " дробей в формате a/b:\n";
        for (int i = 0; i < n; i++) {
            func f;
            cin >> f;
            if (!cin) {
                cerr << "Ошибка ввода дроби!" << endl;
                return 1;
            }
            fractions.push_back(f);
        }
        
        cout << "\nВведенные дроби:\n";
        for (auto& f : fractions) {
            cout << f << endl;
        }

        ofstream textFile("fractions.txt");
        if (!textFile) {
            cerr << "Ошибка открытия файла fractions.txt для записи!" << endl;
            return 1;
        }
        for (auto& f : fractions) {
            textFile << f << endl;  
        }
        textFile.close();

        ofstream binFile("fractions.bin", ios::binary);
        if (!binFile) {
            cerr << "Ошибка открытия файла fractions.bin для записи!" << endl;
            return 1;
        }
        for (auto& f : fractions) {
            f.saveBinary(binFile);
        }
        binFile.close();

        cout << "\nЗагрузка из fractions.txt:\n";
        ifstream textIn("fractions.txt");
        if (!textIn) {
            cerr << "Ошибка открытия файла fractions.txt для чтения!" << endl;
            return 1;
        }
        while (textIn.peek() != EOF) {
            func tmp;
            textIn >> tmp;  
            if (textIn) {
                cout << tmp << endl;
            }
        }
        textIn.close();


        cout << "\nЗагрузка из fractions.bin:\n";
        ifstream binIn("fractions.bin", ios::binary);
        if (!binIn) {
            cerr << "Ошибка открытия файла fractions.bin для чтения!" << endl;
            return 1;
        }
        while (binIn.peek() != EOF) {
            func tmp;
            tmp.loadBinary(binIn);
            if (binIn) {
                cout << tmp << endl;
            }
        }
        binIn.close();


        

    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    cout << "\nКоличество объектов после завершения: " << func::getCount() << endl;
    return 0;
}