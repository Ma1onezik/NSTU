#include "fruc.h"
#include <cmath>
#include <stdexcept>
#include <iostream>


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

bool fruc::operator<(const fruc& other) const {
    return this->todouble() < other.todouble();
}

bool fruc::operator==(const fruc& other) const {
    return *numerator == *other.numerator && *denominator == *other.denominator;
}

fruc fruc::operator+(const fruc& other) const {
    return add(other);
}

fruc fruc::operator-(const fruc& other) const {
    return sub(other);
}

fruc fruc::operator*(const fruc& other) const {
    return mult(other);
}

fruc fruc::operator/(const fruc& other) const {
    return div(other);
}

fruc& fruc::operator=(const fruc& other) {
    if (this != &other) { 
        *numerator = *other.numerator;
        *denominator = *other.denominator;
        *stringRep = *other.stringRep;
    }
    return *this;
}

ostream& operator<<(ostream& os, const fruc& fraction) {
    os << *fraction.stringRep;
    return os;
}

double fruc::todouble() const {
    return static_cast<double>(*numerator) / *denominator;
}

int fruc::getCount() {
    return count;
}