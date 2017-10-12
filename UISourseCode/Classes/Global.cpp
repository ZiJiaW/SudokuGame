#include "Global.h"
#include "IntroductionScene.h"
#include "HelloWorldScene.h"
#include "CocosGui.h"
#include "cocos-ext.h"
#include "GameScene.h"
#include "GUI\CCControlExtension\CCControl.h"
#include "GUI\CCControlExtension\CCControlButton.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

static Global* m_global = NULL;
Global::Global()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            grid[i][j].num = 0;
            grid[i][j].isGiven = false;
        }
    }
}
Global::~Global() {}
Global* ShareGlobal()
{
    if (m_global == NULL)
    {
        m_global = new Global();
    }
    return m_global;
}