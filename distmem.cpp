#include "distmem.h"
#include <iostream>
#include <map>
#include "dist_file.h"
int main() {
	Domain dm("Hello");
	dm.makeFiles();
	return 0;
}
