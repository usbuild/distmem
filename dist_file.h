#ifndef __DIST_FILE_H__
#define __DIST_FILE_H__
#include <sys/stat.h>
#include <sys/types.h>
#include "distmem.h"
#define BLOCK_SIZE  512
#define KEY_LEN     256
#define MAX_LEN     1   //to save the last block offset
#define OFFSET      16

class Domain
{
public:
	Domain(std::string &str):name(str){
        dir_path = string(DATA_PATH) +  PATH_SEP + name + PATH_SEP;
        makeFiles();
    }
	Domain(const char *str):name(str){
        dir_path = string(DATA_PATH) +  PATH_SEP + name + PATH_SEP;
        makeFiles();
    }
	virtual ~Domain(){
        fclose(biffs);
        fclose(idxfs);
        fclose(dmdfs);
	}

	
private:
    FILE *biffs;
    FILE *idxfs;
    FILE *dmdfs;
    string dir_path;
    string name;

	void makeFiles() {
        mkdir(dir_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        biffs = fopen((dir_path + name + BIF_EXT).c_str(), "wb");
        idxfs = fopen((dir_path + name + IDX_EXT).c_str(), "wb");
        dmdfs = fopen((dir_path + name + DMD_EXT).c_str(), "wb");
	}
};
#endif
