#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdio>
#include "usage.h"
#include "data2.h"
#include "statics.h"
#include "model.h"
#include <vector>

using namespace std;

struct t_record {
	int iter;
	double test_auc;
	t_record(int it) {iter = it;}
};
ostream & operator<<(ostream & c, const t_record & r) {
	c << r.iter << "," << r.test_auc;
	return c;
}
vector<t_record> Records;

int main(int argc, const char* argv[])
{
    /** preprocessing **/
    Parameter param;
    try {
        param.parse(argc, argv);
        param.post_parse();
    } catch (const char * message) {
        cerr << message << endl;
        param.print_help(cerr);
        exit(1);
    }

    srand( param.seed );
    param.print_setting(cerr);

    /** data part **/
    DataSet data;
    data.Read_File(param["train_file"], param["valid_file"]);
    cerr << data.Nuser() << " " << data.Nitem() << endl;

    /** model declaration **/
    Model model(data.Nuser(), data.Nitem(), param.K);

    /** core part **/
    
    t_record best_record(-1);
    best_record.test_auc = 0.;
    
    cout << "iter,test_auc,best_iter,best_test_auc" << endl;
    for(int iter = 0; iter < param.iter; iter++) {
    	t_record record(iter);
        //cerr << "#" << iter;
        
        switch (param.mode) {
            case 1:
                model.update(data.train, param);    //original
                break;
            case 2:
                model.update2(data.train, param);   //penalty
                break;
            case 3:
                model.update3(data.train, param);   //mix of 4 and 5: block the risky update and change the weight of remaining update
                break;
            case 4:
                model.update4(data.train, param);   //change the weight of each update  (BPRMF-w in our paper)
                break;
            case 5:
                model.update5(data.train, param);   //block some risky update that might cause unfair condition(BPRMF-s in our paper) 
                break;
            default:
                cerr << "error in mode(there are only mode in 1~4)" << endl;
                exit(1);
        }
        
        record.test_auc = calculate_auc_by_draw_sample(data.test, model, 10);
        	
    	if (record.test_auc > best_record.test_auc) {
    		best_record = record;
    	} else {
    		param.iter = best_record.iter + 200;
    	}
    	
    	cout << record << "," << best_record << endl;
    	
        /*if(iter % param.output_period == 0 || iter == (param.iter - 1)){
        	
            char filename[1000];
            sprintf(filename, "%s_iter_%d.rank", param["output_file"], iter);

            model.to_rank_matrix(data.test, filename);
            fstream f;
            f.open((string(filename) + ".done").c_str(), ios::out);
            f.close();
        }*/
    }
    
    
    
    
}
