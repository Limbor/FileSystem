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

	editBox = ui::EditBox::create(Size(800, 600), "editbox.png");
	editBox->setText(fs->getContent(file).c_str());
	editBox->setInputMode(ui::EditBox::InputMode::ANY);
	editBox->setFontName("Arial");
	editBox->setPosition(Vec2(450, 400));
	editBox->setFontSize(25);
	editBox->setFontColor(Color4B::BLACK);
	editBox->setDelegate(this);
	this->addChild(editBox);

	auto back = ui::Button::create("return.png");
	back->setPosition(Vec2(900, 70));
	back->addClickEventListener([&](Ref *psender) {
		this->fs->setContent(this->file, this->content);
		auto scene = DirectoryScene::createScene(this->fs);
		Director::getInstance()->replaceScene(scene);
	});
	this->addChild(back);
}

void EditScene::editBoxTextChanged(cocos2d::ui::EditBox * editBox, const std::string & text)
{
	this->content = text;
}

void EditScene::editBoxReturn(cocos2d::ui::EditBox * editBox)
{
}
