#ifndef __BTREE_H__
#define __BTREE_H__
#include <distmem.h>

template<typename T>
class BTreeNode;

template<typename T>
struct NodeUnit{
    BTreeNode<T>* next;
    T data;
};

template<typename T>
class BTreeNode 
{
private:
    int size;
    int usedSize;
    BTreeNode<T>* parent;
    NodeUnit<T>* body;
    bool leaf;

public:
    BTreeNode(int num);
    bool isFull(); 
    bool isEmpty();
    bool isOverHalf();
    int length();
    NodeUnit<T>* get(int i);
    int search(T t);
    int remove(T t);
    int insert(T t); 
    bool isLeaf();
    void setLeaf(bool leaf);
};


template<typename T>
class BTree 
{
private:
    int size;
    BTreeNode<T> *root;
    BTreeNode<T>* locate(T t);
public:
    BTree(int num);
    NodeUnit<T>* search(T t);
    int insert(T t);
    int remove(T t);
};

#endif
