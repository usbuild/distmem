#include <cstring>
#include <distmem.h>
#include <btree.h>
using std::memmove;
template<typename T>
BTreeNode<T>::BTreeNode(int num):size(num), usedSize(0) {
    this->body = new NodeUnit<T>[size + 1]; 
}

template<typename T>
bool BTreeNode<T>::isFull() {
    return this->usedSize == size;
}

template<typename T>
bool BTreeNode<T>::isEmpty() {
    return this->usedSize == 0;
}

template<typename T>
bool BTreeNode<T>::isOverHalf() {
    return this->usedSize > (this->size / 2);
}

template<typename T>
int BTreeNode<T>::insert(T t) {
    if(this->isFull()) return 1;
    int pos = search(t);
    NodeUnit<T> *unit = &body[pos];
    memmove(unit + 1, unit, (usedSize - pos) * sizeof(NodeUnit<T>));
    ++usedSize;
    unit->next = NULL;
    unit->data = t;
    return 0;
}
template<typename T>
int BTreeNode<T>::remove(T t) {
    if(this->isEmpty()) return 1;
    int pos = search(t);
    NodeUnit<T> *unit = &body[pos];
    if(unit->data == t) {
        --usedSize;
        memmove(unit, unit + 1, (usedSize - pos) * sizeof(NodeUnit<T>));
        return 0;
    }
    return 1;
}


template<typename T>
int BTreeNode<T>::search(T t) {
    int i = 0;
    for (i = 0; i < usedSize; ++i) {//should use binary search
        NodeUnit<T> *unit = &body[i];
        if(unit->data >= t) {
            break;
        }
    }
    return i;
}


template<typename T>
int BTreeNode<T>::length() {
    return usedSize;
}

template<typename T>
NodeUnit<T>* BTreeNode<T>::get(int i) {
    return &body[i];
}

template<typename T>
BTree<T>::BTree(int num):size(num) { 
    root = new BTreeNode<T>(size);
}

using std::cout;
using std::endl;
int main(int argc, const char *argv[])
{
    BTree<int> b(100);
    return 0;
}
