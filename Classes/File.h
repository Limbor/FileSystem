#ifndef _FILE_H_
#define _FILE_H_

#include <iostream>

using namespace std;

class File
{
private:
	string name;
	int address;
	int length;
	File *nextFile;
public:
	friend class Directory;
	friend class FileSystem;
	File(string name) :name(name), length(0),
		address(0), nextFile(NULL) {};
	void setName(string name) { this->name = name; }
	string getName() { return name; }
	void setLength(int length) { this->length = length; }
	int getLength() { return length; }
	void setAddress(int address) { this->address = address; }
	int getAddress() { return address; }
};

#endif // _FILE_H_