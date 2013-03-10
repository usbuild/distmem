#ifndef __DISTMEM_H__
#define __DISTMEM_H__
#include <string.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <vector>
#define DM_NAME     "DistMem"
#define BIF_EXT     ".bif"
#define IDX_EXT     ".idx"
#define DMD_EXT     ".dmd"
#define DATA_PATH   "data"
#define PATH_SEP    "/"
#define BLOCK_SIZE  256
#define KEY_LEN     256
#define MAX_LEN     1   //to save the last block offset
#define OFFSET      4
#define IDX_SIZE    sizeof(struct index)
#define BIF_SIZE    sizeof(uint32_t)
#define BIF_END     0xfffffffe
#define BIF_NOT_USE 0xffffffff
#define IDX_NOT_FOUND 0xffffffff
#define DEFAULT_PORT    4327
#define ARRAY_SEP   ',' 

using std::string;
using std::vector;
extern int errno;
typedef unsigned char byte;
class index{
public:
    byte used;
    char key[KEY_LEN];
    uint32_t   length;
    uint32_t   offset;
    bool operator >= (index &i) {
        return strcmp(key, i.key) >= 0;
    }

    bool operator == (index &i) {
        return strcmp(key, i.key) == 0;
    }
};
#endif
