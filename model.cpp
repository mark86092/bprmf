#include <iostream>
#include "model.h"

using namespace Eigen;

Model::Model(int numU, int numI, int K) {
    Nuser = numU;
    Nitem = numI;
    this->K = K;
    P = MatrixXd::Random(Nuser, K) * 0.01;
    Q = MatrixXd::Random(Nitem, K) * 0.01;
}

void Model::update(const vector<Pair> & pairs, const double learning_rate, const double reg_user, const double reg_item)
{
    RowVectorXd new_p(K);
    RowVectorXd new_qi(K);
    RowVectorXd new_qj(K);
    for (auto it = pairs.begin(); it != pairs.end(); it++) {
        size_t u = it->u;
        size_t i = it->i;
        size_t j = it->j;

        double now_i = P.row(u).dot(Q.row(i));
        double now_j = P.row(u).dot(Q.row(j));
        double err = now_i - now_j;
        double sig = sigmoid(err);

        new_p = - learning_rate * (-sig * (Q.row(i) - Q.row(j)) + reg_user * P.row(u));
        new_qi = - learning_rate * (-sig * P.row(u) + reg_item * Q.row(i));
        new_qj = - learning_rate * (-sig * (-P.row(u)) + reg_item * Q.row(j));

        P.row(u) += new_p;
        Q.row(i) += new_qi;
        Q.row(j) += new_qj;
    }
}

double sigmoid(double x) {
    double sig = exp(-x) / (1.0 + exp(-x));
    if(std::isnan(sig)) {
        if(x < 0.0)
            return 1.0;
        else
            return 0.0;
    }
    return sig;
}