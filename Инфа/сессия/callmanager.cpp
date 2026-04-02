#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum class calltype {income, outgoing};

class Call {
private:
    std::string phoneNumber;  
    std::string callTime;     
    int duration;             
    calltype type;            

public:
    Call (std::string phone, std::string time, int dur, calltype t) : phoneNumber(phone), callTime(time), duration(dur), type(t) {}

    std::string getPhoneNumber() const 
    { 
        return phoneNumber; 
    }

    std::string getCallTime() const 
    {
         return callTime; 
    }

    int getDuration() const 
    { 
        return duration; 
    }

    calltype getType() const 
    { 
        return type; 
    }

    void print() const {

        std::string typeStr = (type == calltype::income) ? "Входящий" : "Исходящий";
        
        int minutes = duration / 60;
        int seconds = duration % 60;
        
        std::cout << "Телефон: " << phoneNumber
                  << " | Время: " << callTime
                  << " | Длительность: " << minutes << "м " << seconds << "с"
                  << " | Тип: " << typeStr << std::endl;
    }
};

class Jurnal {
private:
    std::vector<Call> records;  

public:
    void addRecord(const Call record) 
    {
        records.push_back(record);
        std::cout << "Запись добавлена успешно!" << std::endl;
    }

    std::vector<Call> filterByPhone(const std::string& phoneNumber) const {
        std::vector<Call> filteredRecords;
        
        for (const auto& record : records) {
            if (record.getPhoneNumber() == phoneNumber) {
                filteredRecords.push_back(record);
            }
        }
        
        return filteredRecords;
    }

    void PrintAll() const {
        if (records.empty()) {
            cout << "Журнал вызовов пуст." << endl;
            return;
        }

        cout << "Всего записей: " << records.size() << endl;
        cout << "Все записи:" << endl;
        
        for (size_t i = 0; i < records.size(); i++) {
            cout << "Запись #" << i + 1 << ": ";
            records[i].print();
        }
    }

    size_t getRecordCount() const {
        return records.size();
    }

    void clear() {
        records.clear();
        std::cout << "Журнал очищен." << std::endl;
    }

    void statistic() const {
        if (records.empty()) {
            std::cout << "Нет данных для статистики." << std::endl;
            return;
        }

        int totalDuration = 0;
        int incomingCount = 0;
        int outgoingCount = 0;

        for (const auto& record : records) {
            totalDuration += record.getDuration();
            if (record.getType() == calltype::income) {
                incomingCount++;
            } else {
                outgoingCount++;
            }
        }

        int avgDuration = totalDuration / records.size();
        int avgMinutes = avgDuration / 60;
        int avgSeconds = avgDuration % 60;

        std::cout << "Статистика" << std::endl;
        std::cout << "Всего звонков: " << records.size() << std::endl;
        std::cout << "Входящих: " << incomingCount << std::endl;
        std::cout << "Исходящих: " << outgoingCount << std::endl;
        std::cout << "Общая длительность: " << totalDuration / 60 << " минут" << std::endl;
        std::cout << "Средняя длительность: " << avgMinutes << "м " << avgSeconds << "с" << std::endl;
    }
};

void printmenu() {
    std::cout << "\n\tМЕНЮ" << std::endl;
    std::cout << "1. Добавить запись" << std::endl;
    std::cout << "2. Показать все записи" << std::endl;
    std::cout << "3. Найти записи по номеру телефона" << std::endl;
    std::cout << "4. Показать статистику" << std::endl;
    std::cout << "5. Очистить журнал" << std::endl;
    std::cout << "6. Выход" << std::endl;
    std::cout << "Выберите действие: ";
}

int inputInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= 0) {
            std::cin.ignore();
            return value;
        } else {
            std::cout << "Ошибка! Введите целое неотрицательное число." << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
    }
}

int main() {
    Jurnal journal;
    int choice;

    do {
        printmenu();
        int value;
        cout << "Введите номер действия" << endl;
        if (cin >> value && value > 0) 
        {
            std::cin.ignore();
            choice = value;
        } 
        else 
        {
            std::cout << "Ошибка! Введите целое неотрицательное число." << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }


        switch (choice) {
            case 1: {
                std::string phone, time;
                int duration, typeChoice;

                std::cout << "Введите номер телефона: ";
                std::getline(std::cin, phone);
                
                std::cout << "Введите время вызова (формат: ЧЧ:ММ ДД.ММ.ГГГГ): ";
                std::getline(std::cin, time);
                
                duration = inputInt("Введите длительность вызова в секундах: ");
                
                std::cout << "Выберите тип вызова (0 - Входящий, 1 - Исходящий): ";
                typeChoice = inputInt("");
                
                calltype type = (typeChoice == 0) ? calltype::income : calltype::outgoing;
                
                Call record(phone, time, duration, type);
                journal.addRecord(record);
                break;
            }
            
            case 2:
                journal.PrintAll();
                break;
                
            case 3: {
                std::string phone;
                std::cout << "Введите номер телефона для поиска: ";
                std::getline(std::cin, phone);
                
                std::vector<Call> filtered = journal.filterByPhone(phone);
                
                if (filtered.empty()) {
                    std::cout << "Записи с номером " << phone << " не найдены." << std::endl;
                } else {
                    std::cout << "Найдено " << filtered.size() << " записей для номера " << phone << ":" << std::endl;
                    for (const auto& record : filtered) {
                        record.print();
                    }
                }
                break;
            }
            
            case 4:
                journal.statistic();
                break;
                
            case 5:
                journal.clear();
                break;
                
            case 6:
                std::cout << "Выход из программы." << std::endl;
                break;
                
            default:
                std::cout << "Неверный выбор! Попробуйте снова." << std::endl;
        }
        
    } while (choice != 6);

    return 0;
}