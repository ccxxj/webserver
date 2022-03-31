#include <iostream>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main(){
	char *ptr;
	char *buf;
	// char *symlinkpath = "test.cpp";
	// string a("whatever.cgi");
	// char actualpath[PATH_MAX + 1];
	// memset(actualpath, 0, PATH_MAX + 1);
	// // ptr = realpath(symlinkpath, actualpath);
	// ptr = realpath(a.c_str(), actualpath);
	// cout << actualpath << endl;

	long size = pathconf(".", _PC_PATH_MAX);
	if((buf = (char *)malloc((size_t)size)) != NULL)
		ptr = getcwd(buf, (size_t)size);
	cout << buf << endl;	
}
