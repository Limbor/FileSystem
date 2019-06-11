#ifndef __DIRECTORY_SCENE_H__
#define __DIRECTORY_SCENE_H__

#include "cocos2d.h"
#include "FileSystem.h"
#include "ui\CocosGUI.h"

class DirectoryScene : public cocos2d::Scene,cocos2d::ui::EditBoxDelegate
{
private:
	bool isHelping = false;									//是否正处于帮助界面
	bool hasEditedName = false;								//用户是否已经输入
	int content[5][5] = { 0 };								//记录场景中每个位置的物件
	std::string currentName;								//用户输入的名字
	cocos2d::ui::EditBox *editName;							//名字输入框
	cocos2d::Sprite *selected;								//当前选中的物件
	FileSystem *fs;											//文件管理系统
public:
	static const int FOLDER = 1;
	static const int FILE = 2;
    static DirectoryScene* createScene(FileSystem *fs);
    virtual bool init();
	void enemyInit(FileSystem *fs);
	void initDirectory();									//显示当前目录下的文件夹
	void initFile();										//显示当前目录下的文件
	void createMenu();										//创建菜单
	void createEditNameBox();								//创建编辑框
	void onMouseDown(cocos2d::Event *event);				//鼠标点击事件
	void createFolder(std::string name);					//创建文件夹
	void createFile(std::string name);						//创建文件
	void selectFolder();									//选中文件夹触发的事件
	void selectFile();										//选中文件触发的事件
	void selectNothing();									//选中空白触发的事件
	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox *editBox) {};						//开始编辑
	virtual void editBoxTextChanged(cocos2d::ui::EditBox *editBox, const std::string& text);	//编辑框文字改变
	virtual void editBoxReturn(cocos2d::ui::EditBox *editBox);									//结束编辑
    CREATE_FUNC(DirectoryScene);
};

#endif // __DIRECTORY_SCENE_H__
