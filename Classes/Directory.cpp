#include "Directory.h"

Directory::Directory(string name, Directory *parent)
{
	this->name = name;
	this->parent = parent;
	this->child = NULL;
	this->sibling = NULL;
	this->fileList = NULL;
}

void Directory::createSubdirectory(string name)
{
	Directory *newDirectory = new Directory(name, this);
	if (this->child == NULL) this->child = newDirectory;
	else {
		Directory *current = this->child;
		while (current->sibling != NULL) current = current->sibling;
		current->sibling = newDirectory;
	}
}

void Directory::cteateNewFile(string name, int address)
{
	File *newFile = new File(name, address);
	if (this->fileList == NULL) this->fileList = newFile;
	else {
		File *current = this->fileList;
		while (current->nextFile != NULL) current = current->nextFile;
		current->nextFile = newFile;
	}
}

void Directory::deleteSubdirectory(string name)
{
	if (this->child->name == name) {
		Directory *current = this->child;
		this->child = current->sibling;
		remove(current);
	}
	else {
		Directory *current = this->child;
		while (current->sibling->name != name) current = current->sibling;
		Directory *target = current->sibling;
		current->sibling = target->sibling;
		remove(target);
	}
}

void Directory::remove(Directory * current)
{
	if (current != NULL) {
		while (current->fileList != NULL) current->deleteFile(current->fileList->getName());
		while (current->child != NULL) current->deleteSubdirectory(current->child->getName());
		delete current;
	}
}

void Directory::deleteFile(string name)
{
	if (this->fileList->getName() == name) {
		File *current = this->fileList;
		this->fileList = current->nextFile;
		delete(current);
	}
	else {
		File *current = this->fileList;
		while (current->nextFile->getName() != name) current = current->nextFile;
		File *target = current->nextFile;
		current->nextFile = target->nextFile;
		delete(target);
	}
}

Directory * Directory::searchFolder(string name)
{
	Directory *current = this->child;
	while (current->getName() != name) current = current->sibling;
	return current;
}

File * Directory::searchFile(string name)
{
	File *current = this->fileList;
	while (current->getName() != name) current = current->nextFile;
	return current;
}

Directory::~Directory()
{
}