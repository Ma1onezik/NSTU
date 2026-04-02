#ifndef FUNC_H
#define FUNC_H

#include <iostream>
#include <string>
#include <sstream>

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
    fruc(int num, int denom = 1);         
    fruc(const fruc& other);              
    
    
    ~fruc();
    
    int getNumerator() const;
    int getDenominator() const;
    std::string getStringRepresentation() const;
    
    fruc add(const fruc& other) const;
    fruc sub(const fruc& other) const;
    fruc mult(const fruc& other) const;
    fruc div(const fruc& other) const;
    
    void print() const;

    bool operator<(const fruc&other) const;
    bool operator==(const fruc&other) const;
    
    fruc operator+(const fruc& other) const;
    fruc operator-(const fruc& other) const;
    fruc operator*(const fruc& other) const;
    fruc operator/(const fruc& other) const;
    fruc& operator=(const fruc& other);
    
    
    friend std::ostream& operator<<(std::ostream& os, const fruc& fraction);
    
    double todouble() const;
    
    static int getCount();
};

#endif

//push
//pop
//found
//sort