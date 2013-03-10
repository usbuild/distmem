#include <btree.cpp>
using namespace std;
int main(int argc, const char *argv[])
{
    BTree<int> b(5);
    for (int i = 1; i < 20; i++) {
        b.insert(i);
    }
    int *c = b.search(22);
    if(c != NULL) cout << "Found it!" << endl;
    else cout << "Not Found!" << endl;

    return 0;
}
