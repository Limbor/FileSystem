#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include "Directory.h"
#include "Disk.h"
#include <vector>

class FileSystem
{
public:
	Directory *root;
	Directory *current;

	FileSystem();
	void format(Directory *directory);
	string getPath();
	Directory *back();
	void createSubdirectory(string name);
	void cteateNewFile(string name);
	void deleteSubdirectory(string name);
	void deleteFile(string name);
	vector<string> folderNames();
	vector<string> fileNames();
	string getContent(File *file);
	void setContent(File *file, string content);
	~FileSystem();
private:
	Disk *disk = new Disk();
};

#endif // _FILE_SYSTEM_H_
