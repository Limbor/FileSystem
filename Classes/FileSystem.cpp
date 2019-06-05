#include "FileSystem.h"

FileSystem::FileSystem()
{
	this->root = new Directory("root", NULL);
	this->current = root;
}

void FileSystem::format()
{
	while (root->fileList != NULL) root->deleteFile(root->fileList->getName());
	while (root->child != NULL) root->deleteSubdirectory(root->child->getName());
}

string FileSystem::getPath()
{
	string path = current->getName();
	Directory *temp = this->current;
	while (temp != root) {
		temp = temp->parent;
		path = temp->getName() + "/" + path;
	}
	return path;
}

Directory * FileSystem::back()
{
	if (current == root) return NULL;
	return current->parent;
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

FileSystem::~FileSystem()
{
}