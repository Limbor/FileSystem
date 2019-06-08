#include "FileSystem.h"
#include <cstring>

FileSystem::FileSystem()
{
	this->root = new Directory("Root", NULL);
	this->current = root;
}

void FileSystem::format(Directory *directory)
{
	while (directory->fileList != NULL) 
		directory->deleteFile(directory->fileList->getName());
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

FileSystem::~FileSystem()
{
}