#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include "Directory.h"
#include "Disk.h"
#include <fstream>
#include <vector>

//Ŀ¼�ṹ��
//��С48B
struct Catalog {
	byte name[20] = { 0 };		//����
	byte father[20] = { 0 };	//��Ŀ¼����
	int type;					//���ͣ�1���ļ��� 2���ļ�
	int address;				//��ַ
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
	Directory *root;							//��Ŀ¼
	Directory *current;							//��ǰĿ¼

	FileSystem();
	void format(Directory *directory);			//��ʽ��ָ��Ŀ¼
	string getPath();							//��ȡ·��
	Directory *back();							//��ȡ��Ŀ¼
	void createSubdirectory(string name);		//�ڵ�ǰĿ¼������Ŀ¼
	void cteateNewFile(string name);			//�������ļ�
	void deleteSubdirectory(string name);		//ɾ����Ŀ¼
	void deleteFile(string name);				//ɾ���ļ�
	void cleanDisk(Directory *parent);			//�������
	vector<string> folderNames();				//��ȡ������Ŀ¼����
	vector<string> fileNames();					//��ȡ�����ļ�����
	string getContent(File *file);				//��ȡ�ļ�����
	void setContent(File *file, string content);//�����ļ�����
	bool readFile();							//��ȡ�ⲿ�ļ�
	int getFileSize();							//��ȡ�ⲿ�ļ���С
	bool writeFile();							//����ǰ��Ϣȫ��д���ⲿ�ļ�
	~FileSystem();
private:
	string diskFile = "disk.dat";
	string diretoryFile = "directory.dat";
	Disk *disk = new Disk();
};

#endif // _FILE_SYSTEM_H_
