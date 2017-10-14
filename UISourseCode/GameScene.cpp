#include "IntroductionScene.h"
#include "HelloWorldScene.h"
#include "CocosGui.h"
#include "cocos-ext.h"
#include "GameScene.h"
#include "GUI\CCControlExtension\CCControl.h"
#include "GUI\CCControlExtension\CCControlButton.h"
#include "Global.h"
#include <cstdlib>
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
    isSaved = false;
    lastCheck = -1;
    time = ShareGlobal()->time;
    for (int i = 0; i < 80; i++)
    {
        changed[i] = false;
    }
    // 获得界面尺寸
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // 显示计时 tag = 50
    auto timeLabel = Label::createWithTTF("0 s", "fonts/Marker Felt.ttf", 60);
    timeLabel->setPosition(Vec2(origin.x + visibleSize.width - 180, origin.y + 700));
    this->addChild(timeLabel, 1, 50);
    schedule(schedule_selector(GameScene::UpdateTime), 1.0f);
    // 显示棋盘 tag 100-180  背景精灵 200-280
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            int gridnum = ShareGlobal()->grid[i][j].num;
            int isGiven = ShareGlobal()->grid[i][j].isGiven;
            if (gridnum != 0)
            {
                std::string temp(1,char('0'+gridnum));
                auto grid = Label::createWithTTF(temp, "fonts/Marker Felt.ttf", 40);
                grid->setPosition(Vec2(origin.x + 100 + (j + 0.5)*65.0f,
                    origin.y + visibleSize.height - 100 - (i + 0.5)*65.0f));
                auto gridbg = Sprite::create("green.jpg");
                gridbg->setContentSize(Size(65, 65));
                gridbg->setPosition(Vec2(origin.x + 100 + (j + 0.5)*65.0f,
                    origin.y + visibleSize.height - 100 - (i + 0.5)*65.0f));
                if(isGiven)
                    gridbg->setColor(Color3B(51, 51, 255));
                else
                    gridbg->setColor(Color3B(0, 255, 255));
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
    // 菜单设置：清空按钮,返回按钮，保存按钮,check
    auto clLabel = Label::createWithTTF("Clear", "fonts/Marker Felt.ttf", 40);
    auto clItem = MenuItemLabel::create(clLabel, CC_CALLBACK_1(GameScene::ClearCallBack, this));
    clItem->setPosition(Vec2(origin.x + visibleSize.width - 180, origin.y + 400));

    auto returnLabel = Label::createWithTTF("return", "fonts/Marker Felt.ttf", 40);
    auto returnItem = MenuItemLabel::create(returnLabel, CC_CALLBACK_1(GameScene::ReturnCallBack, this));
    returnItem->setPosition(Vec2(origin.x + visibleSize.width - 180, origin.y + 340));

    auto saveLabel = Label::createWithTTF("Save", "fonts/Marker Felt.ttf", 40);
    auto saveItem = MenuItemLabel::create(saveLabel, CC_CALLBACK_1(GameScene::SaveCallBack, this));
    saveItem->setPosition(Vec2(origin.x + visibleSize.width - 180, origin.y + 460));

    auto checkLabel = Label::createWithTTF("Check", "fonts/Marker Felt.ttf", 40);
    auto checkItem = MenuItemLabel::create(checkLabel, CC_CALLBACK_1(GameScene::CheckCallBack, this));
    checkItem->setPosition(Vec2(origin.x + visibleSize.width - 180, origin.y + 520));

    Menu* menu = Menu::create(clItem, returnItem, saveItem, checkItem, NULL);
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
        isSaved = false;
        // 若上次改变了颜色，则恢复之
        if (lastCheck != -1)
        {
            if (lastCheck < 9)
            {
                // 行恢复
                for (int i = 0; i < 9; i++)
                {
                    Sprite* s = (Sprite*)this->getChildByTag(200 + 9 * lastCheck + i);
                    int isGiven = ShareGlobal()->grid[lastCheck][i].isGiven;
                    if (isGiven)
                        s->setColor(Color3B(51, 51, 255));
                    else
                        s->setColor(Color3B(0, 255, 255));
                }
                // 恢复后重置lastcheck
                lastCheck = -1;
            }
            else if (lastCheck < 19)
            {
                // 列恢复
                lastCheck -= 10;
                for (int i = 0; i < 9; i++)
                {
                    Sprite* s = (Sprite*)this->getChildByTag(200 + 9 * i + lastCheck);
                    int isGiven = ShareGlobal()->grid[i][lastCheck].isGiven;
                    if (isGiven)
                        s->setColor(Color3B(51, 51, 255));
                    else
                        s->setColor(Color3B(0, 255, 255));
                }
                // 恢复后重置lastcheck
                lastCheck = -1;
            }
            else
            {
                // 宫恢复
                lastCheck -= 20;
                int rs = 3 * (lastCheck / 3);
                int cs = 3 * (lastCheck % 3);
                for (int i = rs; i < rs + 3; i++)
                {
                    for (int j = cs; j < cs + 3; j++)
                    {
                        Sprite* s = (Sprite*)this->getChildByTag(200 + 9 * i + j);
                        int isGiven = ShareGlobal()->grid[i][j].isGiven;
                        if (isGiven)
                            s->setColor(Color3B(51, 51, 255));
                        else
                            s->setColor(Color3B(0, 255, 255));
                    }
                }
                // 恢复后重置lastcheck
                lastCheck = -1;
            }
        }
        // 检测是否又不符合数独规则的行列宫，改变相应子结构的颜色
        // 只改变第一个发现的子结构，待用户解决后，才会提示另外的错误
        int line = ShareGlobal()->CheckLine();
        int column = ShareGlobal()->CheckColumn();
        int cell = ShareGlobal()->CheckCell();
        if (line != -1)
        {
            for (int i = 0; i < 9; i++)
            {
                Sprite* s = (Sprite*)this->getChildByTag(200 + 9 * line + i);
                s->setColor(Color3B(255, 0, 0));
            }
            lastCheck = line;
        }
        else if (column != -1)
        {
            for (int i = 0; i < 9; i++)
            {
                Sprite* s = (Sprite*)this->getChildByTag(200 + 9 * i + column);
                s->setColor(Color3B(255, 0, 0));
            }
            lastCheck = 10 + column;
        }
        else if (cell != -1)
        {
            int rs = 3 * (cell / 3);
            int cs = 3 * (cell % 3);
            for (int i = rs; i < rs + 3; i++)
            {
                for (int j = cs; j < cs + 3; j++)
                {
                    Sprite* s = (Sprite*)this->getChildByTag(200 + 9 * i + j);
                    s->setColor(Color3B(255, 0, 0));
                }
            }
            lastCheck = 20 + cell;
        }
        // 如果上面的没有问题并且用户填完了,弹出对话框示意结束
        if (ShareGlobal()->IsFinished())
        {
            // 获取屏幕宽度和高度
            auto visibleSize = Director::getInstance()->getVisibleSize();
            Vec2 origin = Director::getInstance()->getVisibleOrigin();
            // 设置对话框颜色，位置，透明度
            Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this, true);
            auto colorLay = LayerColor::create(Color4B(51, 51, 255, 200));
            colorLay->ignoreAnchorPointForPosition(false);
            colorLay->setAnchorPoint(Vec2(0.5, 0.5));
            colorLay->changeHeight(visibleSize.height / 2);
            colorLay->changeWidth(visibleSize.width / 2);
            colorLay->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
            this->addChild(colorLay, 3);
            // 显示文本
            auto label = Label::createWithTTF("Congratulations!", "fonts/Marker Felt.ttf", 70);
            label->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height / 4));
            colorLay->addChild(label, 4);
            // 返回主菜单的按钮
            auto closeLabel = Label::createWithTTF("Exit", "fonts/Marker Felt.ttf", 40);
            auto closeItem = MenuItemLabel::create(closeLabel, [=](Ref* pSender)
            {// 销毁图层，恢复监听，返回主菜单
                colorLay->removeFromParent();
                Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this, true);
                auto scene = HelloWorld::createScene();
                Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4, scene));
            });
            closeItem->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height / 4 - 100));
            auto menu = MenuItemLabel::create(closeItem, NULL);
            menu->setPosition(Vec2::ZERO);
            colorLay->addChild(menu, 4);
        }
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
    // 如果保存了，那么直接返回
    if (isSaved)
    {
        auto scene = HelloWorld::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4, scene));
    }
    // 没保存，那么弹出对话框，屏蔽下方事件监听
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this, true);
    auto colorLay = LayerColor::create(Color4B(0, 0, 0, 128));
    this->addChild(colorLay, 3);
    // 定义返回游戏按钮
    auto returnLabel = Label::createWithTTF("Cancel", "fonts/Marker Felt.ttf", 40);
    auto returnItem = MenuItemLabel::create(returnLabel, [=](Ref* pSender)
    {// 销毁图层，恢复监听
        colorLay->removeFromParent();
        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this, true);
    });
    returnItem->setPosition(Vec2(500, 340));
    // 无论如何也要返回主菜单的按钮
    auto closeLabel = Label::createWithTTF("GiveUp", "fonts/Marker Felt.ttf", 40);
    auto closeItem = MenuItemLabel::create(closeLabel, [=](Ref* pSender)
    {// 销毁图层，恢复监听，返回主菜单
        colorLay->removeFromParent();
        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this, true);
        auto scene = HelloWorld::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4, scene));
    });
    closeItem->setPosition(Vec2(500, 280));

    auto saveLabel = Label::createWithTTF("Save", "fonts/Marker Felt.ttf", 40);
    auto saveItem = MenuItemLabel::create(saveLabel, [=](Ref* pSender)
    {
        // 保存游戏
        std::string temp;
        std::string given;
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                temp += char(ShareGlobal()->grid[i][j].num + '0');
                given += char(int(ShareGlobal()->grid[i][j].isGiven) + '0');
            }
        }
        CCUserDefault::sharedUserDefault()->setStringForKey("puzzle", temp);
        CCUserDefault::sharedUserDefault()->setStringForKey("given", given);
        CCUserDefault::sharedUserDefault()->setIntegerForKey("time", time);
        CCUserDefault::sharedUserDefault()->flush();
        isSaved = true;
        // 返回
        colorLay->removeFromParent();
        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this, true);
        auto scene = HelloWorld::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4, scene));
    });
    saveItem->setPosition(Vec2(500, 400));

    auto menu = Menu::create(returnItem, closeItem,saveItem ,NULL);
    menu->setPosition(Vec2(0, 0));
    colorLay->addChild(menu, 4);
}
// 保存游戏
void GameScene::SaveCallBack(Ref* pSender)
{
    std::string temp;
    std::string given;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            temp += char(ShareGlobal()->grid[i][j].num + '0');
            given += char(int(ShareGlobal()->grid[i][j].isGiven) + '0');
        }
    }
    CCUserDefault::sharedUserDefault()->setStringForKey("puzzle", temp);
    CCUserDefault::sharedUserDefault()->setStringForKey("given", given);
    CCUserDefault::sharedUserDefault()->setIntegerForKey("time", time);
    CCUserDefault::sharedUserDefault()->flush();
    isSaved = true;
}
// check
void GameScene::CheckCallBack(Ref* pSender)
{
    // 点击后暂停事件监听（屏蔽对下方图层的控制）
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this, true);
    auto colorLay = LayerColor::create(Color4B(0, 0, 0, 128));
    this->addChild(colorLay,3);
    auto returnLabel = Label::createWithTTF("return", "fonts/Marker Felt.ttf", 40);
    // 该子图层的返回按钮，lambda函数定义回调函数销毁图层并恢复事件监听
    auto returnItem = MenuItemLabel::create(returnLabel, [=](Ref* pSender)
    {
        colorLay->removeFromParent();
        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this, true);
    });
    returnItem->setPosition(Vec2(500, 400));
    auto menu = Menu::create(returnItem, NULL);
    menu->setPosition(Vec2::ZERO);
    colorLay->addChild(menu, 4);


/////////////////////////////////////////////
    if (!ShareGlobal()->IsFinished())
    {// you haven't finished the board

    }
    else if(!ShareGlobal()->Check())
    {// your answer is wrong 

    }
    else
    {// congratulations!

    }
}
void GameScene::UpdateTime(float dt)
{
    Label* label = (Label*)this->getChildByTag(50);
    time += 1;
    char stime[6];
    itoa(time, stime, 10);
    label->setString(std::string(stime) + " s");
}
