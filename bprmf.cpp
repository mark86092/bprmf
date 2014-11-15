#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdio>
#include <vector>
#include "data.h"
#include "model.h"

using namespace std;

bool sort_by_y(const Pair_i_yui &i, const Pair_i_yui &j) {
	return (i.y > j.y);
}

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
	
	//vector<Pair> sample = train.draw_sample(1);

	/* caculate score of u */
	fstream f;
	f.open("../test_nodes.txt", ios::in);
	size_t u;
	f >> u;

	vector<Pair_i_yui> predict;
	for (size_t i = 0; i < train.Nitem(); i++) {
		if (i == u)
			continue;
		if (train.i_set[u].find(i) != train.i_set[u].end())
			continue;
		double y = model.P.row(u).dot(model.Q.row(i));
		predict.push_back(Pair_i_yui(i, y));
	}
	
	sort(predict.begin(), predict.end(), sort_by_y);
    for (size_t i = 0; i < 30; i++) {
		cout << predict[i].i << " " << predict[i].y << endl;
	}
	/** core part **/
}
