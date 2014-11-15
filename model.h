#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include "usage.h"
#include "data2.h"

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
        double y_cache(size_t u, size_t i) const;
        void update(const __Data & data, const Parameter & param);
        void update2(const __Data & data, const Parameter & param);
        void update3(const __Data & data, const Parameter & param);
        void update4(const __Data & data, const Parameter & param);
        void update5(const __Data & data, const Parameter & param);;
        friend fstream & operator<<(fstream & c, const Model & model) {
            c << "model_file" << model.Nuser << " " << model.Nitem << " " << model.K << endl;
            c << model.P << endl << model.Q << endl;
            return c;
        }
        void to_rank_matrix(const Test_Data & test, const char * filename);

};
double sigmoid(double x);

#endif
