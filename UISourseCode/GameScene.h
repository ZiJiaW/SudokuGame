#pragma once
#include "cocos2d.h"
#include "GUI\CCControlExtension\CCControl.h"
#include "GUI\CCControlExtension\CCControlButton.h"
#include"CocosGui.h"
USING_NS_CC;
using namespace cocos2d::extension;
using namespace cocos2d::ui;
#ifndef GAME_H
#define GAME_H
class GameScene :public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void ReturnCallBack(cocos2d::Ref* pSender, Control::EventType type);
    void NumbutCallBack(cocos2d::Ref* pSender, Control::EventType type);
    void ClearCallBack(cocos2d::Ref* pSender);
//    void SaveCallBack(cocos2d::Ref* pSender);
    void HintCallBack(cocos2d::Ref* pSender);
//    void FromCheckBackToGame(cocos2d::Ref* pSender);
    // 初始值-1；在自动检测中如果改变了某子结构的颜色，则值改变，用于下次填数恢复颜色之用
    // 改变的值设置为：0-8行为0-8；0-8列为10-18；0-8宫为20-28
    int lastCheck;
    CREATE_FUNC(GameScene);
private:
    int selected;
    bool changed[81];
    void UpdateTime(float dt);
    int time;
};

#endif // !GAME_H
