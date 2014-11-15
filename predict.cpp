#include <iostream>
#include "model.h"
#include "data.h"
#include "pair.h"

using namespace std;

bool sort_by_y(const Pair_i_y &i, const Pair_i_y &j) {
    return (i.y > j.y);
}

vector<Pair_i_y> predict_top_N(const Train & train, const Model & model, const size_t u, const size_t N)
{
    vector<Pair_i_y> predict;
    for (size_t i = 0; i < train.Nitem(); i++) {
        if (i == u)
            continue;
        if (train.i_set[u].find(i) != train.i_set[u].end())
            continue;
        double y = model.P.row(u).dot(model.Q.row(i));
        predict.push_back(Pair_i_y(i, y));
    }
    
    sort(predict.begin(), predict.end(), sort_by_y);
    return predict;
}