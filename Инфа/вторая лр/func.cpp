#include "func.h"
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <sstream>

using namespace std;

int func::count = 0;

int func::NOD(int a, int b) const {
    a = abs(a);
    b = abs(b);
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

void func::reduction() {
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

void func::upd_str_repres() {
    stringstream ss;
    if (*denominator == 1) {
        ss << *numerator;
    } else {
        ss << *numerator << "/" << *denominator;
    }
    *stringRep = ss.str();
}

func::func() {
    numerator = new int(0);
    denominator = new int(1);
    stringRep = new string("0");
    count++;
}

func::func(int num, int denom) {
    numerator = new int(num);
    denominator = new int(denom);
    stringRep = new string();
    try {
        reduction();
        upd_str_repres();
    } catch (...) {
        delete numerator;
        delete denominator;
        delete stringRep;
        throw;
    }
    count++;
}

func::func(const func& other) {
    numerator = new int(*other.numerator);
    denominator = new int(*other.denominator);
    stringRep = new string(*other.stringRep);
    count++;
}

func::func(int num) {
    numerator = new int(num);
    denominator = new int(1);
    stringRep = new string();
    upd_str_repres();
    count++;
}

func::~func() {
    delete numerator;
    delete denominator;
    delete stringRep;
    count--;
}

int func::getNumerator() const { return *numerator; }
int func::getDenominator() const { return *denominator; }
string func::getStringRepresentation() const { return *stringRep; }

func func::add(const func& other) const {
    int newNumerator = *numerator * *other.denominator + *other.numerator * *denominator;
    int newDenominator = *denominator * *other.denominator;
    return func(newNumerator, newDenominator);
}

func func::sub(const func& other) const {
    int newNumerator = *numerator * *other.denominator - *other.numerator * *denominator;
    int newDenominator = *denominator * *other.denominator;
    return func(newNumerator, newDenominator);
}

func func::mult(const func& other) const {
    int newNumerator = *numerator * *other.numerator;
    int newDenominator = *denominator * *other.denominator;
    return func(newNumerator, newDenominator);
}

func func::div(const func& other) const {
    if (*other.numerator == 0) {
        throw runtime_error("Division by zero");
    }
    int newNumerator = *numerator * *other.denominator;
    int newDenominator = *denominator * *other.numerator;
    return func(newNumerator, newDenominator);
}

void func::print() const {
    cout << *stringRep << endl;
}

func func::operator+(const func& other) const {
    return add(other);
}

func func::operator+(int value) const {
    return add(func(value, 1));
}

func func::operator-(const func& other) const {
    return sub(other);
}

func func::operator-(int value) const {
    return sub(func(value, 1));
}

func func::operator*(const func& other) const {
    return mult(other);
}

func func::operator/(const func& other) const {
    return div(other);
}

func& func::operator=(const func& other) {
    if (this != &other) {
        *numerator = *other.numerator;
        *denominator = *other.denominator;
        *stringRep = *other.stringRep;
    }
    return *this;
}

func operator+(int value, const func& frac) {
    return func(value, 1) + frac;
}

func operator-(int value, const func& frac){
    return func(value, 1) - frac;
}

func operator/(int value, const func& frac) {
    return func(value, 1) / frac;
}

func operator*(int value, const func& frac) {
    return func(value, 1) * frac;
}

ostream& operator<<(ostream& os, const func& frac) {
    os << frac.getStringRepresentation();
    return os;
}

double func::todouble() const {
    return static_cast<double>(*numerator) / *denominator;
}

float func::tofloat() const {
    return static_cast<float>(*numerator) / *denominator;
}

int func::getCount() {
    return count;
}