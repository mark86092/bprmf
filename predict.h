#ifndef __PREDICT_HPP__
#define __PREDICT_HPP__

#include <iostream>
#include <vector>
#include "model.h"
#include "data.h"
#include "pair.h"

using namespace std;

vector<Pair_i_y> predict_top_N(const Train & train, const Model & model, const size_t u, const size_t N);

#endif
