#include "IntroductionScene.h"
#include "HelloWorldScene.h"
#include "CocosGui.h"
#include "cocos-ext.h"
#include "I2.h"
#include "I3.h"
#include "I4.h"
#include "GUI\CCControlExtension\CCControl.h"
#include "GUI\CCControlExtension\CCControlButton.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
Scene* Intro3::createScene()
{
    auto scene = Scene::create();
    auto layer = Intro3::create();
    scene->addChild(layer);
    return scene;
}
bool Intro3::init()
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

    auto bg = Sprite::create("Technique3.png");
    bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 40));
    this->addChild(bg, 1);


    auto returnButton = Scale9Sprite::create("return.png");
    returnButton->setContentSize(Size(65, 65));
    auto rbut = ControlButton::create(returnButton);
    rbut->setPosition(Vec2(origin.x + visibleSize.width - 130, origin.y + 65));
    rbut->addTargetWithActionForControlEvents(this, cccontrol_selector(Intro3::ReturnCallBack), Control::EventType::TOUCH_UP_INSIDE);
    this->addChild(rbut, 2);

    auto rightButton = Scale9Sprite::create("Right.png");
    rightButton->setContentSize(Size(65, 65));
    auto rightbut = ControlButton::create(rightButton);
    rightbut->setPosition(Vec2(origin.x + visibleSize.width - 65, origin.y + 65));
    rightbut->addTargetWithActionForControlEvents(this, cccontrol_selector(Intro3::from3to4), Control::EventType::TOUCH_UP_INSIDE);
    this->addChild(rightbut, 2);

    auto leftButton = Scale9Sprite::create("Left.png");
    leftButton->setContentSize(Size(65, 65));
    auto leftbut = ControlButton::create(leftButton);
    leftbut->setPosition(Vec2(origin.x + visibleSize.width - 195, origin.y + 65));
    leftbut->addTargetWithActionForControlEvents(this, cccontrol_selector(Intro3::from3to2), Control::EventType::TOUCH_UP_INSIDE);
    this->addChild(leftbut, 2);

    return true;
}

void Intro3::ReturnCallBack(Ref* pSender, Control::EventType type)
{
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4, scene));
}
void Intro3::from3to4(cocos2d::Ref* pSender, Control::EventType type)
{
    auto scene = Intro4::createScene();
    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.4, scene));
}
void Intro3::from3to2(cocos2d::Ref* pSender, Control::EventType type)
{
    auto scene = Intro2::createScene();
    Director::getInstance()->replaceScene(TransitionSlideInL::create(0.4, scene));
}