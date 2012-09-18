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
        struct index idx;
        idx.used = 1;
        bzero(idx.key, KEY_LEN);
        strcpy(idx.key, key);
        idx.length = byte(length % BLOCK_SIZE);

        fseek(idxfs, findUnsetIdx() * IDX_SIZE, SEEK_SET);
        fwrite(&idx, IDX_SIZE, 1, idxfs);
        
        int spice_count = length / BLOCK_SIZE + 1;
        int i = 0;
        int final = BIF_END;
        vector<uint32_t> v;
        for(i = 0; i < spice_count; i++) {
            v.push_back(findUnsetBif());
        }
        for(i = 0; i < spice_count; i++) {
            fseek(biffs, BIF_SIZE * v[i], SEEK_SET);
            if(i == spice_count - 1) {
                fwrite(&final, BIF_SIZE, 1, biffs);
            } else {
                fwrite(&v[i + 1], BIF_SIZE, 1, biffs);
            }
        }
        idx.offset = v[0];
        writeDmd(data, length, idx.offset);
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
        fseek(idxfs, 0, SEEK_SET);
        while(fread(&idx, IDX_SIZE, 1, idxfs) == 1) {
            if(idx.used == 0) { 
                return ftell(idxfs) / IDX_SIZE - 1;
            }
        }
        return ftell(idxfs) / IDX_SIZE;
    }
    uint32_t findUnsetBif() {
        uint32_t indicator;
        uint32_t final = BIF_END;
        fseek(biffs, 0, SEEK_SET);
        while(fread(&indicator, BIF_SIZE, 1, biffs) == 1) {
            if(indicator == BIF_NOT_USE) {
                fseek(biffs, BIF_SIZE * -1, SEEK_CUR);
                fwrite(&final, BIF_SIZE, 1, biffs);
                return ftell(biffs) / BIF_SIZE - 1;
            }
        }
        fwrite(&final, BIF_SIZE, 1, biffs);
        fflush(biffs);
        return ftell(biffs) / BIF_SIZE - 1;
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
