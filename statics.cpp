#include <iostream>
#include <fstream>
#include <algorithm>
#include <omp.h>
#include <map>
#include "statics.h"

using namespace std;

bool myfunction(const Pair_i_yui &i, const Pair_i_yui &j) {
    return (i.y > j.y);
}

bool sort_by_y(const Pair_i_yui &i, const Pair_i_yui &j) {
    return (i.y > j.y);
}

double calculate_auc(size_t u, vector<Pair_i_yui> & doc, const unordered_set<size_t> & i_set, const Model & model) {
    size_t true_num = i_set.size();
    size_t doc_num = doc.size();
    size_t false_num = doc_num - true_num;

    if (true_num == 0) {
        return -1.;
    }
    if (false_num == 0) {
        return 1.;
    }
    for (size_t t = 0; t < doc_num; t++) {
        doc[t].y = model.y_cache(u, doc[t].i);
    }
    sort(doc.begin(), doc.end(), sort_by_y);

    double tpr = 0.0;
    double tp_i = 1.0 / true_num;
    double sum_auc = 0.0;

    for (size_t t = 0; t < doc_num; t++) {
        if (i_set.find(doc[t].i) != i_set.end()) {
            tpr += tp_i;
        } else {
            sum_auc += tpr;
        }
    }
    sum_auc /= false_num;
    return sum_auc;
}

double calculate_auc(const Test_Data & data, const Model & model) {
    size_t effective_Nuser = 0;
    double sum_all_auc = 0.;
    #pragma omp parallel for reduction( +:effective_Nuser) reduction( +:sum_all_auc) num_threads(8)
    for (size_t u = 0; u < data.Nuser(); u++) {
    
        double sum_auc = 0.;    
        vector<Pair_i_yui> doc; // Items considered to be caculated.
        
        for (size_t i = 0; i < data.Nitem(); i++){
        	bool isOK = false;
        	if (data.i_set[u].find(i) != data.i_set[u].end()) {
        		isOK = true;
        	} else {
        		if (data.i2_set[u].find(i) == data.i2_set[u].end()) {
        			isOK = true;
        		}
        	}
        	
            if(isOK) { 
                double y = model.y_cache(u, i);
                doc.push_back(Pair_i_yui(i, y));
            }
        }
        sum_auc = calculate_auc(u, doc, data.i_set[u], model);

        if (sum_auc < 0.)
            continue;
        effective_Nuser++;
        sum_all_auc += sum_auc;
    }
    sum_all_auc /= effective_Nuser;
    return sum_all_auc;
}

double calculate_auc_by_draw_sample(__Data & data, const Model & model, int ji_ratio) {

    size_t effective_Nuser = 0;
    double sum_all_auc = 0.;

    for (size_t u = 0; u < data.Nuser(); u++) {
        vector<Pair> temp = data.draw_sample(u, ji_ratio);
        if (temp.size() == 0)
            continue;
        int correct = 0;
        for (vector<Pair>::iterator it = temp.begin() ; it != temp.end(); it++) {
            if (model.y_cache(u, it->i) >= model.y_cache(u, it->j))
                correct++;
        }
        effective_Nuser++;
        sum_all_auc += (double)correct / (double)temp.size();
    }
    return sum_all_auc / effective_Nuser;
}

/* item_set: all item;
rank_result;
top_n;
filename;
 */
double calc_stdev(unordered_set<size_t> & item_set, vector< vector<size_t> > & rank_result, int top_n, const char * filename){
    assert(item_set.size() > 0);
    if (top_n == -1)
        top_n = INT_INF;

    fstream file;
    if (filename != NULL) {
        file.open(filename, ios::out);
    }

    // build map for saving the numbers of each item
    map<size_t, size_t> number;

    for(unordered_set<size_t>::iterator it = item_set.begin(); it != item_set.end(); ++it){
        number[*it] = 0;
    }
    // output to file: the number of items
    file << number.size() << endl;

    // calc each number of each item
    for(vector< vector<size_t> >::iterator it1 = rank_result.begin(); it1 != rank_result.end(); ++it1){
        int cnt = 0;
        for(vector<size_t>::iterator it2 = it1->begin(); it2 != it1->end(); ++it2){
            if(cnt >= top_n)
                break;
            assert(number.find(*it2) != number.end());
            number[*it2] = number[*it2] + 1;

            // output to file
            if (filename != NULL)
                file << *it2 << " ";
            cnt++;
        }

        // output to file
        if (filename != NULL)
            file << endl;
    }
    file.close();

    //calc stdev
    double mean = 0.0;
    double stdev = 0.0;
    for(map<size_t,size_t>::iterator it = number.begin(); it != number.end(); ++it){
        mean = mean + double(it->second);
        //cout << " " << it->second ;
    }
    //cout << endl;
    mean = mean / number.size();
    for(map<size_t,size_t>::iterator it = number.begin(); it != number.end(); ++it){
        stdev = stdev + (double(it->second) - mean) * (double(it->second) - mean);
    }
    stdev = stdev / number.size();
    
    return sqrt(stdev);
}

double stdev_actual(DataSet & data, const Model & model, int top_n, const char * filename){
    vector< vector<size_t> > rank_result;

    // all the items in testing set
    unordered_set<size_t> I_set;
    for(size_t i = 0; i < data.test.Nitem(); i++){
        I_set.insert(i);
    }

    // test the lenders only in test set & the loans only in test set
    for (unordered_set<size_t>::iterator u_it = data.test.u_set.begin(); u_it != data.test.u_set.end(); ++u_it){
        size_t u = *u_it;

        vector<Pair_i_yui> doc;
        for(size_t i = 0; i < data.test.Nitem(); i++){
            if(data.train.i_set[u].find(i) == data.train.i_set[u].end()){
                double y = model.y_cache(u, i);
                doc.push_back(Pair_i_yui(i, y));
            }
        }
        sort(doc.begin(), doc.end(), myfunction);
        vector<size_t> rank;
        size_t cnt = 0;
        for(vector<Pair_i_yui>::iterator d_it = doc.begin(); d_it != doc.end(); ++d_it){
            if(cnt >= data.test.i_set[u].size())
                break;
            //cout << "i: " << d_it->i << "  yui:" << d_it->y << endl;
            rank.push_back(d_it->i);
            cnt = cnt + 1;
        }
        //cout << endl;
        rank_result.push_back(rank);
    }
    return calc_stdev(I_set, rank_result, top_n, filename);
}


// stdev_type:0  all training & testing items
//            1  all testing items
double *stdev_topn(DataSet & data, const Model & model, int* top_n, int top_num, int stdev_type, const char * filename){
    int max_top_n = -2;
    for(int i = 0; i < top_num; i++){
        if(max_top_n < top_n[i]){
            max_top_n = top_n[i];
        }
    }
    double *stdev = new double[top_num];

    vector< vector<size_t> > rank_result;
    unordered_set<size_t> I_set;
    if(stdev_type == 0){ // all training & testing items
        for(size_t i = 0; i < data.train.Nitem(); i++){
            I_set.insert(i);
        }
    }
    else{  // all testing items
        for(size_t i = 0; i < data.test.Nitem(); i++){
            I_set.insert(i);
        }
    }

    /** 測試所有的 User **/
    for (size_t u = 0; u < data.Nuser(); u++){
        vector<Pair_i_yui> doc;
        if(stdev_type == 0){
            for(size_t i = 0; i < data.train.Nitem(); i++){
                if(data.train.i_set[u].find(i) == data.train.i_set[u].end()){  //to avoid occurence in training set
                    double y = model.y_cache(u, i);
                    doc.push_back(Pair_i_yui(i, y));
                }
            }
        }
        else{
            for(size_t i = 0; i < data.test.Nitem(); i++){
                if(data.train.i_set[u].find(i) == data.train.i_set[u].end()){
                    double y = model.y_cache(u, i);
                    doc.push_back(Pair_i_yui(i, y));
                }
            }
        }
        sort(doc.begin(), doc.end(), myfunction);
        vector<size_t> rank;
        int cnt = 0;
        for(vector<Pair_i_yui>::iterator d_it = doc.begin(); d_it != doc.end(); ++d_it){
            if(cnt >= max_top_n)
                break;
            rank.push_back(d_it->i);
            cnt = cnt + 1;
        }
        rank_result.push_back(rank);
    }

    for(int i = 0; i < top_num; i++){
        if (top_n[i] == max_top_n)
            stdev[i] = calc_stdev(I_set, rank_result, top_n[i], filename);
        else
            stdev[i] = calc_stdev(I_set, rank_result, top_n[i], NULL);
    }
    return stdev;
}
