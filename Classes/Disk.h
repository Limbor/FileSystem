#ifndef _DISK_H_
#define _DISK_H_

#include <iostream>
#include <Windows.h>

//内存块结构体
//大小为512B
struct Block
{
	byte content[508] = { 0 };		//内容
	int link = -1;					//链接索引
};

class Disk
{
public:
	friend class FileSystem;
	static const int BlockSize = 512;						//内存块大小
	static const int DiskSize = 2 * 1024 * 1024;			//磁盘大小
	static const int BlockNumber = DiskSize / BlockSize;	//内存块个数
	Disk();
	int searchFreeBlock();									//寻找空闲内存块
	void clear();											//清空磁盘
	~Disk();
private:
	int *bitMap = new int[BlockNumber]{ 0 };
	Block *blocks = new Block [BlockNumber];
};

#endif // _DISK_H_
