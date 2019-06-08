#ifndef __DIRECTORY_SCENE_H__
#define __DIRECTORY_SCENE_H__

#include "cocos2d.h"
#include "FileSystem.h"
#include "ui\CocosGUI.h"

class DirectoryScene : public cocos2d::Scene,cocos2d::ui::EditBoxDelegate
{
private:
	bool hasEditedName = false;
	int content[5][5] = { 0 };
	std::string currentName;
	cocos2d::ui::EditBox *editName;
	cocos2d::Sprite *selected;
	FileSystem *fs;
public:
	static const int FOLDER = 1;
	static const int FILE = 2;
    static DirectoryScene* createScene(FileSystem *fs);
    virtual bool init();
	void enemyInit(FileSystem *fs);
	void initDirectory();
	void initFile();
	void createMenu();
	void createEditNameBox();
	void onMouseDown(cocos2d::Event *event);
	void createFolder(std::string name);
	void createFile(std::string name);
	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox *editBox) {};						//开始编辑
	virtual void editBoxTextChanged(cocos2d::ui::EditBox *editBox, const std::string& text);	//编辑框文字改变
	virtual void editBoxReturn(cocos2d::ui::EditBox *editBox);									//结束编辑
    CREATE_FUNC(DirectoryScene);
};

#endif // __DIRECTORY_SCENE_H__
