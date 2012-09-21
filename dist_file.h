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
    void find(const char *key, byte* &data, size_t &length) {
        struct index idx;
        uint32_t idx_offset = locate(key);
        if(IDX_NOT_FOUND == idx_offset) {
            data = NULL;
            length = 0;
        }
        fseek(idxfs, IDX_SIZE * idx_offset, SEEK_SET);
        fread(&idx, IDX_SIZE, 1, idxfs);
        data = (byte*) malloc(sizeof(byte) * idx.length);
        length = idx.length;
        uint32_t offset =  idx.offset;

        int write_len = 0;
        while(1) {
            if(readBif(offset) == BIF_END) {
                readDmd(data + write_len, length % BLOCK_SIZE, offset);
                write_len += length % BLOCK_SIZE;
                break;
            } else  {
                readDmd(data + write_len, BLOCK_SIZE, offset);
                write_len += BLOCK_SIZE;
            }
            offset = readBif(offset);
        }
        length = idx.length;
    }

    uint32_t locate(const char *key) {
        struct index idx;
        fseek(idxfs, 0, SEEK_SET);
        while(fread(&idx, IDX_SIZE, 1, idxfs) == 1) {
            if(idx.used == 1) {
                if(strcmp(idx.key, key) == 0) {
                    return ftell(idxfs) / IDX_SIZE - 1;
                }
            } else {
                continue;
            }
        }
        return IDX_NOT_FOUND;
    }

    void insert(const char *key, const byte *data, const size_t length){/*{{{*/
        if(locate(key) != IDX_NOT_FOUND) remove(key);
        struct index idx;
        idx.used = 1;
        bzero(idx.key, KEY_LEN);
        strcpy(idx.key, key);
        idx.length = length;
        fseek(idxfs, findUnsetIdx() * IDX_SIZE, SEEK_SET);
        idx.offset = findUnsetBif();
        fwrite(&idx, IDX_SIZE, 1, idxfs);
        
        int spice_count = length / BLOCK_SIZE + 1;
        int i = 0;
        int final = BIF_END;
        vector<uint32_t> v;
        v.push_back(idx.offset);
        for(i = 0; i < spice_count - 1; i++) {
            v.push_back(findUnsetBif());
        }
        size_t write_len = 0;

        for(i = 0; i < spice_count; i++) {
            fseek(biffs, BIF_SIZE * v[i], SEEK_SET);
            if(i == spice_count - 1) {
                fwrite(&final, BIF_SIZE, 1, biffs);
            } else {
                fwrite(&v[i + 1], BIF_SIZE, 1, biffs);
            }
            //write to dmd
            writeDmd(data + write_len, length - write_len, v[i]);
            write_len += BLOCK_SIZE;
        }
    }/*}}}*/
    
    void remove(const char *key) {
        uint32_t idx_offset = locate(key);
        struct index idx;
        if(IDX_NOT_FOUND == idx_offset) {
            return;
        }
        fseek(idxfs, IDX_SIZE * idx_offset, SEEK_SET);
        fread(&idx, IDX_SIZE, 1, idxfs);
        fseek(idxfs, (-1) * IDX_SIZE , SEEK_CUR);
        idx.used = 0;
        fwrite(&idx, IDX_SIZE, 1, idxfs);
        fflush(idxfs);
        uint32_t bif_offset = idx.offset;
        for( ; ; ) {
            uint32_t old_bif_offset = bif_offset;
            bif_offset = readBif(bif_offset);
            eraseBif(old_bif_offset);
            if(bif_offset == BIF_END || bif_offset == BIF_NOT_USE) return;
        }
    }

private:
    FILE *biffs;
    FILE *idxfs;
    FILE *dmdfs;
    string dir_path;
    string name;

    void readFiles() {/*{{{*/
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
    }/*}}}*/
    uint32_t findUnsetIdx() {/*{{{*/
        struct index idx;
        int i = 0;
        fseek(idxfs, 0, SEEK_SET);
        while(fread(&idx, IDX_SIZE, 1, idxfs) == 1) {
            if(idx.used == 0) { 
                return ftell(idxfs) / IDX_SIZE - 1;
            }
        }
        return ftell(idxfs) / IDX_SIZE;
    }/*}}}*/
    uint32_t findUnsetBif() {
        uint32_t indicator;
        uint32_t final = BIF_END;
        fseek(biffs, 0, SEEK_SET);
        while(fread(&indicator, BIF_SIZE, 1, biffs) == 1) {
            if(indicator == BIF_NOT_USE) {
                fseek(biffs, BIF_SIZE * -1, SEEK_CUR);
                fwrite(&final, BIF_SIZE, 1, biffs);
                fflush(biffs);
                return ftell(biffs) / BIF_SIZE - 1;
            }
        }
        fwrite(&final, BIF_SIZE, 1, biffs);
        fflush(biffs);
        return ftell(biffs) / BIF_SIZE - 1;
    }
    void writeDmd(const byte *data, const size_t length, const uint32_t offset) {
        int write_len = length;
        if(length > BLOCK_SIZE) write_len = BLOCK_SIZE;

        fseek(dmdfs, offset * BLOCK_SIZE, SEEK_SET);
        fwrite(data, write_len, 1, dmdfs);
        fflush(dmdfs);
        if(ftell(dmdfs) % BLOCK_SIZE != 0) {//并没有填充完
            fseek(dmdfs, BLOCK_SIZE - length - 1, SEEK_END);
            byte zero = 0;
            fwrite(&zero, 1, 1, dmdfs);
            fflush(dmdfs);
        }
    }
    void readDmd(byte* data, const uint32_t length, uint32_t offset) {
        byte buff[BLOCK_SIZE];
        fseek(dmdfs, offset * BLOCK_SIZE, SEEK_SET);
        fread(buff, BLOCK_SIZE, 1, dmdfs);
        size_t write_len = (size_t)length;
        if(write_len > BLOCK_SIZE) {
            write_len = BLOCK_SIZE;
        }
        memcpy(data, buff, write_len);
    }
    uint32_t readBif(uint32_t offset) {
        uint32_t data;
        fseek(biffs, offset * BIF_SIZE, SEEK_SET);
        fread(&data, BIF_SIZE, 1, biffs);
        return data;
    }
    void eraseBif(uint32_t offset) {
        fseek(biffs, offset * BIF_SIZE, SEEK_SET);
        uint32_t data = BIF_NOT_USE;
        fwrite(&data, BIF_SIZE, 1, biffs);
        fflush(biffs);
    }
    void eraseIdx(uint32_t offset) {
        struct index idx;
        bzero(&idx, IDX_SIZE);
        fseek(idxfs, offset * IDX_SIZE, SEEK_SET);
        fwrite(&idx, IDX_SIZE, 1, idxfs);
        fflush(idxfs);
    }
};
#endif
