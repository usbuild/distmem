#include <btree.cpp>
#include <stdio.h>
using namespace std;
int main(int argc, const char *argv[])
{
    FILE *f = fopen("./t.dat", "w+");
    BTree<int, 5> b(f);
    for (int i = 1; i < 5; i++) {
        b.insert(i);
        b.print();
    }
    /*
    int *c = b.search(22);
    if(c != NULL) cout << "Found it!" << endl;
    else cout << "Not Found!" << endl;
    */

    return 0;
}
