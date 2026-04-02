#include "fruc.h"
#include <iostream>
#include <queue>
#include <map>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <string>
#include <deque>

using namespace std;
using namespace std::chrono;

vector<fruc> generateFractions(int count) {
    vector<fruc> fractions;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> numDist(-100, 100);
    uniform_int_distribution<> denDist(1, 100);
    
    for (int i = 0; i < count; ++i) {
        int num = numDist(gen);
        int den = denDist(gen);
        fractions.push_back(fruc(num, den));
    }
    return fractions;
}

void analyzeQueue(const vector<fruc>& data) {
    cout << "\nАнализ Queue\n" << endl;
    
    queue<fruc> q;
    
    auto start = high_resolution_clock::now();
    for (const auto& frac : data) {
        q.push(frac);
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "Добавление " << data.size() << " элементов: " 
         << duration.count() << " микросекунд" << endl;
    
    fruc target = data[data.size() / 2];
    start = high_resolution_clock::now();
    
    bool found = false;
    queue<fruc> temp = q;
    while (!temp.empty()) {
        if (temp.front() == target) {
            found = true;
            break;
        }
        temp.pop();
    }
    
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Поиск элемента: " << duration.count() 
         << " микросекунд (найден: " << (found ? "да" : "нет") << ")" << endl;

    start = high_resolution_clock::now();
    deque<fruc> tempDeque;
    while (!q.empty()) {
        tempDeque.push_back(q.front());
        q.pop();
    }
    sort(tempDeque.begin(), tempDeque.end());
    for (const auto& item : tempDeque) {
        q.push(item);
    }

end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Сортировка queue (через _Get_container): " << duration.count() 
         << " микросекунд" << endl;

    
    start = high_resolution_clock::now();
    while (!q.empty()) {
        q.pop();
    }
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Удаление всех элементов: " << duration.count() 
         << " микросекунд" << endl;
    
    cout << endl;


}


void analyzeMap(const vector<fruc>& data) {
    cout << "=== АНАЛИЗ MAP ===" << endl;
    
    map<fruc, string> fruitMap;
    
    auto start = high_resolution_clock::now();
    int index = 0;
    for (auto& fruit : data) {
        fruitMap[fruit] = "Value_" + to_string(++index);
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "Добавление " << data.size() << " элементов: " 
         << duration.count() << " микросекунд" << endl;
    
    fruc target = data[data.size() / 2];
    start = high_resolution_clock::now();
    auto it = fruitMap.find(target);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Поиск элемента: " << duration.count() 
         << " микросекунд (найден: " << (it != fruitMap.end() ? "да" : "нет") << ")" << endl;
    
    start = high_resolution_clock::now();
    for (auto it = fruitMap.begin(); it != fruitMap.end(); ) {
        it = fruitMap.erase(it);
    }
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Удаление всех элементов с итераторами: " << duration.count() 
         << " микросекунд" << endl;
    
}


int main() {
    try {
        const int DATA_SIZE = 10000;
        
        cout << "Генерация " << DATA_SIZE << " случайных дробей..." << endl;
        vector<fruc> testData = generateFractions(DATA_SIZE);
        cout << "Генерация завершена.\n" << endl;
        
        analyzeQueue(testData);
        analyzeMap(testData);
        
        cout << "Анализ для разных размеров данных\n" << endl;
        vector<int> sizes = {1000, 5000, 10000};
        
        for (int size : sizes) {
            cout << "\nРазмер данных: " << size << endl;
            vector<fruc> data = generateFractions(size);
            
            map<fruc, string> testMap;
            auto start = high_resolution_clock::now();
            for (int i = 0; i < size; ++i) {
                testMap[data[i]] = "value";
            }
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(end - start);
            cout << "  Map добавление: " << duration.count() << " мкс" << endl;
        }
        
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
    
    cout << "\nКоличество объектов: " << fruc::getCount() << endl;
    return 0;
}