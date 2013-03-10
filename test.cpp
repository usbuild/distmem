#include <btree.cpp>
#include <stdio.h>
using namespace std;
int main(int argc, const char *argv[])
{
    FILE *f = fopen("./t.dat", "r+");
    BTree<int, 5> b(f);
    /*
    for (int i = 1; i < 20; i++) {
        b.insert(i);
        b.print();
    }
    */
    b.print();
    int *c = b.search(19);
    if(c != NULL) cout << "Found it!" << endl;
    else cout << "Not Found!" << endl;


    return 0;
}
