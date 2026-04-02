#include "fruc.h"
using namespace std;

int fruc::count = 0;

int fruc::NOD(int a, int b) const {
    a = abs(a);
    b = abs(b);
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

void fruc::reduction() {
    if (*denominator == 0) {
        throw runtime_error("Denominator cannot be zero");
    }
    if (*denominator < 0) {
        *numerator = -*numerator;
        *denominator = -*denominator;
    }
    int commonDivisor = NOD(*numerator, *denominator);
    *numerator /= commonDivisor;
    *denominator /= commonDivisor;
}

void fruc::upd_str_repres() {
    stringstream ss;
    if (*denominator == 1) {
        ss << *numerator;
    } else {
        ss << *numerator << "/" << *denominator;
    }
    *stringRep = ss.str();
}

fruc::fruc() {
    numerator = new int(0);
    denominator = new int(1);
    stringRep = new string("0");
    count++;
}

fruc::fruc(int num, int denom) {
    numerator = new int(num);
    denominator = new int(denom);
    stringRep = new string();
    reduction();
    upd_str_repres();
    count++;
}

fruc::fruc(const fruc& other) {
    numerator = new int(*other.numerator);
    denominator = new int(*other.denominator);
    stringRep = new string(*other.stringRep);
    count++;
}

fruc::fruc(int num) {
    numerator = new int(num);
    denominator = new int(1);
    stringRep = new string();
    upd_str_repres();
    count++;
}

fruc::~fruc() {
    delete numerator;
    delete denominator;
    delete stringRep;
    count--;
}

int fruc::getNumerator() const { return *numerator; }
int fruc::getDenominator() const { return *denominator; }
string fruc::getStringRepresentation() const { return *stringRep; }

fruc fruc::add(const fruc& other) const {
    int newNumerator = *numerator * *other.denominator + *other.numerator * *denominator;
    int newDenominator = *denominator * *other.denominator;
    return fruc(newNumerator, newDenominator);
}

fruc fruc::sub(const fruc& other) const {
    int newNumerator = *numerator * *other.denominator - *other.numerator * *denominator;
    int newDenominator = *denominator * *other.denominator;
    return fruc(newNumerator, newDenominator);
}

fruc fruc::mult(const fruc& other) const {
    int newNumerator = *numerator * *other.numerator;
    int newDenominator = *denominator * *other.denominator;
    return fruc(newNumerator, newDenominator);
}

fruc fruc::div(const fruc& other) const {
    if (*other.numerator == 0) {
        throw runtime_error("Division by zero");
    }
    int newNumerator = *numerator * *other.denominator;
    int newDenominator = *denominator * *other.numerator;
    return fruc(newNumerator, newDenominator);
}

void fruc::print() const {
    cout << *stringRep << endl;
}

fruc fruc::operator+(const fruc& other) const { return add(other); }
fruc fruc::operator+(int value) const { return add(fruc(value, 1)); }
fruc fruc::operator-(const fruc& other) const { return sub(other); }
fruc fruc::operator-(int value) const { return sub(fruc(value, 1)); }
fruc fruc::operator*(const fruc& other) const { return mult(other); }
fruc fruc::operator/(const fruc& other) const { return div(other); }

fruc& fruc::operator=(const fruc& other) {
    if (this != &other) {
        *numerator = *other.numerator;
        *denominator = *other.denominator;
        *stringRep = *other.stringRep;
    }
    return *this;
}

fruc operator+(int value, const fruc& func) { return fruc(value, 1) + func; }
fruc operator-(int value, const fruc& func) { return fruc(value, 1) - func; }
fruc operator*(int value, const fruc& func) { return fruc(value, 1) * func; }
fruc operator/(int value, const fruc& func) { return fruc(value, 1) / func; }

ostream& operator<<(ostream& os, const fruc& func) {
    os << func.getStringRepresentation();
    return os;
}

istream& operator>>(istream& is, fruc& func) {
    int num, denom;
    char slash;
    if (is >> num >> slash >> denom && slash == '/') {
        *func.numerator = num;
        *func.denominator = denom;
        func.reduction();
        func.upd_str_repres();
    } else {
        is.setstate(ios::failbit);
    }
    return is;
}

double fruc::todouble() const {
    return static_cast<double>(*numerator) / *denominator;
}

float fruc::tofloat() const {
    return static_cast<float>(*numerator) / *denominator;
}

int fruc::getCount() {
    return count;
}

void fruc::saveText(ofstream& ofs) const {
    ofs << *numerator << "/" << *denominator << "\n";
}

void fruc::loadText(ifstream& ifs) {
    int num, denom;
    char slash;
    ifs >> num >> slash >> denom;
    if (ifs) {
        *numerator = num;
        *denominator = denom;
        reduction();
        upd_str_repres();
    }
}

void fruc::saveBinary(ofstream& ofs) const {
    ofs.write(reinterpret_cast<const char*>(numerator), sizeof(int));
    ofs.write(reinterpret_cast<const char*>(denominator), sizeof(int));
}

void fruc::loadBinary(ifstream& ifs) {
    ifs.read(reinterpret_cast<char*>(numerator), sizeof(int));
    ifs.read(reinterpret_cast<char*>(denominator), sizeof(int));
    if (ifs) {
        reduction();
        upd_str_repres();
    }
}

void fruc::show() const {
    cout << "Обычная дробь: " << *stringRep << endl;
}

// Наследники
resultfruc::resultfruc(int num, int denom) : fruc(num, denom) {
    value = todouble();
}

resultfruc::resultfruc(const fruc& other) : fruc(other) {
    value = todouble();
}

double resultfruc::getValue() const {return value; }

void resultfruc::updValue() { 
    value = todouble(); 
}

void resultfruc::show() const {
    cout << "ResultFruc: " << getStringRepresentation()
         << " = " << value << endl;
}

void mixedfruc::updMixed() {
    wholePart = getNumerator() / getDenominator();
}

void mixedfruc::printMixed() const {
    int num = getNumerator() % getDenominator();
    if (num == 0) {
        cout << wholePart << endl;
    } else {
        cout << wholePart << ' ' << num << '/' << getDenominator() << endl;
    }
}

mixedfruc::mixedfruc(int num, int denom) : fruc(num, denom) {
    updMixed();
}

mixedfruc::mixedfruc(const fruc& other) : fruc(other) {
    updMixed();
}

void mixedfruc::show() const {
    cout << "MixedFruc: ";
    printMixed();
}


template <typename T>
Deque<T>::Deque() : head(nullptr), tail(nullptr), size(0) {}

template <typename T>
Deque<T>::~Deque() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

template <typename T>
void Deque<T>::push_front(T obj) {
    Node* newNode = new Node(obj);
    if (head == nullptr) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head = newNode;
    }
    size++;
}

template <typename T>
void Deque<T>::push_back(T obj) {
    Node* newNode = new Node(obj);
    if (tail == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

template <typename T>
void Deque<T>::pop_front() {
    if (head == nullptr) {
        throw runtime_error("Нет объектов для удаления");
    }
    Node* temp = head;
    head = head->next;
    if (head == nullptr) {
        tail = nullptr;
    }
    delete temp;
    size--;
}

template <typename T>
void Deque<T>::insert_at(int index, T obj) {
    if (index < 0 || index > size)
        throw runtime_error("Некорректный индекс добавления");

    if (index == 0) {
        push_front(obj);
        return;
    }
    if (index == size) {
        push_back(obj);
        return;
    }

    Node* cur = head;
    for (int i = 0; i < index - 1; i++) cur = cur->next;

    Node* node = new Node(obj);
    node->next = cur->next;
    cur->next = node;

    size++;
}

template <typename T>
void Deque<T>::remove_at(int index) {
    if (index < 0 || index >= size)
        throw runtime_error("Некорректный индекс удаления");

    if (index == 0) {
        pop_front();
        return;
    }

    Node* cur = head;
    for (int i = 0; i < index - 1; i++) cur = cur->next;

    Node* temp = cur->next;
    cur->next = temp->next;

    if (!cur->next) tail = cur;

    delete temp;
    size--;
}


template <typename T>
T Deque<T>::find_by_value(double value) const {
    throw runtime_error("Поиск по значению не поддерживается для этого типа");
}


template <typename T>
void Deque<T>::show_all() const {
    Node* cur = head;
    int i = 0;
    while (cur) {
        cout << '[' << i++ << "] " << cur->data << endl;
        cur = cur->next;
    }
}

template <typename T>
int Deque<T>::getSize() const { return size; }


template <>
int Deque<int>::find_by_value(double value) const {
    Node* cur = head;
    while (cur) {
        if (fabs(static_cast<double>(cur->data) - value) < 1e-6)  
            return cur->data;
        cur = cur->next;
    }
    throw runtime_error("Объект не найден"); 
}

template <>
float Deque<float>::find_by_value(double value) const {
    Node* cur = head;
    while (cur) {
        if (fabs(static_cast<double>(cur->data) - value) < 1e-6)
            return cur->data;
        cur = cur->next;
    }
    throw runtime_error("Объект не найден");
}

template <>
double Deque<double>::find_by_value(double value) const {
    Node* cur = head;
    while (cur) {
        if (fabs(cur->data - value) < 1e-6)
            return cur->data;
        cur = cur->next;
    }
    throw runtime_error("Объект не найден");
}

template <>
fruc Deque<fruc>::find_by_value(double value) const {
    Node* cur = head;
    while (cur) {
        if (fabs(cur->data.todouble() - value) < 1e-6)
            return cur->data;
        cur = cur->next;
    }
    throw runtime_error("Объект не найден");
}

template <>
mixedfruc Deque<mixedfruc>::find_by_value(double value) const {
    Node* cur = head;
    while (cur) {
        if (fabs(cur->data.todouble() - value) < 1e-6)
            return cur->data;
        cur = cur->next;
    }
    throw runtime_error("Объект не найден");
}

template <>
resultfruc Deque<resultfruc>::find_by_value(double value) const {
    Node* cur = head;
    while (cur) {
        if (fabs(cur->data.todouble() - value) < 1e-6)
            return cur->data;
        cur = cur->next;
    }
    throw runtime_error("Объект не найден");
}


template <>
void Deque<fruc>::show_all() const {
    Node* cur = head;
    int i = 0;
    while (cur) {
        cout << '[' << i++ << "] ";
        cur->data.show();
        cur = cur->next;
    }
}

template <>
void Deque<mixedfruc>::show_all() const {
    Node* cur = head;
    int i = 0;
    while (cur) {
        cout << '[' << i++ << "] ";
        cur->data.show();
        cur = cur->next;
    }
}

template <>
void Deque<resultfruc>::show_all() const {
    Node* cur = head;
    int i = 0;
    while (cur) {
        cout << '[' << i++ << "] ";
        cur->data.show();
        cur = cur->next;
    }
}

template class Deque<int>;
template class Deque<float>;
template class Deque<fruc>;
template class Deque<mixedfruc>;
template class Deque<resultfruc>;