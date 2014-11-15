#ifndef __PAIR_HPP__
#define __PAIR_HPP__

#include <iostream>

using namespace std;

class Pair {
public:
	size_t u;
	size_t i;
	size_t j;
	inline Pair(size_t _u, size_t _i, size_t _j) {u = _u; i = _i; j = _j;}
	Pair() {}
	friend ostream& operator<<(ostream& c, Pair & p) {
        c << "(" << p.u << ", " << p.i << ", " << p.j << ")";
        return c;
	}
};

#endif
