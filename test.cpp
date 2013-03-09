#include <btree.cpp>
using namespace std;
int main(int argc, const char *argv[])
{
    BTree<int> b(5);
    for (int i = 1; i < 10; i++) {
        b.insert(i);
        cout << "================" << endl;
        b.print();
    }
    return 0;
}
