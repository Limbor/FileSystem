#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include "Directory.h"
#include "Disk.h"
#include <fstream>
#include <vector>

//目录结构体
//大小48B
struct Catalog {
	byte name[20] = { 0 };		//名字
	byte father[20] = { 0 };	//父目录名字
	int type;					//类型：1、文件夹 2、文件
	int address;				//地址
	Catalog(string name, string father, int type, int addres) {
		this->address = addres;
		this->type = type;
		for (int i = 0; i < (int)name.length(); i++) {
			this->name[i] = (byte)name[i];
		}
		for (int i = 0; i < (int)father.length(); i++) {
			this->father[i]=(byte)father[i];
		}
	}
};

class FileSystem
{
public:
	Directory *root;							//根目录
	Directory *current;							//当前目录

	FileSystem();
	void format(Directory *directory);			//格式化指定目录
	string getPath();							//获取路径
	Directory *back();							//获取父目录
	void createSubdirectory(string name);		//在当前目录创建子目录
	void cteateNewFile(string name);			//创建子文件
	void deleteSubdirectory(string name);		//删除子目录
	void deleteFile(string name);				//删除文件
	void cleanDisk(Directory *parent);			//清除磁盘
	vector<string> folderNames();				//获取所有子目录名字
	vector<string> fileNames();					//获取所有文件名字
	string getContent(File *file);				//获取文件内容
	void setContent(File *file, string content);//设置文件内容
	bool readFile();							//读取外部文件
	int getFileSize();							//获取外部文件大小
	bool writeFile();							//将当前信息全部写入外部文件
	~FileSystem();
private:
	string diskFile = "disk.dat";
	string diretoryFile = "directory.dat";
	Disk *disk = new Disk();
};

#endif // _FILE_SYSTEM_H_
