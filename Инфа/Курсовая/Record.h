#ifndef RECORD_H
#define RECORD_H

#include <vector>
#include <fstream>
#include <cstring>

struct Record {
    int key;               
    char data[64];         
};


struct IndexEntry {
    int key;               
    long pos;              
    
    bool operator<(const IndexEntry& other) const {
        return key < other.key;
    }
};

#endif