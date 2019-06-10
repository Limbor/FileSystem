#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include "Directory.h"
#include "Disk.h"
#include <fstream>
#include <vector>

struct Catalog {
	byte name[20] = { 0 };
	byte father[20] = { 0 };
	int type;
	int address;
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
	void cleanDisk(Directory *parent);
	vector<string> folderNames();
	vector<string> fileNames();
	string getContent(File *file);
	void setContent(File *file, string content);
	bool readFile();
	int getFileSize();
	bool writeFile();
	~FileSystem();
private:
	string diskFile = "disk.dat";
	string diretoryFile = "directory.dat";
	Disk *disk = new Disk();
};

#endif // _FILE_SYSTEM_H_
