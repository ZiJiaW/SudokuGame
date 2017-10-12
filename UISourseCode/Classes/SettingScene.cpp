#include "IntroductionScene.h"
#include "HelloWorldScene.h"
#include "SettingScene.h"
#include "CocosGui.h"
#include "cocos-ext.h"
#include "GUI\CCControlExtension\CCControl.h"
#include "GUI\CCControlExtension\CCControlButton.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
Scene* SettingScene::createScene()
{
    auto scene = Scene::create();
    auto layer = SettingScene::create();
    scene->addChild(layer);
    return scene;
}

bool SettingScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto testLabel = Label::createWithTTF("Setting of Sudoku: ......", "fonts/Marker Felt.ttf", 60);
    testLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 0.75f*visibleSize.height));
    this->addChild(testLabel, 1);

    auto returnLabel = Label::createWithTTF("return", "fonts/Marker Felt.ttf", 40);
    auto returnItem = MenuItemLabel::create(returnLabel, CC_CALLBACK_1(SettingScene::ReturnCallBack, this));
    returnItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 2.5f*returnItem->getContentSize().height));
    auto menu = Menu::create(returnItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    return true;
}

void SettingScene::ReturnCallBack(Ref* pSender)
{
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4, scene));
}