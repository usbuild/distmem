#include <domain.h>
Domain::Domain(std::string &str):name(str) {
        dir_path = string(DATA_PATH) +  PATH_SEP + name + PATH_SEP;
        readFiles();
}
Domain::Domain(const char *str):name(str) {
        dir_path = string(DATA_PATH) +  PATH_SEP + name + PATH_SEP;
        readFiles();
}
Domain::~Domain() {
        fclose(biffs);
        fclose(idxfs);
        fclose(dmdfs);
}
void Domain::get(const char *key, byte* &data, size_t &length) { /*{{{*/
        Index* idx = locate(key);
        if(idx == NULL || idx->used == 0) {
            data = NULL;
            length = 0;
            return;
        }
        data = (byte*) malloc(sizeof(byte) * idx->length);
        length = idx->length;
        uint32_t offset =  idx->offset;

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
        length = idx->length;
}/*}}}*/
void Domain::set(const char *key, const byte *data, const size_t length){/*{{{*/

    Index* idx = locate(key);
    if(idx == NULL) {
        idx = new Index();
    }
    idx->used = 1;
    bzero(idx->key, KEY_LEN);
    strcpy(idx->key, key);
    idx->length = length;
    idx->offset = findUnsetBif();
    insert(idx);

    int spice_count = length / BLOCK_SIZE + 1;
    int i = 0;
    int final = BIF_END;
    vector<uint32_t> v;
    v.push_back(idx->offset);
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
void Domain::remove(const char *key) {/*{{{*/
    Index* idx = locate(key);
    if(idx == NULL) { //waiting for handling
        return;
    }
    idx->used = 0;
    uint32_t bif_offset = idx->offset;
    insert(idx);
    for( ; ; ) {
        uint32_t old_bif_offset = bif_offset;
        bif_offset = readBif(bif_offset);
        eraseBif(old_bif_offset);
        if(bif_offset == BIF_END || bif_offset == BIF_NOT_USE) return;
    }
}/*}}}*/
void Domain::readFiles() {/*{{{*/
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
    tree = new BTree<Index, 1000>(idxfs);
    free(idxpath);
    free(bifpath);
    free(dmdpath);
}/*}}}*/
uint32_t Domain::findUnsetBif() {/*{{{*/
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
}/*}}}*/
void Domain::writeDmd(const byte *data, const size_t length, const uint32_t offset) {/*{{{*/
    int write_len = length;
    if(length > BLOCK_SIZE) write_len = BLOCK_SIZE;

    fseek(dmdfs, offset * BLOCK_SIZE, SEEK_SET);
    fwrite(data, write_len, 1, dmdfs);
    if(ftell(dmdfs) % BLOCK_SIZE != 0) {//并没有填充完

        fseek(dmdfs, BLOCK_SIZE - ftell(dmdfs) % BLOCK_SIZE - 1, SEEK_CUR);
        byte zero = 0;
        fwrite(&zero, 1, 1, dmdfs);
    }
    fflush(dmdfs);
}/*}}}*/
void Domain::readDmd(byte* data, const uint32_t length, uint32_t offset) {/*{{{*/
    byte buff[BLOCK_SIZE];
    fseek(dmdfs, offset * BLOCK_SIZE, SEEK_SET);
    fread(buff, BLOCK_SIZE, 1, dmdfs);
    size_t write_len = (size_t)length;
    if(write_len > BLOCK_SIZE) {
        write_len = BLOCK_SIZE;
    }
    memcpy(data, buff, write_len);
}/*}}}*/
uint32_t Domain::readBif(uint32_t offset) {/*{{{*/
    uint32_t data;
    fseek(biffs, offset * BIF_SIZE, SEEK_SET);
    fread(&data, BIF_SIZE, 1, biffs);
    return data;
}/*}}}*/
void Domain::eraseBif(uint32_t offset) {/*{{{*/
    fseek(biffs, offset * BIF_SIZE, SEEK_SET);
    uint32_t data = BIF_NOT_USE;
    fwrite(&data, BIF_SIZE, 1, biffs);
    fflush(biffs);
}/*}}}*/
void Domain::eraseIdx(uint32_t offset) {/*{{{*/
    Index idx;
    bzero(&idx, IDX_SIZE);
    fseek(idxfs, offset * IDX_SIZE, SEEK_SET);
    fwrite(&idx, IDX_SIZE, 1, idxfs);
    fflush(idxfs);
}/*}}}*/
Index* Domain::locate(const char *key) {/*{{{*/
    Index idx, *pidx;
    strcpy(idx.key, key);
    // use for  cache result
    pidx = cache.get(idx);
    if(pidx == NULL) {
        pidx = tree->search(idx);
        if(pidx != NULL) {
            cache.set(*pidx);
        }
    }
    return pidx;
}/*}}}*/

void Domain::insert(Index *idx) {
    cache.set(*idx);
    tree->insert(*idx);
}
