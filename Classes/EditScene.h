#ifndef _EDIT_SCENE_
#define _EDIT_SCENE_

#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include "DirectoryScene.h"

class EditScene : public cocos2d::Scene, cocos2d::ui::EditBoxDelegate
{
private:
	string oldContent;						//����������
	string editContent;						//�༭������
	File *file;								//�������ļ�
	FileSystem *fs;							//�ļ�����ϵͳ
	cocos2d::ui::EditBox *editBox;			//�༭��
public:
	static EditScene* createScene(FileSystem *fs, File *file);
	virtual bool init();
	void enemyInit(FileSystem *fs, File *file);
	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox *editBox) {};						//��ʼ�༭
	virtual void editBoxTextChanged(cocos2d::ui::EditBox *editBox, const std::string& text);	//�༭�����ָı�
	virtual void editBoxReturn(cocos2d::ui::EditBox *editBox);									//�����༭
	CREATE_FUNC(EditScene);
};




#endif // _EDIT_SCENE_

