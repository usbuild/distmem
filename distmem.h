#ifndef __DISTMEM_H__
#define __DISTMEM_H__
#include <string.h>
#include <string>
#include <stdio.h>
#include <errno.h>
#define DM_NAME     "DistMem"
#define BIF_EXT     ".bif"
#define IDX_EXT     ".idx"
#define DMD_EXT     ".dmd"
#define DATA_PATH   "data"
#define PATH_SEP    "/"
using std::string;
extern int errno;
typedef char byte;

#endif
