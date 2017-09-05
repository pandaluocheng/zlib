#include "myzlib.h"
MyZlib::MyZlib():m_fp(NULL){
		memset(m_data,0,sizeof(m_data));
}
MyZlib::MyZlib(const string& filename):m_filename(filename),m_fp(NULL){
	memset(m_data,0,sizeof(m_data));
}
MyZlib::~MyZlib(){
	gzclose(m_fp);
}
/*bool MyZlib::mygzread(){
	m_fp = gzopen(m_filename.c_str(),"rb");
	if(!m_fp)
	{
		perror("open file error");
		exit(-1);
	}
	gzseek(m_fp,0,SEEK_END);
	unsigned long len = gztell(m_fp);
	gzseek(m_fp,0,SEEK_SET);
	while(!gzeof(m_fp)){
		memset(m_data,0,sizeof(m_data));
		gzread(m_fp,m_data,sizeof(m_data));
		m_data[255]='\0';
		cout<<m_data<<" ";
	}
	return 1;
}*/
int MyZlib::gzcompress(Bytef *data, uLong ndata,
               Bytef *zdata, uLong *nzdata){
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


int MyZlib::gzdecompress(Byte *zdata, uLong nzdata,
                 Byte *data, uLong *ndata){
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