#include "IntroductionScene.h"
#include "HelloWorldScene.h"
#include "CocosGui.h"
#include "cocos-ext.h"
#include "GameScene.h"
#include "GUI\CCControlExtension\CCControl.h"
#include "GUI\CCControlExtension\CCControlButton.h"
#include "Global.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

bool GameScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    // 初始化私有变量
    selected = -1;
    for (int i = 0; i < 80; i++)
    {
        changed[i] = false;
    }
    // 获得界面尺寸
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // 显示棋盘 tag 100-180  背景精灵 200-280
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            int gridnum = ShareGlobal()->grid[i][j].num;
            if (gridnum != 0)
            {
                char s[2];
                s[0] = '0' + gridnum;
                s[1] = '\0';
                std::string temp(s);
                auto grid = Label::createWithTTF(temp, "fonts/Marker Felt.ttf", 40);
                grid->setPosition(Vec2(origin.x + 100 + (j + 0.5)*65.0f,
                    origin.y + visibleSize.height - 100 - (i + 0.5)*65.0f));
                auto gridbg = Sprite::create("green.jpg");
                gridbg->setContentSize(Size(65, 65));
                gridbg->setPosition(Vec2(origin.x + 100 + (j + 0.5)*65.0f,
                    origin.y + visibleSize.height - 100 - (i + 0.5)*65.0f));
                gridbg->setColor(Color3B(51, 51, 255));
                this->addChild(gridbg, 1, 200 + 9 * i + j);
                this->addChild(grid, 2, 100 + 9 * i + j);
            }
            else
            {
                auto grid = Label::createWithTTF("", "fonts/Marker Felt.ttf", 40);
                grid->setPosition(Vec2(origin.x + 100 + (j + 0.5)*65.0f,
                    origin.y + visibleSize.height - 100 - (i + 0.5)*65.0f));
                auto gridbg = Sprite::create("green.jpg");
                gridbg->setContentSize(Size(65, 65));
                gridbg->setPosition(Vec2(origin.x + 100 + (j + 0.5)*65.0f,
                    origin.y + visibleSize.height - 100 - (i + 0.5)*65.0f));
                gridbg->setColor(Color3B(0, 255, 255));
                this->addChild(gridbg, 1, 200 + 9 * i + j);
                this->addChild(grid, 2, 100 + 9 * i + j);
            }
        }
    }
    // 显示迷你键盘 tag 300-308
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            auto button = Scale9Sprite::create("button.png");
            button->setContentSize(Size(65,65));
            auto numbut = ControlButton::create(button);
            numbut->setPosition(Vec2(origin.x + visibleSize.width - 30 - (2.5 - j)*65.0f, origin.y + visibleSize.height - 100 - (i + 6.5)*65.0f));
            numbut->addTargetWithActionForControlEvents(this, cccontrol_selector(GameScene::NumbutCallBack), Control::EventType::TOUCH_UP_INSIDE);
            this->addChild(numbut, 1, 300 + 3 * i + j);
            std::string temp(1, char('0' + 3 * i + j + 1));
            auto numlabel = Label::createWithTTF(temp, "fonts/Marker Felt.ttf", 40);
            numlabel->setPosition(Vec2(origin.x + visibleSize.width - 30 - (2.5 - j)*65.0f, origin.y + visibleSize.height - 100 - (i + 6.5)*65.0f));
            this->addChild(numlabel, 2);
        }
    }
    // 菜单设置：清空按钮,返回按钮，保存按钮
    auto clLabel = Label::createWithTTF("Clear", "fonts/Marker Felt.ttf", 40);
    auto clItem = MenuItemLabel::create(clLabel, CC_CALLBACK_1(GameScene::ClearCallBack, this));
    clItem->setPosition(Vec2(origin.x + visibleSize.width - 180, origin.y + 400));

    auto returnLabel = Label::createWithTTF("return", "fonts/Marker Felt.ttf", 40);
    auto returnItem = MenuItemLabel::create(returnLabel, CC_CALLBACK_1(GameScene::ReturnCallBack, this));
    returnItem->setPosition(Vec2(origin.x + visibleSize.width - 180, origin.y + 340));

    auto saveLabel = Label::createWithTTF("Save", "fonts/Marker Felt.ttf", 40);
    auto saveItem = MenuItemLabel::create(saveLabel, CC_CALLBACK_1(GameScene::SaveCallBack, this));
    saveItem->setPosition(Vec2(origin.x + visibleSize.width - 180, origin.y + 460));

    Menu* menu = Menu::create(clItem,returnItem,saveItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    // 设置鼠标事件监听者
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = [=](Event* event)
    {
        EventMouse* e = (EventMouse*)event;
        auto mousepos = e->getLocation();
        mousepos.y = visibleSize.height - mousepos.y;
        for (int i = 0; i < 80; i++)
        {
            Sprite* s = (Sprite*)this->getChildByTag(200 + i);
            if (s->getBoundingBox().containsPoint(mousepos))
            {
                selected = i;
                if (!ShareGlobal()->grid[i / 9][i % 9].isGiven)
                {
                    if (!changed[i])
                    {
                        s->setColor(Color3B(51, 102, 153));
                        changed[i] = true;
                    }
                    else
                    {
                        s->setColor(Color3B(0, 255, 255));
                        changed[i] = false;
                    }
                }
            }
        }
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);
    // 设置键盘监听者
  /*  auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)
    {
        if (char(keyCode) >= '1'&&char(keyCode) <= '9'&&!ShareGlobal()->grid[selected / 9][selected % 9].isGiven)
        {
            Label* grid = (Label*)this->getChildByTag(100 + selected);
            grid->setString(std::string(1, char(keyCode)));
        }
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
    */
    return true;
}
// 填数按钮响应函数
void GameScene::NumbutCallBack(Ref* pSender, Control::EventType type)
{
    if (selected != -1 && !ShareGlobal()->grid[selected / 9][selected % 9].isGiven)
    {
        int i;
        for (i = 0; i < 9; i++)
        {
            ControlButton* t = (ControlButton*)this->getChildByTag(300 + i);
            if (t == pSender)
                break;
        }
        Label* grid = (Label*)this->getChildByTag(100 + selected);
        grid->setString(std::string(1, char('0' + i + 1)));
        ShareGlobal()->grid[selected / 9][selected % 9].num = i + 1;
    }
}
// 清空已填格子
void GameScene::ClearCallBack(Ref* pSender)
{
    for (int i = 0; i < 80; i++)
    {
        if (!ShareGlobal()->grid[i / 9][i % 9].isGiven)
        {
            Label* numLabel = (Label*)this->getChildByTag(100 + i);
            numLabel->setString("");
            ShareGlobal()->grid[i / 9][i % 9].num = 0;
        }
    }
}
// 返回主菜单
void GameScene::ReturnCallBack(Ref* Sender)
{
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4, scene));
}
// 保存游戏
void GameScene::SaveCallBack(Ref* pSender)
{
    std::string temp;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            temp += char(ShareGlobal()->grid[i][j].num + '0');
        }
    }
    CCUserDefault::sharedUserDefault()->setStringForKey("puzzle", temp);
    CCUserDefault::sharedUserDefault()->flush();
}