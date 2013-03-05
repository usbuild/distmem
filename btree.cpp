#include <distmem.h>
#include <btree.h>

template<typename T>
BTreeNode<T>::BTreeNode(int num):size(num) {
    this->t = reinterpret_cast<BTreeNode<T>*>(new char[sizeof(T) * size + sizeof(BTreeNode**) * (size + 1)]); 
}


BTree::BTree(int num):size(num) {}



int main(int argc, const char *argv[])
{
    BTreeNode<int>* node = new BTreeNode<int>(1000);
    return 0;
}
