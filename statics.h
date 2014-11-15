#ifndef __STATICS_HPP__
#define __STATICS_HPP__

#include <iostream>
#include <vector>
#include <unordered_set>
#include "model.h"
#include "data2.h"

#ifndef INT_INF
    #define INT_INF 2147483647
#endif

using namespace std;

bool myfunction(const Pair_i_yui &i, const Pair_i_yui &j);
bool sort_by_y(const Pair_i_yui &i, const Pair_i_yui &j);

double calculate_auc(size_t u, vector<Pair_i_yui> &, const unordered_set<size_t> &, const Model &);
double calculate_auc(const Test_Data &, const Model &);
double calculate_auc_by_draw_sample(__Data &, const Model &, int ji_ratio);
double calc_stdev(unordered_set<size_t> & item_set,vector< vector<size_t> > & rank_result, int top_n, const char * filename);
double stdev_actual(DataSet & data, const Model & model, int top_n, const char * filename);
double *stdev_topn(DataSet & data, const Model & model, int* top_n, int top_num, int stdev_type, const char * filename);



#endif
