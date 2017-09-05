#include "myzlib.h"
#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;
const ulong Buffsize=128;
int main(){
	
	/*string filename = "zran.tar.gz";
	MyZlib myzlib(filename);
	bool flag = myzlib.mygzread(buff);
	if(!flag){
		cout<<"读取错误";
	}*/
	
	
	char data[128];
	gzFile fp = NULL;
	fp = gzopen("zran.tar.gz","rb");
	gzseek(fp,0,SEEK_END);
	unsigned long len = gztell(fp);
	gzseek(fp,0,SEEK_SET);
	cout<<"file len:"<<len<<endl;
	while(!gzeof(fp)){
		memset(data,0,sizeof(data));
		gzread(fp,data,128);
		data[128] = '\0';
		cout<<data;
	}
	gzclose(fp);
	
	/*Bytef  buff[Buffsize];
	Bytef odata[1024];
	ulong olen = 1024;
	FILE* file = fopen("zran.tar.gz", "rb");
	MyZlib myzlib;
	while(!feof(file)){
		bzero(buff,0);
		bzero(odata,0);
		int len = fread(buff,1,128,file);
		cout<<"len:"<<len<<endl;
		if(myzlib.gzdecompress(buff,Buffsize,odata,&olen) == 0){
			cout<<"dlen:"<<olen<<endl;
			cout<<"data:"<<odata<<endl;
		}

			
	}*/
	//fclose(file);
}