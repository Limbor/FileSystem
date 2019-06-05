#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include "Directory.h"
#include <vector>

class FileSystem
{
public:
	Directory *root;
	Directory *current;

	FileSystem();
	void format();
	string getPath();
	Directory *back();
	vector<string> folderNames();
	vector<string> fileNames();
	~FileSystem();
};

#endif // _FILE_SYSTEM_H_
