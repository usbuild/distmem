#ifndef __DIST_FILE_H__
#define __DIST_FILE_H__
#include <string>
#include <fstream>
#ifdef _WIN32
#include <fileapi.h>
#endif
#include "distmem.h"
class Domain
{
public:
	Domain(std::string &str):name(str){}
	Domain(const char *str):name(str){}
	virtual ~Domain(){
		idxfs->close();
		biffs->close();
		dmdfs->close();
	}
	
	bool makeFiles() {
		idxfs = new std::fstream(name + IDX_EXT, std::ios::binary | std::ios::out);
		biffs = new std::fstream(name + BIF_EXT, std::ios::binary | std::ios::out);
		dmdfs = new std::fstream(name + DMD_EXT, std::ios::binary | std::ios::out);
		return true;
	}
private:
	std::string name;
	std::fstream *idxfs;
	std::fstream *biffs;
	std::fstream *dmdfs;
	
};
#endif