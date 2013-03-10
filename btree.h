#ifndef __BTREE_H__
#define __BTREE_H__
#include <distmem.h>
#include <stdio.h>

template<typename T, int size>
class BTreeNode;

template<typename T>
struct NodeUnit{
    long next;
    T data;
};

template<typename T, int size>
class BTreeNode 
{
private:
    int usedSize;
    NodeUnit<T> body[size + 1];

public:
    long parent;
    BTreeNode();
    bool isFull(); 
    bool isEmpty();
    int length();
    NodeUnit<T>* get(int i);
    int search(T t);
    int remove(T t);
    int insert(T t); 
    bool isLeaf();
    BTreeNode<T, size>* explode();
    void dump(NodeUnit<T>* start, int len);
    void shrink(int len);
    void print();
    static BTreeNode<T, size>* newNode();
};


template<typename T, int size>
class BTree 
{
private:
    int nodeNum;
    long root;
    long locate(T t);
    void explode(long pos);
    FILE *file;
    BTreeNode<T, size>* newNode();
    void append(BTreeNode<T, size>* node);
    void setRoot(long i);
    long getNextFreeNode();

#define  IOFFSET sizeof(long)
public:
    BTree(FILE *file);
    T* search(T t);
    int insert(T t);
    int remove(T t);
    void print();
    void writeNode(int i, BTreeNode<T,size>* node);
    BTreeNode<T, size>* readNode(int i);
};

#endif
