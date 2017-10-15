#include "IntroductionScene.h"
#include "HelloWorldScene.h"
#include "CocosGui.h"
#include "cocos-ext.h"
#include "I1.h"
#include "GUI\CCControlExtension\CCControl.h"
#include "GUI\CCControlExtension\CCControlButton.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
Scene* IntroductionScene::createScene()
{
    auto scene = Scene::create();
    auto layer = IntroductionScene::create();
    scene->addChild(layer);
    return scene;
}

bool IntroductionScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto bg0 = Sprite::create("rules.png");
    bg0->setPosition(Vec2(visibleSize / 2));
    this->addChild(bg0, 1); 

    auto bg = Sprite::create("introduction.png");
    bg->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2-40));
    this->addChild(bg, 1);


    auto returnButton = Scale9Sprite::create("return.png");
    returnButton->setContentSize(Size(65, 65));
    auto rbut = ControlButton::create(returnButton);
    rbut->setPosition(Vec2(origin.x + visibleSize.width-130 , origin.y + 65));
    rbut->addTargetWithActionForControlEvents(this, cccontrol_selector(IntroductionScene::ReturnCallBack), Control::EventType::TOUCH_UP_INSIDE);
    this->addChild(rbut, 2);

    auto rightButton = Scale9Sprite::create("Right.png");
    rightButton->setContentSize(Size(65, 65));
    auto rightbut = ControlButton::create(rightButton);
    rightbut->setPosition(Vec2(origin.x + visibleSize.width - 65, origin.y + 65));
    rightbut->addTargetWithActionForControlEvents(this, cccontrol_selector(IntroductionScene::from0to1), Control::EventType::TOUCH_UP_INSIDE);
    this->addChild(rightbut, 2);

    return true;
}

void IntroductionScene::ReturnCallBack(Ref* pSender, Control::EventType type)
{
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4, scene));
}
void IntroductionScene::from0to1(cocos2d::Ref* pSender, Control::EventType type)
{
    auto scene = Intro1::createScene();
    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.4, scene));
}