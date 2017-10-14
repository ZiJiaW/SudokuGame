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
    degree = 0;
    time = 0;
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
bool Global::Check()
{
    bool r = true;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            for (int k = 0; k < 9; k++)
            {
                r &= (i != k ? grid[i][j].num != grid[k][j].num : true);
                r &= (j != k ? grid[i][j].num != grid[i][k].num : true);
            }
            int rs = 3 * (i / 3);
            int cs = 3 * (j / 3);
            for (int p = rs; p < rs + 3; p++)
            {
                for (int q = cs; q < cs + 3; q++)
                {
                    r &= ((p != i&&q != j) ? grid[p][q].num != grid[i][j].num : true);
                }
            }
        }
    }
    return r;
}
bool Global::IsFinished()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (grid[i][j].num == 0)
            {
                return false;
            }
        }
    }
    return true;
}
void Global::GenerateGame()
{
    int result[1][81];
    generate(1, degree, result);
    for (int i = 0; i < 81; i++)
    {
        grid[i / 9][i % 9].num = result[0][i];
        grid[i / 9][i % 9].isGiven = result[0][i] != 0 ? true : false;
    }
}
int Global::CheckLine()
{
    for (int line = 0; line < 9; line++)
    {
        for (int col = 0; col < 8; col++)
        {
            for (int ncol = col + 1; ncol < 9; ncol++)
            {
                if (grid[line][col].num == grid[line][ncol].num && grid[line][col].num != 0)
                {
                    return line;
                }
            }
        }
    }
    return -1;
}
int Global::CheckColumn()
{
    for (int col = 0; col < 9; col++)
    {
        for (int row = 0; row < 8; row++)
        {
            for (int nrow = row + 1; nrow < 9; nrow++)
            {
                if (grid[row][col].num == grid[nrow][col].num && grid[row][col].num != 0)
                {
                    return col;
                }
            }
        }
    }
    return -1;
}
int Global::CheckCell()
{
    for (int cell = 0; cell < 9; cell++)
    {
        int rs = 3 * (cell / 3);
        int cs = 3 * (cell % 3);
        for (int i = rs; i < rs + 3; i++)
        {
            for (int j = cs; j < cs + 3; j++)
            {
                // 判断是否有与i，j相同的格子
                // 懒得写不傻逼的算法了...
                for (int k = rs; k < rs + 3; k++)
                {
                    for (int l = cs; l < cs + 3; l++)
                    {
                        if (k != i || l != j)
                        {
                            if (grid[i][j].num != 0 && grid[i][j].num == grid[k][l].num)
                            {
                                return cell;
                            }
                        }
                    }
                }
            }
        }
    }
    return -1;
}