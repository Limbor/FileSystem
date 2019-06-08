#include "Disk.h"

Disk::Disk()
{
}

int Disk::searchFreeBlock()
{
	for (int i = 0; i < BlockNumber; i++) {
		if (bitMap[i] == 0) return i;
	}
	return -1;
}

void Disk::clear()
{
	delete blocks;
	blocks = new Block[BlockNumber];
	delete bitMap;
	bitMap = new int[BlockNumber];
}

Disk::~Disk()
{
}