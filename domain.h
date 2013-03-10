#ifndef __DM_DOMAIN_H__
#define __DM_DOMAIN_H__
#include <sys/stat.h>
#include <sys/types.h>
#include <distmem.h>
#include <btree.hpp>
#include <cache.hpp>
class Domain
{
public:
    Domain(std::string &str);
    Domain(const char *str);
    ~Domain();
    void get(const char *key, byte* &data, size_t &length);
    void set(const char *key, const byte *data, const size_t length);
    void remove(const char *key);
private:
    FILE *biffs;
    FILE *idxfs;
    FILE *dmdfs;
    BTree<class index, 1000> *tree;
    Cache<class index, 3> cache;
    string dir_path;
    string name;

    void readFiles();
    uint32_t findUnsetIdx();
    uint32_t findUnsetBif();
    void writeDmd(const byte *data, const size_t length, const uint32_t offset);
    void readDmd(byte* data, const uint32_t length, uint32_t offset);
    uint32_t readBif(uint32_t offset);
    void eraseBif(uint32_t offset);
    void eraseIdx(uint32_t offset);
    class index* locate(const char *key);
    void insert(class index *idx);
};
#endif
