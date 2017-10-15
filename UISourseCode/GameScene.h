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
    // ��ʼֵ-1�����Զ����������ı���ĳ�ӽṹ����ɫ����ֵ�ı䣬�����´������ָ���ɫ֮��
    // �ı��ֵ����Ϊ��0-8��Ϊ0-8��0-8��Ϊ10-18��0-8��Ϊ20-28
    int lastCheck;
    CREATE_FUNC(GameScene);
private:
    int selected;
    bool changed[81];
    void UpdateTime(float dt);
    int time;
};

#endif // !GAME_H
