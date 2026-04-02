#include <iostream>
#include <vector>

using namespace std;

// enum class typeofstud {budg, plat};

class student;

class groop {
    private:
    vector<student> students;

    public:
    
    void AddStudent(const student p)
    {
        students.push_back(p);
    }

    void SubStudent(int index) {
        if (index >= 0 && index < students.size()) {
            students.erase(students.begin() + index);
        }
    }

    void chtype (int i) {
        if (i >= 0 && i < students.size()) {
            students[i].ctype();
        }
    }

    int findStudent(const string& name, const string& famil, const string& otchestvo) const {
        for (size_t i = 0; i < students.size(); i++) {
            if (students[i].getname() == name && 
                students[i].getfamil() == famil && 
                students[i].getotchestvo() == otchestvo) {
                return i;
            }
        }
        return -1;
    }

    void print() 
    {
        for (const auto& student : students) {
            cout << "Имя: " << student.getname() << endl;
            cout << "Фамилия: " << student.getfamil() << endl;
            cout << "Отчество: " << student.getotchestvo() << endl;
            cout << "Номер группы: " << student.nomgr() << endl;
            cout << "Почта: " << student.getemail() << endl;
            cout << "Средняя оценка: " << student.getmark() << endl;
            string t = (student.gettype() == true) ? "бюджет" : "платное" ;
            cout << "Тип обучения: " << t << endl;

        }
    }
};

class student {
    private:
    int nomgroop;
    string name;
    string famil;
    string otchestvo;
    string email;
    float agvmark;
    bool type = true;
    
    public:
    student(int nm ,string p_name, string p_famil, string p_otchestvo, string p_email, float p_agvmark, bool t) :nomgroop(nm), name(p_name), famil(p_famil), otchestvo(p_otchestvo), email(p_email), agvmark(p_agvmark), type(t) {}

    void ctype() {
        if (type == true) type = false;
        else 
        {
            cout << "студент и так на бюджете" << endl;
        }
    }

    bool gettype () const
    {
        return type;
    }

    float getmark () const
    {
        return agvmark;
    }

    string getemail () const
    {
        return email;
    }

    int nomgr () const
    {
        return nomgroop;
    }

    string getname () const
    {
        return name;
    }

    string getfamil () const
    {
        return famil;
    }

    string getotchestvo () const
    {
        return otchestvo;
    }

    
};

int main () {
    groop cl;
    for 
 
    return 0;
}

