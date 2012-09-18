#ifndef __DISTMEM_H__
#define __DISTMEM_H__
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
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

using std::string;
extern int errno;
typedef unsigned char byte;
struct index{
    byte used;
    char key[KEY_LEN];
    byte length;
    uint32_t   offset;
};
#endif
