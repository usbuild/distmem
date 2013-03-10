#ifndef __DM_CACHE_HPP__
#define __DM_CACHE_HPP__
#include <string.h>

template<typename T, int size>
class Cache
{
private:
    T data[size];
    int used;
public:
    Cache():used(0){}
    T* get(T &t);
    void set(T &t);
    int contains(T &t);
};
template<typename T, int size>
int Cache<T, size>::contains(T &t) {
    for (int i = 0; i < used; i++) {
        if(data[i] == t) {
            return i;
        }
    }
    return -1;
}

//implementation
template<typename T, int size>
T* Cache<T, size>::get(T &t) {
    int pos = this->contains(t);
    if(pos == -1) return NULL;
    T temp;
    memcpy(&temp, &data[pos], sizeof(T));
    memmove(data + 1, data, sizeof(T) * pos);
    memcpy(&data[0], &temp, sizeof(T));
    return &data[0];
}

template<typename T, int size>
void Cache<T, size>::set(T &t) {
    if(get(t) == NULL) {
        if(used != size) ++used;
        memcpy(&data[used - 1], &t, sizeof(T));
    }
    get(t);
}

#endif
