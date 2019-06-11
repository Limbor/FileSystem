#ifndef _DIRECTORY_H_
#define _DIRECTORY_H_

#include "File.h"

class Directory
{
private:
	string name;				//目录名称
	Directory *parent;			//父目录
	Directory *sibling;			//兄弟目录
	Directory *child;			//子目录
	File *fileList;				//文件列表
public:
	friend class FileSystem;
	Directory(string name, Directory *parent);
	void setName(string name) { this->name = name; }
	string getName() { return this->name; }
	void createSubdirectory(string name);			//创建子目录
	void cteateNewFile(string name, int address);	//创建子文件
	void deleteSubdirectory(string name);			//删除子目录
	void remove(Directory *current);				//递归删除全部目录
	void deleteFile(string name);					//删除子文件
	Directory *searchFolder(string name);			//寻找子目录
	File *searchFile(string name);					//寻找子文件
	~Directory();
};

#endif // _DIRECTORY_H_
