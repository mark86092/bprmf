#include "argument.h"

void Argument_Pair::value(const char * target_value) {
	if (target_value == NULL) {
		has_value = false;
		_value = "";
	} else {
		has_value = true;
		_value = string(target_value);
	}
}
const char * Argument_Pair::value() const {
	if (has_value == false)
		return NULL;
	else
		return _value.c_str();
}
Argument_Pair::Argument_Pair(const char * default_value, const string & description) {
	value(default_value);
	_description = description;
}
Argument_Pair & Argument_Pair::add(const string & arg) {
	_arg.insert(arg);
	return (*this);
}
ostream & operator<<(ostream & c, const Argument_Pair & arp) {
	bool first = true;
	for (set<string>::iterator it = arp._arg.begin(); it != arp._arg.end(); it++) {
		if (first) {
			first = false;
			c << (*it);
		} else {
			c << ", ";
			c << (*it);
		}
	}
	c << endl;
	if (arp._description != "")
		c << "\t" << arp._description << endl;
	return c;
}


Argument_Pair & Argument::create_index(const string & key, const char * default_value, const string & description) {
	if (bind.count(key) > 0) {
		throw logic_error("The key '" + key + "' exists");
	} else {
		bind.insert( pair<string, Argument_Pair>(key, Argument_Pair(default_value, description)) );
	}
	return bind.at(key);
}
const Argument_Pair & Argument::index(const string & key) const {
	return bind.at(key);
}
Argument_Pair & Argument::index(const string & key) {
	return bind.at(key);
}
void Argument::print_help(ostream & c) const {
	for (map<string, Argument_Pair>::const_iterator it = bind.begin(); it != bind.end(); it++) {
		c << (*it).second;
	}
}
void Argument::print_setting(ostream & c) const {
	for (map<string, Argument_Pair>::const_iterator it = bind.begin(); it != bind.end(); it++) {
		c << (*it).first << ": ";
		if ((*it).second.has_value == true) {
			c << (*it).second.value() << endl;
		} else {
			c << "[None]" << endl;
		}
	}
}
void Argument::parse(int argc, const char * argv[]) {
	for (int i = 1; i < argc; i++) {
		for (map<string, Argument_Pair>::iterator it = bind.begin(); it != bind.end(); it++) {
			Argument_Pair & the_arp = (*it).second;
			if (the_arp._arg.count(argv[i]) > 0) {
				the_arp.value(argv[i+1]);
				//cout << "find " << it->first << " from " << argv[i] << " as " << the_arp.value() << endl;
				i++;
				break;
			}
		}
	}
}
