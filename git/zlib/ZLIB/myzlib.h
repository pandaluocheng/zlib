#ifndef _MYZLIB_H_
#define _MYZLIB_H_
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "./include/zlib.h"
using namespace std;
class MyZlib{
public:
	MyZlib();
	MyZlib(const string& filename);
	~MyZlib();
	bool mygzread();
	int gzcompress(Bytef *data, uLong ndata,
	Bytef *zdata, uLong *nzdata);
	int gzdecompress(Byte *zdata, uLong nzdata,
			Byte *data, uLong *ndata);
private:
	string m_filename;
	gzFile m_fp;
	char m_data[256];
};
#endif