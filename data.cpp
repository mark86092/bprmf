#include <iostream>
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include "data.h"

using namespace std;

Train::Train() {
    _Nuser = 0;
    _Nitem = 0;
    i_set = NULL;
}
Train::~Train() {
    delete [] i_set;
}
void Train::Read_File(const char * filename) {
    fstream file;
    file.open(filename, ios::in);
    assert(file != NULL);

    file >> _Nuser >> _Nitem;
    i_set = new unordered_set<size_t>[_Nuser];
    {
        size_t u;
        size_t i;
        while (file >> u >> i) {
            i_set[u].insert(i);
        }
    }
    file.close();
}
