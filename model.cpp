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

double Model::y_cache(size_t u, size_t i) const {
    return P.row(u).dot(Q.row(i));
}

void Model::update(const __Data & data, const Parameter & param)
{
    RowVectorXd new_p(K);
    RowVectorXd new_qi(K);
    RowVectorXd new_qj(K);
    vector<Pair> temp; 
    if(param.shuffle == 1){
        temp = data.draw_sample(param.ji_ratio);
    }else{
        temp = data.draw_sample_no_shuffle(param.ji_ratio);
    }
    for (vector<Pair>::iterator it = temp.begin(); it != temp.end(); it++) {
        size_t u = it->u;
        size_t i = it->i;
        size_t j = it->j;

        double now_i = P.row(u).dot(Q.row(i));
        double now_j = P.row(u).dot(Q.row(j));
        double err = now_i - now_j;
        double sig = sigmoid(err);

        new_p = - param.learning_rate * (-sig * (Q.row(i) - Q.row(j)) + param.reg_user * P.row(u));
        new_qi = - param.learning_rate * (-sig * P.row(u) + param.reg_item * Q.row(i));
        new_qj = - param.learning_rate * (-sig * (-P.row(u)) + param.reg_item * Q.row(j));

        P.row(u) += new_p;
        Q.row(i) += new_qi;
        Q.row(j) += new_qj;
    }
}

void Model::update2(const __Data & data, const Parameter & param) {
    RowVectorXd new_p(K);
    RowVectorXd new_qi(K);
    RowVectorXd new_qj(K);
    vector<Pair> temp; 
    if(param.shuffle == 1){
        temp = data.draw_sample(param.ji_ratio);
    }else{
        temp = data.draw_sample_no_shuffle(param.ji_ratio);
    }
    for (vector<Pair>::iterator it = temp.begin(); it != temp.end(); it++) {
        size_t u = it->u;
        size_t i = it->i;
        size_t j = it->j;

        double now_i = P.row(u).dot(Q.row(i));
        double now_j = P.row(u).dot(Q.row(j));
        double err = now_i - now_j;
        double sig = sigmoid(err);

        /** 自訂變數 **/
        size_t sample_size_loan = param.j_sample_size;
        size_t sample_size_lender = (sample_size_loan * Nuser) / Nitem ;
        if(sample_size_lender == 0) { sample_size_lender = 1;};
        double average = 0;
        double cost = param.cost / Nuser / Nitem;

        double sample_lender_ratio = (double) Nuser / sample_size_lender;
        double sample_loan_ratio = (double) Nitem / sample_size_loan;

        /** gradient of Q **/
        RowVectorXd blue;
        double green_i = 0.;
        double green_j = 0.;
        RowVectorXd P_partial_sum = RowVectorXd::Zero(K);
        {
            for(size_t su = 0; su < sample_size_lender; su++)
            {
                size_t temp_lender = rand() % Nuser;
                green_i += P.row(temp_lender).dot(Q.row(i));
                green_j += P.row(temp_lender).dot(Q.row(j));
                P_partial_sum += P.row(temp_lender);
            }
        }
        green_i = green_i * sample_lender_ratio - average;
        green_j = green_j * sample_lender_ratio - average;
        blue = P_partial_sum * sample_lender_ratio;

        /** gradient of P **/
        RowVectorXd red = RowVectorXd::Zero(K);
        for(size_t si = 0 ; si < sample_size_loan; si++)
        {
            size_t temp_loan = rand() % Nitem;
            double temp5 = 0.;
            for(size_t su = 0; su < sample_size_lender; su++)
            {
                size_t temp_lender = ( rand() % Nuser);
                temp5 += P.row(temp_lender).dot(Q.row(temp_loan));
            }
            red += (temp5 * sample_lender_ratio - average) * Q.row(temp_loan) * sample_loan_ratio;
        }
        new_p = - param.learning_rate * ( -sig * (Q.row(i) - Q.row(j)) + param.reg_user * P.row(u) + cost * red );
        new_qi = - param.learning_rate * ( -sig * P.row(u) + param.reg_item * Q.row(i) + cost * green_i * blue);
        new_qj = - param.learning_rate * ( -sig * (-P.row(u)) + param.reg_item * Q.row(j) + cost * green_j * blue);

        P.row(u) += new_p;
        Q.row(i) += new_qi;
        Q.row(j) += new_qj;
    }
}

void Model::update3(const __Data & data, const Parameter & param){
    RowVectorXd new_p(K);
    RowVectorXd new_qi(K);
    RowVectorXd new_qj(K);
    vector<Pair> temp; 
    if(param.shuffle == 1){
        temp = data.draw_sample(param.ji_ratio);
    }else{
        temp = data.draw_sample_no_shuffle(param.ji_ratio);
    }
    for (vector<Pair>::iterator it = temp.begin(); it != temp.end(); it++) {
        size_t u = it->u;
        size_t i = it->i;
        size_t j = it->j;

        double now_i = P.row(u).dot(Q.row(i));
        double now_j = P.row(u).dot(Q.row(j));
        double err = now_i - now_j;
        double sig = sigmoid(err);

        size_t sample_size = param.j_sample_size;
        int win = 0;
        for(size_t iter = 0; iter < sample_size; iter++){
            size_t temp_u = rand() % Nuser;
            double temp_i = P.row(temp_u).dot(Q.row(i));
            double temp_j = P.row(temp_u).dot(Q.row(j));
            if(temp_i > temp_j){
                win = win + 1;
            } 
        }
        if ((size_t)win < (sample_size / 2)) {
            continue;
        }
        double weight = 2. * (sample_size - win) / sample_size;
        sig *= weight;

        new_p = - param.learning_rate * (-sig * (Q.row(i) - Q.row(j)) + param.reg_user * P.row(u));
        new_qi = - param.learning_rate * (-sig * P.row(u) + param.reg_item * Q.row(i));
        new_qj = - param.learning_rate * (-sig * (-P.row(u)) + param.reg_item * Q.row(j));

        P.row(u) += new_p;
        Q.row(i) += new_qi;
        Q.row(j) += new_qj;
    }
}





void Model::update4(const __Data & data, const Parameter & param) {
    RowVectorXd new_p(K);
    RowVectorXd new_qi(K);
    RowVectorXd new_qj(K);
    vector<Pair> temp; 
    if(param.shuffle == 1){
        temp = data.draw_sample(param.ji_ratio);
    }else{
        temp = data.draw_sample_no_shuffle(param.ji_ratio);
    }
    for (vector<Pair>::iterator it = temp.begin(); it != temp.end(); it++) {
        size_t u = it->u;
        size_t i = it->i;
        size_t j = it->j;

        double now_i = P.row(u).dot(Q.row(i));
        double now_j = P.row(u).dot(Q.row(j));
        double err = now_i - now_j;
        double sig = sigmoid(err);

        /*** sample and compare ***/
        size_t sample_size = param.j_sample_size;
        int win = 0;
        for(size_t s = 0; s < sample_size; s++){
            size_t temp_lender = rand() % Nuser;
            double temp_i = P.row(temp_lender).dot(Q.row(i));
            double temp_j = P.row(temp_lender).dot(Q.row(j));

            if(temp_i > temp_j){
                win = win + 1;
            } 
        }
        double weight = 2. * (sample_size - win) / sample_size;
        sig *= weight;

        new_p = - param.learning_rate * (-sig * (Q.row(i) - Q.row(j)) + param.reg_user * P.row(u));
        new_qi = - param.learning_rate * (-sig * P.row(u) + param.reg_item * Q.row(i));
        new_qj = - param.learning_rate * (-sig * (-P.row(u)) + param.reg_item * Q.row(j));

        P.row(u) += new_p;
        Q.row(i) += new_qi;
        Q.row(j) += new_qj;
    }
}

void Model::update5(const __Data & data, const Parameter & param){
    RowVectorXd new_p(K);
    RowVectorXd new_qi(K);
    RowVectorXd new_qj(K);
    vector<Pair> temp; 

    for (size_t u = 0; u < Nuser; u++) {
        unordered_set<size_t> & i_set = data.i_set[u];
        for (unordered_set<size_t>::iterator it = i_set.begin(); it != i_set.end(); it++) {
            bool j_get = false;
            size_t i = *it;
            size_t j;
            while (j_get == false) { // Get a GOOD j
                j = rand() % Nitem;
                if (i_set.find(j) == i_set.end()) { // test whether j is in i_set
                    size_t sample_size = param.j_sample_size;
                    int win = 0;
                    for(size_t iter = 0; iter < sample_size; iter++){
                        size_t temp_u = rand() % Nuser;
                        double temp_i = P.row(temp_u).dot(Q.row(i));
                        double temp_j = P.row(temp_u).dot(Q.row(j));
                        if(temp_i > temp_j){
                            win = win + 1;
                        } 
                    }
                    if ((size_t)win >= (sample_size / 2)) {
                        j_get = true;
                    }
                }
            }

            /** u, i, j(good)  **/
            double now_i = P.row(u).dot(Q.row(i));
            double now_j = P.row(u).dot(Q.row(j));
            double err = now_i - now_j;
            double sig = sigmoid(err);

            new_p = - param.learning_rate * (-sig * (Q.row(i) - Q.row(j)) + param.reg_user * P.row(u));
            new_qi = - param.learning_rate * (-sig * P.row(u) + param.reg_item * Q.row(i));
            new_qj = - param.learning_rate * (-sig * (-P.row(u)) + param.reg_item * Q.row(j));

            P.row(u) += new_p;
            Q.row(i) += new_qi;
            Q.row(j) += new_qj;

        }
    }
}

void Model::to_rank_matrix(const Test_Data & test, const char * filename) {
	fstream file;
	file.open(filename, ios::out);
	
	file << test.u_set.size() << " " << test.Nitem() << endl;
	
	for (auto it = test.u_set.begin(); it != test.u_set.end(); it++) {
		file << *it << " ";
		for (size_t i = 0; i < test.Nitem(); i++) {
			file << P.row(*it).dot(Q.row(i)) << " ";
		}
		file << endl;
	}
	
	file.close();
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
