#ifndef __DIST_DATA_H__
#define __DIST_DATA_H__
#include "distmem.h"
class DataAtom{
public:
	byte *data;
	DataAtom():data(NULL), length(0){}
	DataAtom(const byte *d, size_t l):length(l){
		this->data = new byte[this->length];
		std::memcpy(this->data, d, this->length);
	}
	~DataAtom(){
		if(this->length > 0) delete this->data;
	}
	void set(const byte *d, const size_t l);
	void append(const byte *d, const size_t l);
	size_t size() const {
		return this->length;
	}
private:
	size_t length;
};
void DataAtom::set(const byte *d, const size_t l) {
	delete this->data;
	this->length = l;
	this->data = new byte[this->length];
	std::memcpy(this->data, d, this->length);
}
void DataAtom::append(const byte *d, const size_t l) {
	byte *tmp = new byte[this->length + l];
	std::memcpy(tmp, this->data, this->length);
	std::memcpy(tmp + this->length, d, l);
	delete this->data;
	this->data = tmp;
	this->length += l;
}
class BPMap{

};
#endif