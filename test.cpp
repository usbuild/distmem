#include <btree.hpp>
#include <stdio.h>
using namespace std;
class A 
{
public:
    int a;
    long b;

    bool operator == (A &b) {
        return a == b.a;
    }

    bool operator >= (A &b) {
        return a >= b.a;
    }
};

int main(int argc, const char *argv[])
{
    FILE *f = fopen("./t.dat", "r+");
    BTree<A, 1000> b(f);

    /*
    for (int i = 1; i < 100000; i++) {
        A a;
        a.a = i;
        a.b = 2 * i;
        b.insert(a);
    }
    */

    A a;
    a.a = 100;

    A *c = b.search(a);
    if(c != NULL) cout << c->b << endl;
    else cout << "Not Found!" << endl;


    return 0;
}
