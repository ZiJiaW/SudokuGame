#pragma once
#ifndef INTRO_SCENE_H
#define INTRO_SCENE_H
#include "cocos2d.h"

class IntroductionScene :public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    // return item callback
    void ReturnCallBack(cocos2d::Ref* pSender);

    CREATE_FUNC(IntroductionScene);
};

#endif // !

