#ifndef _DIRECTORY_H_
#define _DIRECTORY_H_

#include "File.h"

class Directory
{
private:
	string name;				//Ŀ¼����
	Directory *parent;			//��Ŀ¼
	Directory *sibling;			//�ֵ�Ŀ¼
	Directory *child;			//��Ŀ¼
	File *fileList;				//�ļ��б�
public:
	friend class FileSystem;
	Directory(string name, Directory *parent);
	void setName(string name) { this->name = name; }
	string getName() { return this->name; }
	void createSubdirectory(string name);			//������Ŀ¼
	void cteateNewFile(string name, int address);	//�������ļ�
	void deleteSubdirectory(string name);			//ɾ����Ŀ¼
	void remove(Directory *current);				//�ݹ�ɾ��ȫ��Ŀ¼
	void deleteFile(string name);					//ɾ�����ļ�
	Directory *searchFolder(string name);			//Ѱ����Ŀ¼
	File *searchFile(string name);					//Ѱ�����ļ�
	~Directory();
};

#endif // _DIRECTORY_H_
