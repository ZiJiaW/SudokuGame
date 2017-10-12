#pragma once
#ifndef SET_SCENE_H
#define SET_SCENE_H
#include "cocos2d.h"

class SettingScene :public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void ReturnCallBack(cocos2d::Ref* pSender);

    CREATE_FUNC(SettingScene);
private:

};


#endif // !SET_SCENE_H
