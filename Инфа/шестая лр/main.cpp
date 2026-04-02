#include "fruc.h"
#include <iostream>
using namespace std;

int main() {
    cout << "Testing exceptions..." << endl;
    
    try {
        fruc f1(1, 0);
        cout << "ERROR: NO exception for zero denominator!" << endl;
    } catch (const exception& e) {
        cout << "PASS: " << e.what() << endl;
    }

    try {
        fruc f1(1, 2);
        fruc f2(0, 1);
        fruc result = f1 / f2;
        cout << "ERROR: No exception for division by zero!" << endl;
    } catch (const exception& e) {
        cout << "PASS: " << e.what() << endl;
    }

    try {
        Deque dq;
        dq.pop_front();
        cout << "ERROR: No exception for empty deque pop!" << endl;
    } catch (const exception& e) {
        cout << "PASS: " << e.what() << endl;
    }

    try {
        Deque dq;
        dq.push_back(new fruc(1, 2));
        dq.remove_at(5);
        cout << "ERROR: No exception for invalid index!" << endl;
    } catch (const exception& e) {
        cout << "PASS: " << e.what() << endl;
    }

    try {
        fruc f;
        ifstream ifs("nonexistent.txt");
        f.loadText(ifs);
        cout << "ERROR: No exception for file error!" << endl;
    } catch (const exception& e) {
        cout << "PASS: " << e.what() << endl;
    }

    cout << "All exception tests completed!" << endl;
    return 0;
}