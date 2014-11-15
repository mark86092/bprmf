#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include "pair.h"

using namespace Eigen;

class Model {
    public:
        MatrixXd P;
        MatrixXd Q;
        size_t Nuser;
        size_t Nitem;
        size_t K;
    public:
        Model(int numU, int numI, int K);
        void update(const vector<Pair> & pairs, const double learning_rate, const double reg_user, const double reg_item);
};
double sigmoid(double x);

#endif
