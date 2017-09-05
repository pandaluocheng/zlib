#include <iostream>
#include <cstdio>
#include <fstream>
#include "./include/zlib.h"
#include <string.h>
#include <fcntl.h>
using namespace std;
/* data 原数据 ndata 原数据长度 zdata 压缩后数据 nzdata 压缩后长度 */
int gzcompress(Bytef *data, uLong ndata,
               Bytef *zdata, uLong *nzdata)
{
    z_stream c_stream;
    int err = 0;

    if (data && ndata > 0) {
        c_stream.zalloc = NULL;
        c_stream.zfree = NULL;
        c_stream.opaque = NULL;
        //只有设置为MAX_WBITS + 16才能在在压缩文本中带header和trailer
        if (deflateInit2(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                         MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) return -1;
        c_stream.next_in = data;
        c_stream.avail_in = ndata;
        c_stream.next_out = zdata;
        c_stream.avail_out = *nzdata;
        while (c_stream.avail_in != 0 && c_stream.total_out < *nzdata) {
            if (deflate(&c_stream, Z_NO_FLUSH) != Z_OK) return -1;
        }
        if (c_stream.avail_in != 0) return c_stream.avail_in;
        for (;;) {
            if ((err = deflate(&c_stream, Z_FINISH)) == Z_STREAM_END) break;
            if (err != Z_OK) return -1;
        }
        if (deflateEnd(&c_stream) != Z_OK) return -1;
        *nzdata = c_stream.total_out;
        return 0;
    }
    return -1;
}

/* zdata 数据 nzdata 原数据长度 data 解压后数据 ndata 解压后长度 */
int gzdecompress(Byte *zdata, uLong nzdata,
                 Byte *data, uLong *ndata)
{
    int err = 0;
    z_stream d_stream = { 0 }; /* decompression stream */
    static char dummy_head[2] = {
        0x8 + 0x7 * 0x10,
        (((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
    };
    d_stream.zalloc = NULL;
    d_stream.zfree = NULL;
    d_stream.opaque = NULL;
    d_stream.next_in = zdata;
    d_stream.avail_in = 0;
    d_stream.next_out = data;
    //只有设置为MAX_WBITS + 16才能在解压带header和trailer的文本
    if (inflateInit2(&d_stream, MAX_WBITS + 16) != Z_OK) return -1;
    //if(inflateInit2(&d_stream, 47) != Z_OK) return -1;
    while (d_stream.total_out < *ndata && d_stream.total_in < nzdata) {
        d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
        if ((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) break;
        if (err != Z_OK) {
            if (err == Z_DATA_ERROR) {
                d_stream.next_in = (Bytef*)dummy_head;
                d_stream.avail_in = sizeof(dummy_head);
                if ((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK) {
                    return -1;
                }
            }
            else return -1;
        }
    }
    if (inflateEnd(&d_stream) != Z_OK) return -1;
    *ndata = d_stream.total_out;
    return 0;
}

#define BUF_SIZE 65535
int main()
{
	
	 /* char *data = "this is a gzip test from NinGoo.net";
	  gzFile fp=NULL;
	  fp=gzopen("test_out.gz","wb");
	  gzwrite(fp,data,strlen(data));
	  gzclose(fp);
	Bytef zdata[BUF_SIZE];
	uLong nzdata = BUF_SIZE;
	Bytef  odata[BUF_SIZE];
	uLong nodata = BUF_SIZE;
	
		FILE* fd = fopen("test_out.gz", "rb");
		int fread_len = fread(zdata,1,sizeof(zdata),fd);
		printf("fread_len:%d\n",fread_len);
	
	
		    for(int i = 0; i < fread_len; i++)  
           {  
                printf(" %02x",zdata[i]);  
           }  
           printf("\n");  
		memset(odata, 0, BUF_SIZE);
		int  flag = gzdecompress(zdata, fread_len, odata, &nodata);
			cout<<flag<<endl;
			fprintf(stdout, "len:%d data:%s\n", nodata, odata);*/


			
	/*char *data = "kjdalkfjdflkjdlkfjdklfjdlkfjlkdjflkdjflddajfkdjfkdfaskf;ldsfk;ldakf;ldskfl;dskf;ld";	
	uLong ndata = strlen(data);	
	Bytef zdata[BUF_SIZE];
	uLong nzdata = BUF_SIZE;
	Bytef  odata[BUF_SIZE];
	uLong nodata = BUF_SIZE;
	
	memset(zdata, 0, BUF_SIZE);
	//if(zcompress((Bytef *)data, ndata, zdata, &nzdata) == 0)
	if(gzcompress((Bytef *)data, ndata, zdata, &nzdata) == 0)
	{
		fprintf(stdout, "nzdata:%d %s\n", nzdata, zdata);
		memset(odata, 0, BUF_SIZE);
		//if(zdecompress(zdata, ndata, odata, &nodata) == 0)
		if(gzdecompress(zdata, ndata, odata, &nodata) == 0)
		{
			fprintf(stdout, "%d %s\n", nodata, odata);
		}
	}*/
	Bytef  odata[BUF_SIZE];
	uLong nodata = BUF_SIZE;
	Bytef buff[100];
	char *data = "this is a gzip test from NinGoo.net,dasdasdasdasdas, \
		asdadd dasdasd ,dasdasdsaaikgergherp hoerkppwefwwfwkefwefpwefwefwefp \
		 asdasdasdjehpoejhopreophjerpohjerpohjerpo \
		 this is a gzip test from NinGoo.net,dasdasdasdasdas \
		 this is a gzip test from NinGoo.net,dasdasdasdasdas \
		 this is a gzip test from NinGoo.net,dasdasdasdasdas \
		 this is a gzip test from NinGoo.net,dasdasdasdasdas \
		";
	cout<<strlen(data)<<endl;
	gzFile fp=NULL;
	fp=gzopen("test_out.gz","wb");
	gzwrite(fp,data,strlen(data));
	gzclose(fp);
	FILE* fd = fopen("test_out.gz", "rb");
	while(!feof(fd))
	{
		bzero(buff,0);
		int len = fread(buff,1,128,fd);
		//fgets((char*)buff,128,fd);
		cout<<"slen:"<<len<<endl;
		if(gzdecompress(buff, 128, odata, &nodata) == 0)
		{
			fprintf(stdout, "dlen:%d data:%s\n", nodata, odata);
		}
	}
	return 0;
}