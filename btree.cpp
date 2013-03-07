#include <cstring>
#include <distmem.h>
#include <btree.h>
using std::memmove;
using std::memset;
template<typename T>
BTreeNode<T>::BTreeNode(int num):size(num), usedSize(0), leaf(true) {
    this->body = new NodeUnit<T>[size + 1]; 
    memset(this->body, 0, sizeof(NodeUnit<T>) * (size + 1));
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
    memcpy(&unit->data, &t, sizeof(T));
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
bool BTreeNode<T>::isLeaf() {
    return this->leaf;
}

template<typename T>
void BTreeNode<T>::setLeaf(bool leaf) {
    this->leaf = leaf;
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

template<typename T>
BTreeNode<T>* BTree<T>::locate(T t) {
    BTreeNode<T>* node = this->root;
    NodeUnit<T> *nu;
    while(node != NULL) {
        nu = node->search(t);
        if(nu->data == t) {
            return node;
        } else {
            node = node->next;
        }
    }
    return node;
}

template<typename T>
NodeUnit<T>* BTree<T>::search(T t) {
    BTreeNode<T>* node = this->locate(t);
    NodeUnit<T>* unit = node->get(node->search(t));
    if(unit->data == t) return unit;
    return NULL;
}

template<typename T>
int BTree<T>::insert(T t) {
    BTreeNode<T>* node = this->locate(t);
    NodeUnit<T>* unit = node->search(t);
    if(unit->data == t) {
        memcpy(&unit->data, &t, sizeof(T));
    } else {
        unit->next = new BTreeNode<T>(size);
        unit->next->insert(t);
    }
}

template<typename T>
int BTree<T>::remove(T t) {

}

using std::cout;
using std::endl;
int main(int argc, const char *argv[])
{
    BTree<int> b(100);
    return 0;
}
