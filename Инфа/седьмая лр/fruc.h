#ifndef FRUC_H
#define FRUC_H

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <stdexcept>
#include <fstream>

class fruc {
private:
    int* numerator;
    int* denominator;
    std::string* stringRep;
    static int count;

    int NOD(int a, int b) const;
    void reduction();
    void upd_str_repres();

public:
    fruc();
    fruc(int num, int denom);
    fruc(const fruc& other);
    fruc(int num);
    virtual ~fruc();

    int getNumerator() const;
    int getDenominator() const;
    std::string getStringRepresentation() const;

    fruc add(const fruc& other) const;
    fruc sub(const fruc& other) const;
    fruc mult(const fruc& other) const;
    fruc div(const fruc& other) const;

    void print() const;

    fruc operator+(const fruc& other) const;
    fruc operator+(int value) const;
    fruc operator-(const fruc& other) const;
    fruc operator-(int value) const;
    fruc operator*(const fruc& other) const;
    fruc operator/(const fruc& other) const;
    fruc& operator=(const fruc& other);

    friend fruc operator+(int value, const fruc& func);
    friend fruc operator-(int value, const fruc& func);
    friend fruc operator*(int value, const fruc& func);
    friend fruc operator/(int value, const fruc& func);

    friend std::ostream& operator<<(std::ostream& os, const fruc& func);
    friend std::istream& operator>>(std::istream& is, fruc& func);

    double todouble() const;
    float tofloat() const;

    static int getCount();

    void saveText(std::ofstream& ofs) const;
    void loadText(std::ifstream& ifs);

    void saveBinary(std::ofstream& ofs) const;
    void loadBinary(std::ifstream& ifs);

    virtual void show() const;
};

class mixedfruc : public fruc {
private: 
    int wholePart;
public:
    mixedfruc(int num = 0, int denom = 1);
    mixedfruc(const fruc& other);

    void updMixed();
    void printMixed() const;
    void show() const override;
};

class resultfruc : public fruc {
private:
    double value;
public:
    resultfruc(int num = 0, int denom = 1);
    resultfruc(const fruc& other);

    double getValue() const; 
    void updValue();
    void show() const override;
};

template <typename T>
class Deque {
private:
    class Node {
    public:
        T data;
        Node* next;
        Node(T d) : data(d), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int size;

public:
    Deque();
    ~Deque();

    void push_front(T obj);
    void push_back(T obj);
    void pop_front();

    void insert_at(int index, T obj);
    void remove_at(int index);
    T find_by_value(double value) const;

    void show_all() const;
    int getSize() const;
};

template <> int Deque<int>::find_by_value(double value) const;
template <> float Deque<float>::find_by_value(double value) const;
template <> double Deque<double>::find_by_value(double value) const;
template <> fruc Deque<fruc>::find_by_value(double value) const;
template <> mixedfruc Deque<mixedfruc>::find_by_value(double value) const;
template <> resultfruc Deque<resultfruc>::find_by_value(double value) const;

template <> void Deque<fruc>::show_all() const;
template <> void Deque<mixedfruc>::show_all() const;
template <> void Deque<resultfruc>::show_all() const;

#endif