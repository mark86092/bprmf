#ifndef __DATA_HPP__
#define __DATA_HPP__

#include <iostream>
#include <vector>
#include <unordered_set>
#include "pair.h"

using namespace std;

class Train {
protected:
	size_t _Nuser; 
	size_t _Nitem;
public:
	Train();
	~Train();
    
	/** 其實就是 Iu+ 這個集合 **/
    unordered_set<size_t> * i_set;
	
	inline size_t Nuser() const {return _Nuser;}
	inline size_t Nitem() const {return _Nitem;}
	void Read_File(const char * filename);
	vector<Pair> draw_sample(const int ji_ratio) const;
};

#endif
