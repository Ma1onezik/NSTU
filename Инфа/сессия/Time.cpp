#include <iostream>
#include <string>

using namespace std;

class Time {
private:
    int sec;
    int min;
    int hour;
    int day;
    int mon;
    int year;

    void NormTime() {

    if(sec >= 60) {
        min += sec / 60;
        sec %= 60;
    } else if (sec < 0) {
        int borr = (-sec + 59) / 60;
        min -= borr;
        sec += borr * 60;
    }

    if (min >= 60) {
        hour += min / 60;
        min %= 60;
    } else if (min < 0) {
        int borr = (-min + 59) / 60;
        hour -= borr;
        min += borr * 60;
    }
    
    if (hour >= 24) {
        day += hour / 24;
        hour %= 24;
    } else if (hour < 0) {
        int borr = (-hour + 23) / 24;
        day -= borr;
        hour += borr * 24;
    }

    if (mon > 12) {
        year += (mon - 1) / 12;  
        mon = ((mon - 1) % 12) + 1;
    } else if (mon < 1) {
        year -= (1 - mon + 11) / 12; 
        mon = 12 - ((1 - mon - 1) % 12);
        if (mon == 0) mon = 12;
    }

    while (day < 1) {
        mon--;
        if (mon < 1) {
            mon = 12;
            year--;
        }
        day += getDaysInMonth(mon);
    }
    
    while (day > getDaysInMonth(mon)) {
        day -= getDaysInMonth(mon);
        mon++;
        if (mon > 12) {
            mon = 1;
            year++;
        }
    }
}

    int getDaysInMonth(int m) const {
        switch (m) {
            case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                return 31;
            case 4: case 6: case 9: case 11:
                return 30;
            case 2:
                return 28;
            default:
                return 0;
        }
    }

    void addDaysInternal(int days) {
        day += days;
        NormTime();
    }

    void subtractDaysInternal(int days) {
        day -= days;
        NormTime();
    }

public:
    Time() : day(1), mon(1), year(2000), hour(0), min(0), sec(0) {}
    
    Time(int d, int m, int y, int h = 0, int mn = 0, int s = 0) 
        : day(d), mon(m), year(y), hour(h), min(mn), sec(s) {
        NormTime();
    }

    void addSeconds(int n) {
        sec += n;
        NormTime();
    }

    void subSeconds(int n) {
        sec -= n;
        NormTime();        
    }

    void addDay(int n) {
        addDaysInternal(n); 
    }

    void subDay(int n) {
        subtractDaysInternal(n);
    }

    void PrintTime() const {
        cout << "Дата и время: " << day << '.' << mon << '.' << year << "." 
             << hour << '.' << min << '.' << sec << endl;
    }

    void inputFromConsole() {
        char dot;
        
        cout << "Введите дату и время в формате день.месяц.год.часы.минуты.секунды" << endl;
        cout << "Пример: 12.03.2006.11.32.56" << endl;
        cout << "Ваш ввод: ";

        cin >> day >> dot >> mon >> dot >> year >> dot 
            >> hour >> dot >> min >> dot >> sec;
        
        NormTime();
    }
    
    int getDay() const { return day; }
    int getMonth() const { return mon; }
    int getYear() const { return year; }
};

int main() {
    cout << "Программа запущена" << endl;
    
    Time dt;
    int choice;
    
    cout << "Добро пожаловать, это программа для работы с датами" << endl;
    cout << "Вы хотите:" << endl;
    cout << "1. Использовать дату по умолчанию (01.01.2000.00.00.00)" << endl;
    cout << "2. Ввести свою дату в формате дд.мм.гггг.чч.мм.сс" << endl;
    cout << "Ваш выбор: ";
    cin >> choice;
    
    if (choice == 2) {
        dt.inputFromConsole();
    }
    
    cout << "\nТекущая дата и время: ";
    dt.PrintTime();
    
    while (true) {
        cout << "\nМЕНЮ ОПЕРАЦИЙ:" << endl;
        cout << "1. Добавить секунды" << endl;
        cout << "2. Вычесть секунды" << endl;
        cout << "3. Добавить дни" << endl;
        cout << "4. Вычесть дни" << endl;
        cout << "5. Показать текущее время" << endl;
        cout << "6. Выход" << endl;
        cout << "Выберите операцию: ";
        
        int operation;
        cin >> operation;
        
        if (operation == 6) {
            cout << "Выход из программы." << endl;
            break;
        }
        
        int value;
        switch (operation) {
            case 1:
                cout << "Введите количество секунд для добавления: ";
                cin >> value;
                dt.addSeconds(value);
                cout << "После добавления " << value << " секунд: ";
                dt.PrintTime();
                break;
                
            case 2:
                cout << "Введите количество секунд для вычитания: ";
                cin >> value;
                dt.subSeconds(value);
                cout << "После вычитания " << value << " секунд: ";
                dt.PrintTime();
                break;
                
            case 3:
                cout << "Введите количество дней для добавления: ";
                cin >> value;
                dt.addDay(value);
                cout << "После добавления " << value << " дней: ";
                dt.PrintTime();
                break;
                
            case 4:
                cout << "Введите количество дней для вычитания: ";
                cin >> value;
                dt.subDay(value);
                cout << "После вычитания " << value << " дней: ";
                dt.PrintTime();
                break;
                
            case 5:
                cout << "Текущая дата и время: ";
                dt.PrintTime();
                break;
                
            default:
                cout << "Неверный выбор. Попробуйте снова." << endl;
        }
    }
    
    return 0;
}