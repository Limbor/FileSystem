#ifndef _EDIT_SCENE_
#define _EDIT_SCENE_

#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include "DirectoryScene.h"

class EditScene : public cocos2d::Scene, cocos2d::ui::EditBoxDelegate
{
private:
	string content;
	File *file;
	FileSystem *fs;
	cocos2d::ui::EditBox *editBox;
public:
	static EditScene* createScene(FileSystem *fs, File *file);
	virtual bool init();
	void enemyInit(FileSystem *fs, File *file);
	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox *editBox) {};						//开始编辑
	virtual void editBoxTextChanged(cocos2d::ui::EditBox *editBox, const std::string& text);	//编辑框文字改变
	virtual void editBoxReturn(cocos2d::ui::EditBox *editBox);									//结束编辑
	CREATE_FUNC(EditScene);
};




#endif // _EDIT_SCENE_

