#ifndef _DIRECTORY_H_
#define _DIRECTORY_H_

#include "File.h"

class Directory
{
private:
	string name;
	Directory *parent;
	Directory *sibling;
	Directory *child;
	File *fileList;
public:
	friend class FileSystem;
	Directory(string name, Directory *parent);
	void setName(string name) { this->name = name; }
	string getName() { return this->name; }
	void createSubdirectory(string name);
	void cteateNewFile(string name);
	void deleteSubdirectory(string name);
	void remove(Directory *current);
	void deleteFile(string name);
	Directory *searchFolder(string name);
	File *searchFile(string name);
	~Directory();
};

#endif // _DIRECTORY_H_
