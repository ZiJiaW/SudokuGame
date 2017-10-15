#pragma once
#ifndef INTRO_SCENE_H
#define INTRO_SCENE_H
#include "cocos2d.h"
#include "IntroductionScene.h"
#include "HelloWorldScene.h"
#include "CocosGui.h"
#include "cocos-ext.h"
#include "GUI\CCControlExtension\CCControl.h"
#include "GUI\CCControlExtension\CCControlButton.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
class IntroductionScene :public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    // return item callback
    void ReturnCallBack(cocos2d::Ref* pSender, Control::EventType type);
    void from0to1(cocos2d::Ref* pSender, Control::EventType type);

    CREATE_FUNC(IntroductionScene);
};

#endif // !

