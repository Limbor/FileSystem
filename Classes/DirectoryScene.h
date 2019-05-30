#ifndef __DIRECTORY_SCENE_H__
#define __DIRECTORY_SCENE_H__

#include "cocos2d.h"

class DirectoryScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(DirectoryScene);
};

#endif // __DIRECTORY_SCENE_H__
