#ifndef _argument_hpp_
#define _argument_hpp_

#include <iostream>
#include <set>
#include <map>
#include <string>
#include <stdexcept>

using namespace std;

class Argument_Pair {
friend class Argument;
private:
	string _value;
	bool has_value;
	set<string> _arg;
	string _description;
public:
	Argument_Pair(const char * default_value = NULL, const string & description = "");
	void value(const char *);
	const char * value() const;
	Argument_Pair & add(const string & );
	friend ostream & operator<<(ostream &, const Argument_Pair &);
};

class Argument {
private:
	map<string, Argument_Pair> bind;
public:
	Argument_Pair & create_index(const string &, const char * = NULL, const string & = "");
	const Argument_Pair & index(const string &) const;
	Argument_Pair & index(const string &);
	void print_help(ostream &) const;
	void print_setting(ostream &) const;
	void parse(int argc, const char * argv[]);
	const char * operator[](const string & key) const {
		return index(key).value();
	}
	const char * value(const string & key) const {
		return index(key).value();
	}
};

#endif
