#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdio>
#include <vector>
#include "data.h"
#include "model.h"

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

    /** core part **/
}
