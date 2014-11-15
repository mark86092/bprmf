#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdio>
#include <vector>
#include "data.h"
#include "model.h"
#include "predict.h"

using namespace std;

int main(int argc, const char* argv[])
{
    /** preprocessing **/
    srand( 0 );

	const char train_file[] = "../train_edges.bpr";
	const int K = 10;
	const double learning_rate = 0.05;
	const double reg_user = 0.01;
	const double reg_item = 0.01;

    /** data part **/
    Train train;
    train.Read_File(train_file);
    cerr << train.Nuser() << " " << train.Nitem() << endl;

    /** model declaration **/
    Model model(train.Nuser(), train.Nitem(), K);
	
    for (int iter = 0; iter < 100; iter++) {
        cout << iter << endl;
    	vector<Pair> sample = train.draw_sample(1);
        model.update(sample, learning_rate, reg_user, reg_item);
    }

	/* caculate score of u */
	fstream f;
	f.open("../test_nodes.txt", ios::in);
    fstream p; 
    p.open("predict.txt", ios::out);
	size_t u;
	while (f >> u) 
    {
        p << u << ":";
        vector<Pair_i_y> predict = predict_top_N(train, model, u, 30);
        p << predict[0].i;
        for (size_t n = 1; n < 30; n++)
        {
            p << "," << predict[n].i;
        }
        p << endl;
    }
}
