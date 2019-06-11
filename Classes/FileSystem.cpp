#include "FileSystem.h"
#include <cstring>
#include <queue>

FileSystem::FileSystem()
{
	this->root = new Directory("root", NULL);
	this->current = root;
	//恢复系统
	readFile();
}

void FileSystem::format(Directory *directory)
{
	cleanDisk(directory);
	while (directory->fileList != NULL) {
		directory->deleteFile(directory->fileList->getName());
	}
	while (directory->child != NULL)
		directory->deleteSubdirectory(directory->child->getName());
}

string FileSystem::getPath()
{
	string path = current->getName();
	Directory *temp = this->current;
	while (temp != root) {
		temp = temp->parent;
		path = temp->getName() + "\\" + path;
	}
	return path;
}

Directory * FileSystem::back()
{
	if (current == root) return NULL;
	return current->parent;
}

void FileSystem::createSubdirectory(string name)
{
	current->createSubdirectory(name);
}

void FileSystem::cteateNewFile(string name)
{
	int index = disk->searchFreeBlock();
	disk->bitMap[index] = 1;
	current->cteateNewFile(name, index);
}

void FileSystem::deleteSubdirectory(string name)
{
	cleanDisk(current->searchFolder(name));
	current->deleteSubdirectory(name);
}

void FileSystem::deleteFile(string name)
{
	File *file = current->searchFile(name);
	int index = file->address;
	while (index != -1) {
		disk->bitMap[index] = 0;
		memset(disk->blocks[index].content, 0, Disk::BlockSize - 4);
		int i = index;
		index = disk->blocks[index].link;
		disk->blocks[i].link = -1;
	}
	current->deleteFile(name);
}

void FileSystem::cleanDisk(Directory * parent)
{
	//使用层序遍历
	//将所有子目录下的文件占用的内存块清空
	queue<Directory*> directorys;
	directorys.push(parent);
	while (!directorys.empty()) {
		Directory *directory = directorys.front();
		directorys.pop();
		Directory *temp = directory->child;
		while (temp != NULL) {
			directorys.push(temp);
			temp = temp->sibling;
		}
		while (directory->fileList != NULL) {
			int index = directory->fileList->getAddress();
			while (index != -1) {
				disk->bitMap[index] = 0;
				memset(disk->blocks[index].content, 0, Disk::BlockSize - 4);
				int i = index;
				index = disk->blocks[index].link;
				disk->blocks[i].link = -1;
			}
			directory->deleteFile(directory->fileList->getName());
		}
	}
}

vector<string> FileSystem::folderNames()
{
	vector<string> names;
	Directory *temp = current->child;
	while (temp != NULL) {
		names.push_back(temp->getName());
		temp = temp->sibling;
	}
	return names;
}

vector<string> FileSystem::fileNames()
{
	vector<string> names;
	File *temp = current->fileList;
	while (temp != NULL) {
		names.push_back(temp->getName());
		temp = temp->nextFile;
	}
	return names;
}

string FileSystem::getContent(File * file)
{
	//将内存的字节信息转为字符串
	//并清空原本的磁盘空间
	string content;
	int index = file->address;
	while (index != -1) {
		Block current = disk->blocks[index];
		if (index != file->address) disk->bitMap[index] = 0;
		for (int i = 0; i < Disk::BlockSize - 4; i++) {
			content += current.content[i];
			current.content[i] = 0;
		}
		int j = index;
		index = disk->blocks[index].link;
		disk->blocks[j].link = -1;
	}
	return content;
}

void FileSystem::setContent(File * file, string content)
{
	int index = file->address;
	byte *b = (unsigned char*)content.c_str();
	//占满的内存块数量
	int size = (int)content.length() / (Disk::BlockSize - 4);
	//剩余的字节信息大小
	int remain = (int)content.length() % (Disk::BlockSize - 4);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < Disk::BlockSize - 4; j++) {
			disk->blocks[index].content[j] = b[j + i * (Disk::BlockSize - 4)];
		}
		if (i < size - 1 || (i == size - 1 && remain > 0)) {
			int next = disk->searchFreeBlock();
			disk->bitMap[next] = 1;
			disk->blocks[index].link = next;
			index = next;
		}
	}
	for (int i = 0; i < remain; i++) {
		disk->blocks[index].content[i] = b[i + size * (Disk::BlockSize - 4)];
	}
}

bool FileSystem::readFile()
{
	//以二进制方式读取文件
	//按字节大小分块写入block结构体
	//还原磁盘信息
	ifstream fin("disk.dat", ios::binary);
	if (!fin.is_open()) {
		fin.close();
		return false;
	}
	for (int i = 0; i < Disk::BlockNumber; i++) {
		Block &block = this->disk->blocks[i];
		fin.read((char *)(&block), sizeof(block));
	}
	fin.close();
	//计算出文件中catalog结构体的数量
	int catalogSize = sizeof(Catalog);
	int fileSize = getFileSize();
	if (fileSize == 0 || fileSize < catalogSize || (fileSize % catalogSize) != 0) {
		return false;
	}
	int count = fileSize / catalogSize;
	fin = ifstream("file.dat", ios::binary);
	//按层进行还原
	queue<Directory*> directorys;
	directorys.push(root);
	for (int i = 0; i < count; i++) {
		Directory *father = directorys.front();
		Catalog catalog("", "", 0, 0);
		fin.read((char*)(&catalog), sizeof(catalog));
		//还原目录的catalog信息
		if (catalog.type == 1) {
			//如果当前结点不是即将写入目录的父节点
			//取出队列中的第一个节点
			if ((char*)catalog.father == father->getName()) {
				father->createSubdirectory((char*)catalog.name);
				Directory *current = father->searchFolder((char*)catalog.name);
				directorys.push(current);
			}
			else {
				directorys.pop();
				father = directorys.front();
				father->createSubdirectory((char*)catalog.name);
				Directory *current = father->searchFolder((char*)catalog.name);
				directorys.push(current);
			}
		}
		//还原文件的catalog信息
		else if (catalog.type == 2) {
			if ((char*)catalog.father == father->getName()) {
				father->cteateNewFile((char*)catalog.name, catalog.address);
			}
			else {
				directorys.pop();
				father = directorys.front();
				father->cteateNewFile((char*)catalog.name, catalog.address);
			}
			int index = catalog.address;
			while (index != -1) {
				disk->bitMap[index] = 1;
				index = disk->blocks[index].link;
			}
		}
	}
	fin.close();
	return true;
}

int FileSystem::getFileSize()
{
	int size = 0;
	ifstream fin("file.dat", ios::_Nocreate);
	if (!fin.is_open())
	{
		return 0;
	}
	fin.seekg(0, ios::end);
	size = fin.tellg();
	fin.close();
	return size;
}

bool FileSystem::writeFile()
{
	//按二进制的方式将block结构体整体写入
	ofstream fout("disk.dat", ios::binary);
	if (!fout.is_open()) {
		fout.close();
		return false;
	}
	for (int i = 0; i < Disk::BlockNumber; i++) {
		Block &block = this->disk->blocks[i];
		fout.write((char *)(&block), sizeof(block));
	}
	fout.close();

	fout = ofstream("file.dat", ios::binary);
	if (!fout.is_open()) {
		fout.close();
		return false;
	}
	//使用层序遍历读取整个目录结构
	queue<Directory*> directorys;
	directorys.push(this->root);
	while (!directorys.empty()) {
		Directory *father = directorys.front();
		directorys.pop();
		Directory *current = father->child;
		//通过构建catalog结构体储存信息
		//以二进制写入文件
		while (current != NULL) {
			Catalog catalog(current->getName(), father->getName(), 1, -1);
			fout.write((char *)(&catalog), sizeof(catalog));
			directorys.push(current);
			current = current->sibling;
		}
		File *temp = father->fileList;
		while (temp != NULL) {
			Catalog catalog(temp->getName(), father->getName(), 2, temp->address);
			fout.write((char *)(&catalog), sizeof(catalog));
			temp = temp->nextFile;
		}
	}
	return true;
}

FileSystem::~FileSystem()
{
}