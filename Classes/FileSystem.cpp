#include "FileSystem.h"
#include <cstring>
#include <queue>

FileSystem::FileSystem()
{
	this->root = new Directory("root", NULL);
	this->current = root;
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
	int size = (int)content.length() / (Disk::BlockSize - 4);
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

	int catalogSize = sizeof(Catalog);
	int fileSize = getFileSize();
	if (fileSize == 0 || fileSize < catalogSize || (fileSize % catalogSize) != 0) {
		return false;
	}
	int count = fileSize / catalogSize;
	fin = ifstream("file.dat", ios::binary);
	queue<Directory*> directorys;
	directorys.push(root);
	for (int i = 0; i < count; i++) {
		Directory *father = directorys.front();
		Catalog catalog("", "", 0, 0);
		fin.read((char*)(&catalog), sizeof(catalog));
		if (catalog.type == 1) {
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
	queue<Directory*> directorys;
	directorys.push(this->root);
	while (!directorys.empty()) {
		Directory *father = directorys.front();
		directorys.pop();
		Directory *current = father->child;
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