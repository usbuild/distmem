#include "distmem.h"
#include <map>
#include "dist_file.h"
int main() {
	Domain dm("Hello");

    string str1 = "Hello World";
    string str2 = "nihao shijie";
    dm.insert("str1", (const byte*)(str1.c_str()), str1.length() + 1);
    dm.insert("str2", (const byte*)(str2.c_str()), str2.length() + 1);
    byte *data;
    size_t length;
    dm.find("str1", data, length);
    if(length > 0) {
        puts((char*)data);
    }
	return 0;
}
