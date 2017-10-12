#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "CocosGui.h"
#include "cocos-ext.h"
#include "GUI\CCControlExtension\CCControl.h"
#include "GUI\CCControlExtension\CCControlButton.h"
#include "IntroductionScene.h"
#include "SettingScene.h"
#include "GameScene.h"
#include "Global.h"
USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocos2d::extension;

const char* startGame = "Start Game";
const char* continueGame = "Continue";
const char* settings = "Settings";
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

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    /*auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));*/
    ////////////////////////////////////my menu
    auto stLabel = Label::createWithTTF(startGame, "fonts/Marker Felt.ttf", 40);
    auto ctLabel = Label::createWithTTF(continueGame, "fonts/Marker Felt.ttf", 40);
    auto seLabel = Label::createWithTTF(settings, "fonts/Marker Felt.ttf", 40);
    auto inLabel = Label::createWithTTF(introduction, "fonts/Marker Felt.ttf", 40);
    auto exLabel = Label::createWithTTF(exitt, "fonts/Marker Felt.ttf", 40);

    auto itemSt = MenuItemLabel::create(stLabel, CC_CALLBACK_1(HelloWorld::StartGameCallBack, this));
    auto itemCt = MenuItemLabel::create(ctLabel, CC_CALLBACK_1(HelloWorld::ContinueCallBack, this));
    auto itemSe = MenuItemLabel::create(seLabel, CC_CALLBACK_1(HelloWorld::SettingCallBack, this));
    auto itemIn = MenuItemLabel::create(inLabel, CC_CALLBACK_1(HelloWorld::IntroductionCallBack, this));
    auto itemEx = MenuItemLabel::create(exLabel, CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    itemSt->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 10.5f*itemSt->getContentSize().height));
    itemCt->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 8.5f*itemSt->getContentSize().height));
    itemSe->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 6.5f*itemSt->getContentSize().height));
    itemIn->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 4.5f*itemSt->getContentSize().height));
    itemEx->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 2.5f*itemSt->getContentSize().height));

    // create menu, it's an autorelease object
    auto menu = Menu::create(itemSt, itemCt, itemSe, itemIn, itemEx, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...
    //try save
    /*std::string storedPro("590602083023000670678139005215086407309010568067905312731800000952701830406590701");
    CCUserDefault::sharedUserDefault()->setStringForKey("puzzle", storedPro);
    CCUserDefault::sharedUserDefault()->flush();//±£´æ*/
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Sudoku Game 1.0", "fonts/Marker Felt.ttf", 80);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    
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

}
void HelloWorld::ContinueCallBack(Ref* pSender)
{
   // auto savedPuzzle = CCUserDefault::sharedUserDefault()->getStringForKey("puzzle").c_str();
    if (CCUserDefault::sharedUserDefault()->getStringForKey("puzzle").c_str())
    {
        for (int i = 0; i < 81; i++)
        {
            ShareGlobal()->grid[i / 9][i % 9].num = CCUserDefault::sharedUserDefault()->getStringForKey("puzzle").c_str()[i] - '0';
            int a = ShareGlobal()->grid[i / 9][i % 9].num;
            if (a != 0)
                ShareGlobal()->grid[i / 9][i % 9].isGiven = true;
            else
                ShareGlobal()->grid[i / 9][i % 9].isGiven = false;
        }
        auto scene = GameScene::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4, scene));
    }
}
void HelloWorld::SettingCallBack(Ref* pSender)
{
    auto scene = SettingScene::createScene();
    Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4, scene));
}
void HelloWorld::IntroductionCallBack(Ref* pSender)
{
    auto scene = IntroductionScene::createScene();
    Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4, scene));
}