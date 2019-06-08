#ifndef _DISK_H_
#define _DISK_H_

#include <iostream>
#include <Windows.h>

struct Block
{
	byte content[508] = { 0 };
	int link = -1;
};

class Disk
{
public:
	friend class FileSystem;
	static const int BlockSize = 512;
	static const int DiskSize = 2 * 1024 * 1024;
	static const int BlockNumber = DiskSize / BlockSize;
	Disk();
	int searchFreeBlock();
	void clear();
	~Disk();
private:
	int *bitMap = new int[BlockNumber]{ 0 };
	Block *blocks = new Block [BlockNumber];
};

#endif // _DISK_H_
