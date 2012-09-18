#ifndef __DIST_FILE_H__
#define __DIST_FILE_H__
#include <sys/stat.h>
#include <sys/types.h>
#include "distmem.h"

class Domain
{
public:
    Domain(std::string &str):name(str){
        dir_path = string(DATA_PATH) +  PATH_SEP + name + PATH_SEP;
        readFiles();
    }
    Domain(const char *str):name(str){
        dir_path = string(DATA_PATH) +  PATH_SEP + name + PATH_SEP;
        readFiles();
    }
    virtual ~Domain(){
        fclose(biffs);
        fclose(idxfs);
        fclose(dmdfs);
    }
    void insert(const char *key, const byte *data, const size_t length){
        struct index i;
        i.used = 1;
        bzero(i.key, KEY_LEN);
        strcpy(i.key, key);
        i.length = byte(length % BLOCK_SIZE);
        i.offset = findUnsetBif();
        writeDmd(data, length, i.offset);
        fseek(idxfs, findUnsetIdx() * IDX_SIZE, SEEK_SET);
        fwrite(&i, IDX_SIZE, 1, idxfs);

    }


private:
    FILE *biffs;
    FILE *idxfs;
    FILE *dmdfs;
    string dir_path;
    string name;

    void readFiles() {
        mkdir(dir_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        char *idxpath = strdup((dir_path + name + IDX_EXT).c_str());
        char *bifpath = strdup((dir_path + name + BIF_EXT).c_str());
        char *dmdpath = strdup((dir_path + name + DMD_EXT).c_str());
        if(access(idxpath, F_OK) || access(bifpath, F_OK) || access(dmdpath, F_OK)) {
            biffs = fopen(bifpath, "wb+");
            idxfs = fopen(idxpath, "wb+");
            dmdfs = fopen(dmdpath, "wb+");
        } else {
            biffs = fopen(bifpath, "rb+");
            idxfs = fopen(idxpath, "rb+");
            dmdfs = fopen(dmdpath, "rb+");
        }
        free(idxpath);
        free(bifpath);
        free(dmdpath);
    }
    uint32_t findUnsetIdx() {
        struct index idx;
        int i = 0;
        while(fread(&idx, IDX_SIZE, 1, idxfs) == 1) {
            if(idx.used == 0) { 
                return ftell(idxfs) / IDX_SIZE - 1;
            }
        }
        return ftell(idxfs) / IDX_SIZE;
    }
    uint32_t findUnsetBif() {
        uint32_t indicator;
        while(fread(&indicator, sizeof(indicator), 1, biffs) == 1) {
            if(indicator == 0xffff) {
                return ftell(biffs) / BIF_SIZE - 1;
            }
        }
        return ftell(biffs) / BIF_SIZE;
    }
    void writeDmd(const byte *data, const size_t length, const uint32_t offset) {
        fseek(dmdfs, offset * BLOCK_SIZE, SEEK_SET);
        fwrite(data, length, 1, dmdfs);
        fseek(dmdfs, BLOCK_SIZE - length - 1, SEEK_END);
        byte zero = 0;
        fwrite(&zero, 1, 1, dmdfs);
    }
};
#endif
