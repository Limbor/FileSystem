#include "DirectoryScene.h"
#include "SimpleAudioEngine.h"
#include "EditScene.h"

USING_NS_CC;

DirectoryScene* DirectoryScene::createScene(FileSystem *fs)
{
	DirectoryScene* enemy = new DirectoryScene();
	if (enemy && enemy->init())
	{
		enemy->autorelease();
		enemy->enemyInit(fs);
		return enemy;
	}
	CC_SAFE_DELETE(enemy);
	return NULL;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool DirectoryScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    return true;
}

void DirectoryScene::enemyInit(FileSystem * fs)
{
	if (fs == NULL) this->fs = new FileSystem();
	else this->fs = fs;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto background = LayerColor::create(Color4B::WHITE);
	this->addChild(background);

	initDirectory();
	initFile();

	
	auto title = Label::createWithSystemFont(this->fs->getPath(), "Arial", 40);
	title->setTextColor(Color4B::GRAY);
	title->setPosition(400, 750);
	this->addChild(title);
	auto line1 = DrawNode::create();
	line1->drawLine(Vec2(0, 730), Vec2(824, 730), Color4F::BLACK);
	this->addChild(line1);
	auto line2 = DrawNode::create();
	line2->drawLine(Vec2(0, 768), Vec2(824, 768), Color4F::BLACK);
	this->addChild(line2);

	createMenu();

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = CC_CALLBACK_1(DirectoryScene::onMouseDown, this);
	dispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void DirectoryScene::initDirectory()
{
	std::vector<std::string> folderNames = fs->folderNames();
	if (folderNames.empty()) return;
	std::vector<std::string>::iterator i;
	for (i = folderNames.begin(); i != folderNames.end(); i++) {
		createFolder(*i);
	}
}

void DirectoryScene::initFile()
{
	std::vector<std::string> fileNames = fs->fileNames();
	if (fileNames.empty()) return;
	std::vector<std::string>::iterator i;
	for (i = fileNames.begin(); i != fileNames.end(); i++) {
		createFile(*i);
	}
}

void DirectoryScene::createMenu()
{
	auto menu = DrawNode::create();
	menu->drawRect(Vec2(0, 0), Vec2(200, 768), Color4F::BLACK);
	menu->setPosition(Vec2(824, 0));
	menu->setTag(0);

	auto label = Label::createWithSystemFont("Menu", "Arial", 25);
	label->setTextColor(Color4B::GRAY);
	label->setPosition(50, 750);
	menu->addChild(label);
	auto line = DrawNode::create();
	line->drawLine(Vec2(824, 730), Vec2(1024, 730), Color4F::BLACK);
	this->addChild(line);

	auto newFolder = ui::Button::create("nFolder.png");
	newFolder->setPosition(Vec2(47, 690));
	newFolder->addClickEventListener([&](Ref *pSender) {
		if (editName != NULL) {
			editName->removeFromParent();
			createEditNameBox();
		}
		if (hasEditedName) {
			createFolder(this->currentName);
			fs->createSubdirectory(this->currentName);
			hasEditedName = false;
		}
	});
	menu->addChild(newFolder);
	auto label1 = Label::createWithSystemFont("New Folder", "Arial", 18);
	label1->setTextColor(Color4B::BLACK);
	label1->setPosition(47, 640);
	menu->addChild(label1);

	auto newFile = ui::Button::create("nFile.png");
	newFile->setPosition(Vec2(145, 690));
	newFile->addClickEventListener([&](Ref *pSender) {
		if (editName != NULL) {
			editName->removeFromParent();
			createEditNameBox();
		}
		if (hasEditedName) {
			createFile(this->currentName);
			fs->cteateNewFile(this->currentName);
			hasEditedName = false;
		}
	});
	menu->addChild(newFile);
	auto label2 = Label::createWithSystemFont("New File", "Arial", 18);
	label2->setTextColor(Color4B::BLACK);
	label2->setPosition(145, 640);
	menu->addChild(label2);

	auto openFolder = ui::Button::create("oFolder.png");
	openFolder->setPosition(Vec2(47, 510));
	openFolder->addClickEventListener([&](Ref *psender) {
		if (selected != NULL) {
			int tag = selected->getTag();
			int i = (tag - 1) / 5;
			int j = tag % 5 - 1;
			if (content[i][j] == FOLDER) {
				fs->current = fs->current->searchFolder(selected->getName());
				auto scene = DirectoryScene::createScene(this->fs);
				Director::getInstance()->replaceScene(scene);
			}
		}
	});
	menu->addChild(openFolder);
	auto label3 = Label::createWithSystemFont(" Open \nFolder", "Arial", 18);
	label3->setTextColor(Color4B::BLACK);
	label3->setPosition(50, 440);
	menu->addChild(label3);

	auto openFile = ui::Button::create("oFile.png");
	openFile->setPosition(Vec2(145, 510));
	openFile->addClickEventListener([&](Ref *psender) {
		if (selected != NULL) {
			int tag = selected->getTag();
			int i = (tag - 1) / 5;
			int j = tag % 5 - 1;
			if (content[i][j] == FILE) {
				File *file = fs->current->searchFile(selected->getName());
				auto scene = EditScene::createScene(this->fs, file);
				Director::getInstance()->replaceScene(scene);
			}
		}
	});
	menu->addChild(openFile);
	auto label4 = Label::createWithSystemFont("Open\nFile", "Arial", 18);
	label4->setTextColor(Color4B::BLACK);
	label4->setPosition(145, 440);
	menu->addChild(label4);

	auto deleteFolder = ui::Button::create("dFolder.png");
	deleteFolder->setPosition(Vec2(45, 360));
	deleteFolder->addClickEventListener([&](Ref *psender) {
		if (selected != NULL) {
			int tag = selected->getTag();
			int i = (tag - 1) / 5;
			int j = tag % 5 - 1;
			if (content[i][j] == FOLDER) {
				fs->deleteSubdirectory(selected->getName());
				selected->removeFromParent();
				selected = NULL;
				content[i][j] = 0;
			}
		}
	});
	menu->addChild(deleteFolder);
	auto label5 = Label::createWithSystemFont("Delete\nFolder", "Arial", 18);
	label5->setTextColor(Color4B::BLACK);
	label5->setPosition(45, 290);
	menu->addChild(label5);

	auto deleteFile = ui::Button::create("dFile.png");
	deleteFile->setPosition(Vec2(145, 360));
	deleteFile->addClickEventListener([&](Ref *psender) {
		if (selected != NULL) {
			int tag = selected->getTag();
			int i = (tag - 1) / 5;
			int j = tag % 5 - 1;
			if (content[i][j] == FILE) {
				fs->deleteFile(selected->getName());
				selected->removeFromParent();
				selected = NULL;
				content[i][j] = 0;
			}
		}
	});
	menu->addChild(deleteFile);
	auto label6 = Label::createWithSystemFont("Delete\n File", "Arial", 18);
	label6->setTextColor(Color4B::BLACK);
	label6->setPosition(145, 290);
	menu->addChild(label6);

	auto formatCurrent = ui::Button::create("clean.png");
	formatCurrent->setPosition(Vec2(45, 210));
	formatCurrent->addClickEventListener([&](Ref *psender) {
		this->fs->format(fs->current);
		auto scene = DirectoryScene::createScene(this->fs);
		Director::getInstance()->replaceScene(scene);
	});
	menu->addChild(formatCurrent);
	auto label7 = Label::createWithSystemFont(" Clean \nCurrent", "Arial", 18);
	label7->setTextColor(Color4B::BLACK);
	label7->setPosition(45, 140);
	menu->addChild(label7);

	auto formatAll = ui::Button::create("cleanup.png");
	formatAll->setPosition(Vec2(145, 210));
	formatAll->addClickEventListener([&](Ref *psender) {
		this->fs->format(fs->root);
		this->fs->current = this->fs->root;
		auto scene = DirectoryScene::createScene(this->fs);
		Director::getInstance()->replaceScene(scene);
	});
	menu->addChild(formatAll);
	auto label8 = Label::createWithSystemFont("Clean\n All", "Arial", 18);
	label8->setTextColor(Color4B::BLACK);
	label8->setPosition(145, 140);
	menu->addChild(label8);

	auto refresh = ui::Button::create("refresh.png");
	refresh->setPosition(Vec2(45, 70));
	refresh->addClickEventListener([&](Ref *psender) {
		auto scene = DirectoryScene::createScene(this->fs);
		Director::getInstance()->replaceScene(scene);
	});
	menu->addChild(refresh);
	auto label9 = Label::createWithSystemFont("Refresh", "Arial", 18);
	label9->setTextColor(Color4B::BLACK);
	label9->setPosition(45, 20 );
	menu->addChild(label9);

	auto back = ui::Button::create("return.png");
	back->setPosition(Vec2(145, 70));
	back->addClickEventListener([&](Ref *psender) {
		if (fs->back() != NULL) {
			fs->current = fs->back();
			auto scene = DirectoryScene::createScene(this->fs);
			Director::getInstance()->replaceScene(scene);
		}
	});
	menu->addChild(back);
	auto label10 = Label::createWithSystemFont("Return", "Arial", 18);
	label10->setTextColor(Color4B::BLACK);
	label10->setPosition(145, 20);
	menu->addChild(label10);

	this->addChild(menu);
	createEditNameBox();
}

void DirectoryScene::createEditNameBox()
{
	editName = ui::EditBox::create(Size(160, 40), "edit.png");
	editName->setFontName("Arial");
	editName->setPosition(Vec2(100, 600));
	editName->setFontSize(18);
	editName->setFontColor(Color4B::BLACK);
	editName->setPlaceholderFontName("Arial");
	editName->setPlaceholderFontSize(18);
	editName->setPlaceHolder("Input Name");
	editName->setPlaceholderFontColor(Color4B::RED);
	editName->setMaxLength(10);
	editName->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	editName->setDelegate(this);
	DrawNode *m = (DrawNode*)this->getChildByTag(0);
	m->addChild(editName);
}

void DirectoryScene::onMouseDown(Event *event)
{
	EventMouse *e = (EventMouse*)event;
	Vec2 point = e->getLocationInView();
	/*if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
		if (isEditingName) return;
		if (!hasMenu) {
			auto menu = DrawNode::create();
			menu->drawRect(Vec2(0, 0), Vec2(200, 350), Color4F::BLACK);
			menu->setPosition(point - Vec2(0, 350));
			menu->setTag(0);
			auto newFolder = ui::Button::create("nFolder.png");
			newFolder->setPosition(Vec2(100, 330));
			newFolder->addClickEventListener(CC_CALLBACK_1(DirectoryScene::newFolderCallBack, this));
			menu->addChild(newFolder);
			this->hasMenu = true;
			this->addChild(menu);
		}
		else {
			DrawNode *d = (DrawNode*)getChildByTag(0);
			d->setPosition(point -= Vec2(0, 350));
		}
	}*/
	int x = point.x;
	int y = point.y;
	int i = (670 - y) / 150;
	int j = (x - 50) / 150;
	if (i >= 5 || j >= 5) return;
	if (content[i][j] != 0) {
		if (selected != NULL) selected->setScale((float)0.80);
		int tag = j + 1 + i * 5;
		Sprite *s = (Sprite*)getChildByTag(tag);
		s->setScale(1.25);
		selected = s;
	}
}

void DirectoryScene::createFolder(std::string name)
{
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (this->content[i][j] == 0) {
				this->content[i][j] = FOLDER;
				auto folder = Sprite::create("folder.png");
				folder->setPosition(100 + j * 150, 670 - i * 150);
				folder->setName(name);
				folder->setTag(j + 1 + i * 5);
				auto txtName = Label::createWithSystemFont(name, "Arial", 20);
				txtName->setTextColor(Color4B::BLACK);
				txtName->setPosition(65, 10);
				folder->addChild(txtName);
				this->addChild(folder);
				return;
			}
		}
	}
}

void DirectoryScene::createFile(std::string name)
{
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (this->content[i][j] == 0) {
				this->content[i][j] = FILE;
				auto file = Sprite::create("file.png");
				file->setPosition(100 + j * 150, 670 - i * 150);
				file->setName(name);
				file->setTag(j + 1 + i * 5);
				auto txtName = Label::createWithSystemFont(name, "Arial", 20);
				txtName->setTextColor(Color4B::BLACK);
				txtName->setPosition(65, 10);
				file->addChild(txtName);
				this->addChild(file);
				return;
			}
		}
	}
}

void DirectoryScene::editBoxTextChanged(ui::EditBox * editBox, const std::string & text)
{
	this->currentName = text;
}

void DirectoryScene::editBoxReturn(ui::EditBox * editBox)
{
	this->hasEditedName = true;
}
