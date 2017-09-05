#include "static.h"

class Directory{
public:
	Directory(){}
	Directory(int param);
private:
	int m_param;
};
Directory::Directory(int param):m_param(param){
	std::size_t disks = tfs.numDisk();
}
const int num=1;
Directory d1(num);
Filesystem tfs;
int main(){
	
}