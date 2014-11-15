#include <iostream>
#include "data.h"

using namespace std;

int main() {
	Train train;
	train.Read_File("../train_edges.bpr");
	cout << train.Nuser() << train.Nitem() << endl;
}
