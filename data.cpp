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

/* 對每個 u 當中的每個 i_set(u) 元素都 sample ji_ratio 個 j*/
vector<Pair> Train::draw_sample(const int ji_ratio) const {
    vector<Pair> samples;
    for (size_t u = 0; u < Nuser(); u++) {
        for (auto it = i_set[u].begin(); it != i_set[u].end(); it++) {
            size_t i = *it;
            for (size_t Npair = 0; Npair < (size_t)ji_ratio; ) {
                size_t j = rand() % Nitem();
                if (i_set[u].find(j) == i_set[u].end()) {
                    Npair++;
                    samples.push_back(Pair(u, i, j));
                }
            }
        }
    }
    random_shuffle(samples.begin(), samples.end());
    return samples;
}