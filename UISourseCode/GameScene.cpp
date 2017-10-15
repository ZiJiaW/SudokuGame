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
    lastCheck = -1;
    time = ShareGlobal()->time;
    for (int i = 0; i < 81; i++)
    {
        changed[i] = false;
    }

    // 生成目前棋盘的解
    ShareGlobal()->GenerateSolution();
    // 获得界面尺寸
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // background
    auto bg = Sprite::create("backgroundempty.png");
    bg->setPosition(Vec2(visibleSize / 2));
    bg->setContentSize(Size(1024, 768));
    this->addChild(bg, 0);
    // 显示难度
    std::string s;
    if (ShareGlobal()->degree == 1)
    {
        s += "Easy.png";
    }
    else if (ShareGlobal()->degree == 2)
    {
        s += "Normal.png";
    }
    else
    {
        s += "Hard.png";
    }
    auto deg = Sprite::create(s.c_str());
    deg->setPosition(Vec2(origin.x + 210, origin.y + 720));
    this -> addChild(deg, 1);

    // 显示计时 tag = 50
    auto timeLabel = Label::createWithTTF("0 s", "fonts/Marker Felt.ttf", 60);
    timeLabel->setColor(Color3B(0, 0, 0));
    timeLabel->setPosition(Vec2(origin.x + visibleSize.width - 180, origin.y + 720));
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
                grid->setColor(Color3B(51, 204, 255));
                auto gridbg = Sprite::create("Table.png");
                gridbg->setContentSize(Size(65, 65));
                gridbg->setPosition(Vec2(origin.x + 100 + (j + 0.5)*65.0f,
                    origin.y + visibleSize.height - 100 - (i + 0.5)*65.0f));
                if (isGiven)
                {
                    gridbg->setColor(Color3B(0, 102, 255));
                }
                else
                {
                    gridbg->setColor(Color3B(255, 255, 255));
                }
                this->addChild(gridbg, 1, 200 + 9 * i + j);
                this->addChild(grid, 2, 100 + 9 * i + j);
            }
            else
            {
                auto grid = Label::createWithTTF("", "fonts/Marker Felt.ttf", 40);
                grid->setPosition(Vec2(origin.x + 100 + (j + 0.5)*65.0f,
                    origin.y + visibleSize.height - 100 - (i + 0.5)*65.0f));
                grid->setColor(Color3B(51, 204, 255));
                auto gridbg = Sprite::create("Table.png");
                gridbg->setContentSize(Size(65, 65));
                gridbg->setPosition(Vec2(origin.x + 100 + (j + 0.5)*65.0f,
                    origin.y + visibleSize.height - 100 - (i + 0.5)*65.0f));
                gridbg->setColor(Color3B(255, 255, 255));
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
            int num = 3 * i + j + 1;
            auto button = Scale9Sprite::create(std::string(1, char(num + '0')) + ".png");
            button->setContentSize(Size(65,65));
            auto numbut = ControlButton::create(button);
            numbut->setPosition(Vec2(origin.x + visibleSize.width - 30 - (2.5 - j)*65.0f, origin.y + visibleSize.height - 100 - (i + 6.5)*65.0f));
            numbut->addTargetWithActionForControlEvents(this, cccontrol_selector(GameScene::NumbutCallBack), Control::EventType::TOUCH_UP_INSIDE);
            this->addChild(numbut, 1, 300 + 3 * i + j);
        }
    }
    // 菜单设置：清空按钮,返回按钮，保存按钮,check,提示
    auto clLabel = Label::createWithTTF("Clear", "fonts/Marker Felt.ttf", 50);
    clLabel->setColor(Color3B(0,0,0));
    auto clItem = MenuItemLabel::create(clLabel, CC_CALLBACK_1(GameScene::ClearCallBack, this));
    clItem->setPosition(Vec2(origin.x + visibleSize.width - 30 - (2.5 - 1)*65.0f-20, origin.y + visibleSize.height - 100 - (-2 + 6.5)*65.0f));

    auto returnButton = Scale9Sprite::create("return.png");
    returnButton->setContentSize(Size(65, 65));
    auto rbut = ControlButton::create(returnButton);
    rbut->setPosition(Vec2(origin.x + visibleSize.width - 30 - (2.5 - 2)*65.0f, origin.y + visibleSize.height - 100 - (3 + 6.5)*65.0f));
    rbut->addTargetWithActionForControlEvents(this, cccontrol_selector(GameScene::ReturnCallBack), Control::EventType::TOUCH_UP_INSIDE);
    this->addChild(rbut, 2);

    auto hintLabel = Label::createWithTTF("Hint", "fonts/Marker Felt.ttf", 50);
    hintLabel->setColor(Color3B(0, 0, 0));
    auto hintItem = MenuItemLabel::create(hintLabel, CC_CALLBACK_1(GameScene::HintCallBack, this));
    hintItem->setPosition(Vec2(origin.x + visibleSize.width - 30 - (2.5 - 1)*65.0f-20, origin.y + visibleSize.height - 100 - (-3 + 6.5)*65.0f));
    // 音乐播放与暂停
    auto musicOn = MenuItemImage::create("SoundOn.png", "SoundOn.png");
    auto musicOff = MenuItemImage::create("SoundOff.png", "SoundOff.png");
    auto musicItem = MenuItemToggle::createWithCallback([=](Ref* pSender) {
        if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
        {
            SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        }
        else
        {
            SimpleAudioEngine::getInstance()->playBackgroundMusic("bg.wav", true);
        }
    }, musicOn, musicOff, NULL);
    musicItem->setPosition(Vec2(origin.x + visibleSize.width - 30 - (2.5 - 1)*65.0f, origin.y + visibleSize.height - 100 - (3 + 6.5)*65.0f));
    musicItem->setScale(1.15f);
    Menu* menu = Menu::create(clItem, hintItem,musicItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);



    // 设置鼠标事件监听者
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = [=](Event* event)
    {
        EventMouse* e = (EventMouse*)event;
        auto mousepos = e->getLocation();
        mousepos.y = visibleSize.height - mousepos.y;
        for (int i = 0; i < 81; i++)
        {
            Sprite* s = (Sprite*)this->getChildByTag(200 + i);
            if (s->getBoundingBox().containsPoint(mousepos) && !ShareGlobal()->grid[i / 9][i % 9].isGiven)
            {
                selected = i;
                if (!changed[i])
                {
                    s->setColor(Color3B(51, 102, 153));
                    changed[i] = true;
                }
                else
                {
                    s->setColor(Color3B(255, 255, 255));
                    changed[i] = false;
                }
            }
            else if(!ShareGlobal()->grid[i / 9][i % 9].isGiven)
            {//只改变选中的可变格子的颜色,其他格子变回来
                s->setColor(Color3B(255, 255, 255));
                changed[i] = false;
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
                        s->setColor(Color3B(0, 102, 255));
                    else
                        s->setColor(Color3B(255, 255, 255));
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
                        s->setColor(Color3B(0, 102, 255));
                    else
                        s->setColor(Color3B(255, 255, 255));
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
                            s->setColor(Color3B(0, 102, 255));
                        else
                            s->setColor(Color3B(255, 255, 255));
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
        else if (ShareGlobal()->IsFinished())
        {
            // 获取屏幕宽度和高度
            auto visibleSize = Director::getInstance()->getVisibleSize();
            Vec2 origin = Director::getInstance()->getVisibleOrigin();
            // 设置对话框颜色，位置，透明度
            Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this, true);
            auto colorLay = LayerColor::create(Color4B(51, 102, 255, 200));
            colorLay->ignoreAnchorPointForPosition(false);
            colorLay->setAnchorPoint(Vec2(0.5, 0.5));
            colorLay->changeHeight(visibleSize.height / 2);
            colorLay->changeWidth(visibleSize.width / 2);
            colorLay->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
            this->addChild(colorLay, 3);
            // 显示文本
            auto label = Label::createWithTTF("Congratulations!", "fonts/Marker Felt.ttf", 70);
            label->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height / 4+80));
            label->setColor(Color3B(0, 0, 0));
            colorLay->addChild(label, 4);
            // 用时
            std::string stime("You cost ");
            char ctime[6];
            itoa(time, ctime, 10);
            stime += ctime;
            stime += " s";
            auto tlabel = Label::createWithTTF(stime.c_str(), "fonts/Marker Felt.ttf", 50);
            tlabel->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height / 4 -10));
            tlabel->setColor(Color3B(0, 0, 0));
            colorLay->addChild(tlabel, 4);
            // 记录最佳时间
            if (ShareGlobal()->degree == 1 && (ShareGlobal()->bestRecordEasy > time || ShareGlobal()->bestRecordEasy == -1))
            {
                // 如果没有记录或是好于最佳记录
                CCUserDefault::sharedUserDefault()->setIntegerForKey("bestRecordEasy", time);
            }
            if (ShareGlobal()->degree == 2 && (ShareGlobal()->bestRecordMedium > time || ShareGlobal()->bestRecordMedium == -1))
            {
                // 如果没有记录或是好于最佳记录
                CCUserDefault::sharedUserDefault()->setIntegerForKey("bestRecordMedium", time);
            }
            if (ShareGlobal()->degree == 3 && (ShareGlobal()->bestRecordHard > time || ShareGlobal()->bestRecordHard == -1))
            {
                // 如果没有记录或是好于最佳记录
                CCUserDefault::sharedUserDefault()->setIntegerForKey("bestRecordHard", time);
            }
            // 返回主菜单的按钮
            auto closeLabel = Label::createWithTTF("Exit", "fonts/Marker Felt.ttf", 40);
            closeLabel->setColor(Color3B(0, 0, 0));
            auto closeItem = MenuItemLabel::create(closeLabel, [=](Ref* pSender)
            {// 销毁图层，恢复监听，返回主菜单
                colorLay->removeFromParent();
                Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this, true);
                auto scene = HelloWorld::createScene();
                Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4, scene));
            });
            closeItem->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height / 4 - 100));
            auto menu = Menu::create(closeItem, NULL);
            menu->setPosition(Vec2::ZERO);
            colorLay->addChild(menu, 5);
        }
    }
}

// 清空已填格子
void GameScene::ClearCallBack(Ref* pSender)
{
    for (int i = 0; i < 81; i++)
    {
        Sprite* s = (Sprite*)this->getChildByTag(200 + i);
        if (!ShareGlobal()->grid[i / 9][i % 9].isGiven)
        {
            Label* numLabel = (Label*)this->getChildByTag(100 + i);
            numLabel->setString("");
            ShareGlobal()->grid[i / 9][i % 9].num = 0;
            s->setColor(Color3B(255, 255, 255));
        }
        else
        {
            s->setColor(Color3B(0, 102, 255));
        }
    }
}

// 返回主菜单
void GameScene::ReturnCallBack(Ref* Sender,Control::EventType type)
{
    // 弹出对话框，屏蔽下方事件监听
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this, true);
    auto colorLay = LayerColor::create(Color4B(0, 0, 0, 128));
    this->addChild(colorLay, 3);
    // 会话内容
    auto text = Label::createWithTTF("You haven't finished, do you want to save?", "fonts/Marker Felt.ttf", 50);
    text->setPosition(Vec2(512, 480));
    colorLay->addChild(text, 4);
    // 定义返回游戏按钮
    auto returnLabel = Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 40);
    auto returnItem = MenuItemLabel::create(returnLabel, [=](Ref* pSender)
    {// 销毁图层，恢复监听
        colorLay->removeFromParent();
        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this, true);
    });
    returnItem->setPosition(Vec2(512, 340));
    // 无论如何也要返回主菜单的按钮
    auto closeLabel = Label::createWithTTF("GiveUp", "fonts/Marker Felt.ttf", 40);
    auto closeItem = MenuItemLabel::create(closeLabel, [=](Ref* pSender)
    {// 销毁图层，恢复监听，返回主菜单
        colorLay->removeFromParent();
        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this, true);
        auto scene = HelloWorld::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4, scene));
    });
    closeItem->setPosition(Vec2(512, 280));

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
        CCUserDefault::sharedUserDefault()->setIntegerForKey("degree", ShareGlobal()->degree);
        CCUserDefault::sharedUserDefault()->flush();
        // 返回
        colorLay->removeFromParent();
        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this, true);
        auto scene = HelloWorld::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4, scene));
    });
    saveItem->setPosition(Vec2(512, 400));

    auto menu = Menu::create(returnItem, closeItem,saveItem ,NULL);
    menu->setPosition(Vec2(0, 0));
    colorLay->addChild(menu, 4);
}


void GameScene::UpdateTime(float dt)
{
    Label* label = (Label*)this->getChildByTag(50);
    time += 1;
    char stime[6];
    itoa(time, stime, 10);
    label->setString(std::string(stime) + " s");
}

void GameScene::HintCallBack(Ref* pSender)
{
    if (selected != -1)
    {
        Label* num = (Label*)this->getChildByTag(100 + selected);
        num->setString(std::string(1, char(ShareGlobal()->solution[selected / 9][selected % 9] + '0')));
        ShareGlobal()->grid[selected / 9][selected % 9].num = ShareGlobal()->solution[selected / 9][selected % 9];
        Sprite* s = (Sprite*)this->getChildByTag(200 + selected);
        s->setColor(Color3B(0, 255, 204));
    }
}
