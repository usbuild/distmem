#include "distmem.h"
#include <map>
#include "dist_file.h"
int main() {
	Domain dm("Hello");
    string hello = "This is Hello String";
    dm.insert("Hello", (const byte*)(hello.c_str()), hello.length());
	return 0;
}
