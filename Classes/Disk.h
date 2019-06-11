#ifndef _DISK_H_
#define _DISK_H_

#include <iostream>
#include <Windows.h>

//�ڴ��ṹ��
//��СΪ512B
struct Block
{
	byte content[508] = { 0 };		//����
	int link = -1;					//��������
};

class Disk
{
public:
	friend class FileSystem;
	static const int BlockSize = 512;						//�ڴ���С
	static const int DiskSize = 2 * 1024 * 1024;			//���̴�С
	static const int BlockNumber = DiskSize / BlockSize;	//�ڴ�����
	Disk();
	int searchFreeBlock();									//Ѱ�ҿ����ڴ��
	void clear();											//��մ���
	~Disk();
private:
	int *bitMap = new int[BlockNumber]{ 0 };
	Block *blocks = new Block [BlockNumber];
};

#endif // _DISK_H_
