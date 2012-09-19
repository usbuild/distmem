#include "distmem.h"
#include <map>
#include "dist_file.h"
int main() {
	Domain dm("Hello");
    /*
    string hello = "This is Hello String";
    dm.insert("Hello", (const byte*)(hello.c_str()), hello.length() + 1);
    */
    byte *data;
    size_t length;
    dm.find("Hello", data, length);
    printf("%u\n", length);
    printf("%s\n", data);
	return 0;
}
