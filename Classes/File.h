#pragma once
#include <iostream>

using namespace std;

class File
{
private:
	string name;
	int address;
	int length;
public:
	File();
	void setName(string name) { this->name = name; }
	string getName() { return name; }
	void setLength(int length) { this->length = length; }
	int getLength() { return length; }
	int getAdress() { return address; }
	~File();
};