#ifndef _FILE_H_
#define _FILE_H_

#include <iostream>

using namespace std;

class File
{
private:
	string name;			//�ļ���
	int address;			//�ļ���ַ
	int length;				//�ļ���С

	File *nextFile;			//�ļ�����ָ��
public:
	friend class Directory;
	friend class FileSystem;
	File(string name, int address) :name(name), length(1),
		address(address), nextFile(NULL) {};
	void setName(string name) { this->name = name; }
	string getName() { return name; }
	void setLength(int length) { this->length = length; }
	int getLength() { return length; }
	void setAddress(int address) { this->address = address; }
	int getAddress() { return address; }
};

#endif // _FILE_H_