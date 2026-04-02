#ifndef FUNC_H
#define FUNC_H

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <stdexcept>

class func {
private:
    int* numerator;
    int* denominator;
    std::string* stringRep;
    static int count;

    int NOD(int a, int b) const;
    void reduction();
    void upd_str_repres();

public:
    func();
    func(int num, int denom);
    func(const func& other);
    func(int num);
    ~func();

    int getNumerator() const;
    int getDenominator() const;
    std::string getStringRepresentation() const;

    func add(const func& other) const;
    func sub(const func& other) const;
    func mult(const func& other) const;
    func div(const func& other) const;

    void print() const;

    func operator+(const func& other) const;
    func operator+(int value) const;
    func operator-(const func& other) const;
    func operator-(int value) const;
    func operator*(const func& other) const;
    func operator/(const func& other) const;
    func& operator=(const func& other);

    friend func operator+(int value, const func& frac);
    friend func operator-(int value, const func& frac);
    friend func operator*(int value, const func& frac);
    friend func operator/(int value, const func& frac);
    friend func operator*(int value, const func& frac);
    friend func operator/(int value, const func& frac);
    friend std::ostream& operator<<(std::ostream& os, const func& frac);

    double todouble() const;
    float tofloat() const;

    static int getCount();
};

#endif // FUNC_H