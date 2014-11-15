#ifndef _USAGE_H_
#define _USAGE_H_
#include <iostream>
#include <cstdlib>
#include <cassert>
#include "argument.h"

using namespace std;
class Parameter : public Argument
{
    public:
        int K, seed, mode;
        double learning_rate, reg_user, reg_item;
        int iter;
        double cost;
        int ji_ratio;
        int stdev_type;
        int stdev_output_period;
        int output_period;
        int j_sample_size;
        int final_only;
        int shuffle;
        Parameter() {
			create_index("K", "10", "hidden factor (default 10)").add("-K");
			create_index("seed", "0", "random seed (default 0)").add("-S").add("--seed");
			create_index("mode", "1", "uptate mode (1:origin method, 2: Penalty_method, 3:mixed, 4:BPRMF-s, 5:BPRMF-w)").add("-m").add("--mode");
			create_index("learning_rate", "0.05", "learning rate (default 0.05)").add("-L").add("-l").add("--learning_rate");
			create_index("reg_user", "0.001", "user regularization (defaut 0.001)").add("-u");
			create_index("reg_item", "0.001", "item regularization (defaut 0.001)").add("-i");
			create_index("iter", "100", "iteration (default 100)").add("--iter");
			create_index("train_file", NULL).add("--train");
			create_index("valid_file", NULL).add("--valid");
			create_index("output_file", NULL).add("--output");
			create_index("rank_matrix", NULL).add("--rank_matrix").add("--rank-matrix");
			create_index("stdev_type", "0", "0: all  1: test only (default 0)").add("--stdev_type").add("-stdev_type").add("--stdev-type");
			create_index("stdev_output_period", "100", "(default 100)").add("--stdev_output_period").add("-stdev_output_period").add("--stdev-output-period");
			create_index("output_period", "100", "(default 100)").add("--output_period").add("-output_period").add("--output-period");
			create_index("ji_ratio", "1").add("--ji_ratio").add("-ji_ratio").add("--ji-ratio");
			create_index("j_sample_size", "6").add("--j_sample_size").add("-j_sample_size").add("--j-sample-size");
			create_index("final_only", "1", "caculate the stdev and auc for only final iteration (default 1)").add("--final_only").add("-final_only").add("--final-only");
			
			create_index("shuffle", "1").add("--shuffle").add("-shuffle");
			create_index("cost", "0.001").add("--cost").add("-cost");
        }
        void post_parse() {
			K = atoi(value("K"));
			seed = atoi(value("seed"));
			mode = atoi(value("mode"));
			learning_rate = atof(value("learning_rate"));
			reg_user = atof(value("reg_user"));
			reg_item = atof(value("reg_item"));
			iter = atoi(value("iter"));
			if (iter == -1) iter = 1000000;
			stdev_type = atoi(value("stdev_type"));
			assert(stdev_type == 0 || stdev_type == 1);
			stdev_output_period = atoi(value("stdev_output_period"));
			assert(stdev_output_period > 0);
            output_period = atoi(value("output_period"));
			ji_ratio = atoi(value("ji_ratio"));
			j_sample_size = atoi(value("j_sample_size"));
			assert(j_sample_size > 0);
			final_only = atoi(value("final_only"));
			assert(final_only == 0 || final_only == 1);
			shuffle = atoi(value("shuffle"));
			cost = atof(value("cost"));
			assert(cost >= 0.0);
			if (value("train_file") == NULL)
				throw "需要 --train";
			if (value("valid_file") == NULL)
				throw "需要 --valid";
        }
};

#endif
