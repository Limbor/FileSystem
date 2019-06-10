#include "EditScene.h"

USING_NS_CC;

EditScene * EditScene::createScene(FileSystem * fs, File *file)
{
	EditScene* enemy = new EditScene();
	if (enemy && enemy->init())
	{
		enemy->autorelease();
		enemy->enemyInit(fs, file);
		return enemy;
	}
	CC_SAFE_DELETE(enemy);
	return NULL;
}

bool EditScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	return true;
}

void EditScene::enemyInit(FileSystem * fs, File *file)
{
	this->fs = fs;
	this->file = file;
	oldContent = fs->getContent(file);

	auto background = LayerColor::create(Color4B::WHITE);
	this->addChild(background);

	auto title = Label::createWithSystemFont(this->file->getName(), "Arial", 40);
	title->setTextColor(Color4B::GRAY);
	title->setPosition(512, 750);
	this->addChild(title);
	auto line1 = DrawNode::create();
	line1->drawLine(Vec2(0, 730), Vec2(1024, 730), Color4F::BLACK);
	this->addChild(line1);
	auto line2 = DrawNode::create();
	line2->drawLine(Vec2(0, 768), Vec2(1024, 768), Color4F::BLACK);
	this->addChild(line2);

	editContent = oldContent;
	editBox = ui::EditBox::create(Size(800, 600), "editbox.png");
	editBox->setText(oldContent.c_str());
	editBox->setInputMode(ui::EditBox::InputMode::ANY);
	editBox->setFontName("Arial");
	editBox->setPosition(Vec2(450, 400));
	editBox->setFontSize(25);
	editBox->setFontColor(Color4B::BLACK);
	editBox->setDelegate(this);
	this->addChild(editBox);

	auto hint = Label::createWithSystemFont("Click to view the contents of the file",
		"Arial", 40);
	hint->setTextColor(Color4B::GRAY);
	hint->setPosition(450, 400);
	this->addChild(hint);

	auto save = ui::Button::create("save.png");
	save->setPosition(Vec2(950, 200));
	save->addClickEventListener([&](Ref *psender) {
		this->fs->setContent(this->file, this->editContent);
		auto scene = DirectoryScene::createScene(this->fs);
		Director::getInstance()->replaceScene(scene);
	});
	this->addChild(save);
	auto label1 = Label::createWithSystemFont("Save", "Arial", 18);
	label1->setTextColor(Color4B::BLACK);
	label1->setPosition(950, 140);
	this->addChild(label1);

	auto cancel = ui::Button::create("cancel.png");
	cancel->setPosition(Vec2(950, 70));
	cancel->addClickEventListener([&](Ref *psender) {
		this->fs->setContent(this->file, this->oldContent);
		auto scene = DirectoryScene::createScene(this->fs);
		Director::getInstance()->replaceScene(scene);
	});
	this->addChild(cancel);
	auto label2 = Label::createWithSystemFont("Cancel", "Arial", 18);
	label2->setTextColor(Color4B::BLACK);
	label2->setPosition(950, 10);
	this->addChild(label2);
}

void EditScene::editBoxTextChanged(cocos2d::ui::EditBox * editBox, const std::string & text)
{
	this->editContent = text;
}

void EditScene::editBoxReturn(cocos2d::ui::EditBox * editBox)
{
}
