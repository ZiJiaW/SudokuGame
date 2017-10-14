#pragma once
#include "IntroductionScene.h"
#include "HelloWorldScene.h"
#include "CocosGui.h"
#include "cocos-ext.h"
#include "GUI\CCControlExtension\CCControl.h"
#include "GUI\CCControlExtension\CCControlButton.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
class Intro4 :public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    // return item callback
    void ReturnCallBack(cocos2d::Ref* pSender, Control::EventType type);
    void from4to3(cocos2d::Ref* pSender, Control::EventType type);
    void from4to5(cocos2d::Ref* pSender, Control::EventType type);
    CREATE_FUNC(Intro4);
};