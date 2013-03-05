#ifndef __BTREE_H__
#define __BTREE_H__
#include <distmem.h>

template<typename T>
class BTreeNode 
{
private:
    int size;
    BTreeNode<T>* t;
public:
    BTreeNode(int num);
};

class BTree 
{
private:
    int size;
public:
    BTree(int num);
};

#endif
