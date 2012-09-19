#include "distmem.h"
#include <map>
#include "dist_file.h"
int main() {
	Domain dm("Hello");
    string hello = "I'm a very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very long string";
    dm.insert("Hello", (const byte*)(hello.c_str()), hello.length() + 1);
    byte *data;
    size_t length;
    dm.find("Hello", data, length);
    printf("Data: %s\n", data);
    printf("Length: %u\n", length);
	return 0;
}
