#ifndef __DATA_HPP__
#define __DATA_HPP__

#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

/* 用來存 (i, yui) 的tuple */
class Pair_i_yui {
public:
	size_t i;
	double y;
	Pair_i_yui(size_t _i, double _y) {
		i = _i; y = _y;
	}
	Pair_i_yui() {}
};

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
};

#endif
