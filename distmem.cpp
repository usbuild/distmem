#include "distmem.h"
#include <iostream>
#include <map>
using std::cout;
using std::endl;
using std::string;
int main() {
	DataAtom da;
	string s = "Hello World";
	da.set(s.c_str(), s.size());
	da.append("\0", 1);
	cout << da.data << endl;
	return 0;
}
