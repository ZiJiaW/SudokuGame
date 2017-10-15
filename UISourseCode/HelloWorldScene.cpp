#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "CocosGui.h"
#include "cocos-ext.h"
#include "GUI\CCControlExtension\CCControl.h"
#include "GUI\CCControlExtension\CCControlButton.h"
#include "IntroductionScene.h"
#include "GameScene.h"
#include "Global.h"
USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
using namespace CocosDenshion;
const char* startGame = "New Game";
const char* continueGame = "Load Game";
const char* introduction = "Introduction";
const char* exitt = "Exit";

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // bgm预加载
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("bg.wav");
    // 默认开启背景音乐
    if(!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
        SimpleAudioEngine::getInstance()->playBackgroundMusic("bg.wav", true);
    // 加载最佳记录时间
    if (!(ShareGlobal()->bestRecordEasy = CCUserDefault::sharedUserDefault()->getIntegerForKey("bestRecordEasy")))
    {
        ShareGlobal()->bestRecordEasy = -1;// -1 表示还没有记录
    }
    if (!(ShareGlobal()->bestRecordMedium = CCUserDefault::sharedUserDefault()->getIntegerForKey("bestRecordMedium")))
    {
        ShareGlobal()->bestRecordMedium = -1;// -1 表示还没有记录
    }
    if (!(ShareGlobal()->bestRecordHard = CCUserDefault::sharedUserDefault()->getIntegerForKey("bestRecordHard")))
    {
        ShareGlobal()->bestRecordHard = -1;// -1 表示还没有记录
    }
    
    // add background
    
    auto bg = Sprite::create("background.jpg");
    bg->setPosition(Vec2(visibleSize / 2));
    bg->setContentSize(Size(1024, 768));
    this->addChild(bg, 0);
    
    ////////////////////////////////////my menu
    auto stLabel = Label::createWithTTF(startGame, "fonts/Marker Felt.ttf", 50);
    auto ctLabel = Label::createWithTTF(continueGame, "fonts/Marker Felt.ttf", 50);
    auto inLabel = Label::createWithTTF(introduction, "fonts/Marker Felt.ttf", 50);
    auto exLabel = Label::createWithTTF(exitt, "fonts/Marker Felt.ttf", 50);
    stLabel->setColor(Color3B(0, 0, 0));
    ctLabel->setColor(Color3B(0, 0, 0));
    inLabel->setColor(Color3B(0, 0, 0));
    exLabel->setColor(Color3B(0, 0, 0));
    auto itemSt = MenuItemLabel::create(stLabel, CC_CALLBACK_1(HelloWorld::StartGameCallBack, this));
    auto itemCt = MenuItemLabel::create(ctLabel, CC_CALLBACK_1(HelloWorld::ContinueCallBack, this));
    auto itemIn = MenuItemLabel::create(inLabel, CC_CALLBACK_1(HelloWorld::IntroductionCallBack, this));
    auto itemEx = MenuItemLabel::create(exLabel, CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    itemSt->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 8.5f*itemSt->getContentSize().height));
    itemCt->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 6.5f*itemSt->getContentSize().height));
    itemIn->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 4.5f*itemSt->getContentSize().height));
    itemEx->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 2.5f*itemSt->getContentSize().height));


    // create menu, it's an autorelease object
    auto menu = Menu::create(itemSt, itemCt, itemIn, itemEx, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    // print record 给出各难度最佳记录
    std::string stmp;
    char ctmp[6];
    stmp += "Easy: ";
    if (ShareGlobal()->bestRecordEasy == -1)
    {
        stmp += "no record";
    }
    else
    {
        itoa(ShareGlobal()->bestRecordEasy, ctmp, 10);
        stmp += ctmp;
        stmp += " s";
    }

    auto test = Label::createWithTTF("Best Record", "fonts/Marker Felt.ttf", 30);
    test->setColor(Color3B::BLACK);
    test->setPosition(900,200);
    this->addChild(test, 2);

    auto recordLabelEasy = Label::createWithTTF(stmp.c_str(), "fonts/Marker Felt.ttf",30);
    recordLabelEasy->setColor(Color3B::BLACK);
    recordLabelEasy->setPosition(origin.x + 900, origin.y + 150);
    this->addChild(recordLabelEasy, 1);

    stmp.clear();
    stmp += "Medium: ";
    if (ShareGlobal()->bestRecordMedium == -1)
    {
        stmp += "no record";
    }
    else
    {
        itoa(ShareGlobal()->bestRecordMedium, ctmp, 10);
        stmp += ctmp;
        stmp += " s";
    }
    auto recordLabelMedium = Label::createWithTTF(stmp.c_str(), "fonts/Marker Felt.ttf",30);
    recordLabelMedium->setColor(Color3B::BLACK);
    recordLabelMedium->setPosition(origin.x + 900, origin.y + 100);
    this->addChild(recordLabelMedium, 1);

    stmp.clear();
    stmp += "Hard: ";
    if (ShareGlobal()->bestRecordHard == -1)
    {
        stmp += "no record";
    }
    else
    {
        itoa(ShareGlobal()->bestRecordHard, ctmp, 10);
        stmp += ctmp;
        stmp += " s";
    }
    auto recordLabelHard = Label::createWithTTF(stmp.c_str(), "fonts/Marker Felt.ttf",30);
    recordLabelHard->setColor(Color3B::BLACK);
    recordLabelHard->setPosition(origin.x + 900, origin.y + 50);
    this->addChild(recordLabelHard, 1);

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

// now define the callback functions
void HelloWorld::StartGameCallBack(Ref* pSender)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // 弹出窗口选择难度：HARD MEDIUM EASY 以及取消按钮
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this, true);
    auto colorLay = LayerColor::create(Color4B(51, 51, 255, 200));
    colorLay->ignoreAnchorPointForPosition(false);
    colorLay->setAnchorPoint(Vec2(0.5, 0.5));
    colorLay->changeHeight(visibleSize.height / 2);
    colorLay->changeWidth(visibleSize.width / 2);
    colorLay->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->addChild(colorLay, 3);

    auto cancelLabel = Label::createWithTTF("Cancel", "fonts/Marker Felt.ttf", 40);
    auto cancelItem = MenuItemLabel::create(cancelLabel, [=](Ref* pSender) {
        colorLay->removeFromParent();
        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this, true);
    });
    cancelItem->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height / 4 - 120));

    auto easyLabel = Label::createWithTTF("Easy", "fonts/Marker Felt.ttf", 40);
    auto easyItem = MenuItemLabel::create(easyLabel, [=](Ref* pSender) {
        ShareGlobal()->degree = 1;
        ShareGlobal()->GenerateGame();
        ShareGlobal()->time = 0;
        auto scene = GameScene::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4, scene));
    });
    easyItem->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height / 4 + 120));

    auto mediumLabel = Label::createWithTTF("Normal", "fonts/Marker Felt.ttf", 40);
    auto mediumItem = MenuItemLabel::create(mediumLabel, [=](Ref* pSender) {
        ShareGlobal()->degree = 2;
        ShareGlobal()->GenerateGame();
        ShareGlobal()->time = 0;
        auto scene = GameScene::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4, scene));
    });
    mediumItem->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height / 4 + 40));

    auto hardLabel = Label::createWithTTF("Hard", "fonts/Marker Felt.ttf", 40);
    auto hardItem = MenuItemLabel::create(hardLabel, [=](Ref* pSender) {
        ShareGlobal()->degree = 3;
        ShareGlobal()->GenerateGame();
        ShareGlobal()->time = 0;
        auto scene = GameScene::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4, scene));
    });
    hardItem->setPosition(Vec2(  visibleSize.width / 4,  visibleSize.height / 4 -40));

    auto menu = Menu::create(cancelItem, easyItem, mediumItem, hardItem, NULL);
    menu->setPosition(Vec2(0,0));
    colorLay->addChild(menu, 4);
}
void HelloWorld::ContinueCallBack(Ref* pSender)
{
    if (CCUserDefault::sharedUserDefault()->getIntegerForKey("time"))
    {
        for (int i = 0; i < 81; i++)
        {
            ShareGlobal()->grid[i / 9][i % 9].num = CCUserDefault::sharedUserDefault()->getStringForKey("puzzle").c_str()[i] - '0';
            ShareGlobal()->grid[i / 9][i % 9].isGiven = bool(CCUserDefault::sharedUserDefault()->getStringForKey("given").c_str()[i] - '0');
        }
        ShareGlobal()->degree = CCUserDefault::sharedUserDefault()->getIntegerForKey("degree");
        ShareGlobal()->time = CCUserDefault::sharedUserDefault()->getIntegerForKey("time");
        auto scene = GameScene::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4, scene));
    }
}
void HelloWorld::IntroductionCallBack(Ref* pSender)
{
    auto scene = IntroductionScene::createScene();
    Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4, scene));
}