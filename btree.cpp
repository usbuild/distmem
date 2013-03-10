#include <cstring>
#include <iostream>
#include <distmem.h>
#include <btree.h>
#include <stdio.h>
using std::memmove;
using std::memset;
template<typename T, int size>
BTreeNode<T, size>::BTreeNode():usedSize(0){
    memset(this->body, 0, sizeof(NodeUnit<T>) * (size + 1));
}

template<typename T, int size>
bool BTreeNode<T, size>::isFull() {
    return this->usedSize == size;
}

template<typename T, int size>
bool BTreeNode<T, size>::isEmpty() {
    return this->usedSize == 0;
}

template<typename T, int size>
int BTreeNode<T, size>::insert(T t) {
    if(this->isFull()) return 1;
    int pos = search(t);
    NodeUnit<T> *unit = &body[pos];
    memmove(unit + 1, unit, (usedSize - pos) * sizeof(NodeUnit<T>));
    ++usedSize;
    unit->next = -1;
    memcpy(&unit->data, &t, sizeof(T));
    return 0;
}

template<typename T, int size>
int BTreeNode<T, size>::remove(T t) {
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


template<typename T, int size>
int BTreeNode<T, size>::search(T t) {
    int i = 0;
    for (i = 0; i < usedSize; ++i) {//should use binary search
        NodeUnit<T> *unit = &body[i];
        if(unit->data >= t) {
            break;
        }
    }
    return i;
}

template<typename T, int size>
void BTreeNode<T, size>::dump(NodeUnit<T>* start, int len) {
    memcpy(this->body, start, (len + 1) * sizeof(NodeUnit<T>));
    this->usedSize = len;
}

template<typename T, int size>
BTreeNode<T, size>* BTreeNode<T, size>::explode() {
    int l = this->usedSize / 2 + 1;
    int r = this->usedSize - l;
    BTreeNode<T, size>* right = BTreeNode<T, size>::newNode();
    right->dump(this->body + l, r);
    this->usedSize = l;
    return right;
}

template<typename T, int size>
void BTreeNode<T, size>::shrink(int len) {
    this->usedSize -= len;
}

template<typename T, int size>
int BTreeNode<T, size>::length() {
    return usedSize;
}

template<typename T, int size>
NodeUnit<T>* BTreeNode<T, size>::get(int i) {
    return &body[i];
}

template<typename T, int size>
void BTreeNode<T, size>::print() {
    for (int i = 0; i < this->usedSize; i++) {
        std::cout << this->get(i)->data << "\t";
    }
    std::cout << std::endl;
}


template<typename T, int size>
BTree<T, size>::BTree(FILE *file):file(file), nodeNum(0) { 
    fseek(file, 0, SEEK_END);
    if(ftell(file) == 0) {
        BTreeNode<T, size>* rootNode = BTreeNode<T, size>::newNode();
        rootNode->parent = -1;
        setRoot(getNextFreeNode());
        append(rootNode);
        delete rootNode;
    } else {
        long i = 0;
        fseek(file, 0, SEEK_SET);
        fread(&i, IOFFSET, 1, file);
        setRoot(i);
    }
}

template<typename T, int size>
void BTree<T, size>::writeNode(int i, BTreeNode<T, size>* node) {
    fseek(file, IOFFSET + sizeof(BTreeNode<T, size>) * i, SEEK_SET);
    fwrite(node, sizeof(BTreeNode<T, size>), 1, file);
}

template<typename T, int size>
BTreeNode<T, size>* BTree<T, size>::readNode(int i) {
    BTreeNode<T, size>* node = BTreeNode<T, size>::newNode();
    fseek(file, IOFFSET + sizeof(BTreeNode<T, size>) * i, SEEK_SET);
    fread(node, sizeof(BTreeNode<T, size>), 1, file);
    return node;
}

template<typename T, int size>
BTreeNode<T, size>* BTreeNode<T, size>::newNode() {
    BTreeNode<T, size>* node = (BTreeNode<T, size>*) new BTreeNode<T, size>();

    NodeUnit<T> unit;
    unit.next = -1;
    for (int i = 0; i < size + 1; i++) {
        memcpy(node->body + i, &unit, sizeof(unit));
    }
    return node;
}

template<typename T, int size>
void BTree<T, size>::append(BTreeNode<T, size>* node) {
    writeNode(getNextFreeNode(), node);
    ++nodeNum;
}

template<typename T, int size>
long BTree<T, size>::locate(T t) {
    BTreeNode<T, size>* node;
    long temp = root;
    NodeUnit<T> *nu;
    while(true) {
        node = readNode(temp);
        if(node->isFull()) {
            this->explode(temp);
            temp = node->parent;
            delete node;
            continue;
        }

        nu = node->get(node->search(t));
        if(nu->data == t) {
            delete node;
            return temp;
        } else {
            if(nu->next == -1) {
                delete node;
                return temp;
            }
            temp = nu->next;
            delete node;
        }
    }
}

template<typename T, int size>
void BTree<T, size>::print() {
    BTreeNode<T, size>* node = readNode(this->root);
    node->print();
    for(int i = 0; i <= node->length(); ++i) {
        long next = node->get(i)->next;
        if(next == -1) break;
        BTreeNode<T, size>* t = readNode(next);
        t->print();
    }
}

template<typename T, int size>
T* BTree<T, size>::search(T t) {
    long pos = this->locate(t);
    BTreeNode<T, size>* node = readNode(pos);
    NodeUnit<T>* unit = node->get(node->search(t));
    if(unit->data == t) return &unit->data;
    return NULL;
}

template<typename T, int size>
int BTree<T, size>::insert(T t) {
    long pos = this->locate(t);
    BTreeNode<T, size>* node = readNode(pos);
    NodeUnit<T>* unit = node->get(node->search(t));
    if(unit->data == t) {
        memcpy(&unit->data, &t, sizeof(T));
    } else {
        node->insert(t);
    }
    writeNode(pos, node);
    delete node;
    return 1;
}

template<typename T, int size>
void BTree<T, size>::setRoot(long i) {
    this->root = i;
    fseek(file, 0, SEEK_SET);
    fwrite(&this->root, IOFFSET, 1, file);
}

template<typename T, int size>
long BTree<T, size>::getNextFreeNode() {
    return nodeNum;
}

template<typename T, int size>
void BTree<T, size>::explode(long pos) {

    BTreeNode<T, size>* node = readNode(pos);
    BTreeNode<T, size>* rNode = node->explode();
    NodeUnit<T>* midUnit = node->get(node->length() - 1);
    node->shrink(1);

    if(node->parent == -1) {
        BTreeNode<T, size>* rootNode = BTreeNode<T, size>::newNode();
        setRoot(getNextFreeNode());
        rootNode->parent = -1;
        append(rootNode);
        node->parent = root;
    }
    writeNode(pos, node);
    rNode->parent = node->parent;
    BTreeNode<T, size>* pNode = readNode(node->parent);
    pNode->insert(midUnit->data);
    midUnit = pNode->get(pNode->search(midUnit->data));
    midUnit->next = pos;
    (midUnit + 1)->next = getNextFreeNode();

    append(rNode);
    writeNode(node->parent, pNode);

    delete node;
    delete pNode;
    delete rNode;
}


template<typename T, int size>
int BTree<T, size>::remove(T t) {

}
