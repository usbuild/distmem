#include <btree.cpp>
using namespace std;
int main(int argc, const char *argv[])
{
    BTree<int> b(5);
    for (int i = 0; i < 100; i++) {
        b.insert(i);
    }
    return 0;
}
