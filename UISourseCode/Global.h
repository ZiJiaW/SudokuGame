#pragma once
//#pragma comment(lib,"..\\proj.win32\\Debug.win32 CaaeSudoku.lib")
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "cocos-ext.h"
class Global
{
public:
    Global();
    ~Global();
    typedef struct table
    {
        int num;
        bool isGiven;
    }Table;
    Table grid[9][9];
    bool Check();
    bool IsFinished();
    int degree;
    void GenerateGame();
    // 返回 -1 表示没有发现错误，返回 0-8 表示某行列宫存在错误
    int CheckLine();
    int CheckColumn();
    int CheckCell();
    // 初始化时记录时间
    int time;
    // 最佳记录
    int bestRecordEasy;
    int bestRecordMedium;
    int bestRecordHard;
    // 生成当前数独的解，用于提示功能
    void GenerateSolution();
    int solution[9][9];

};
extern _declspec(dllimport) int sum(int a, int b);
extern _declspec(dllimport) void generate(int number, int mode, int result[][81]);
extern _declspec(dllimport) void generate(int number, int lower, int upper, bool unique, int result[][81]);
extern _declspec(dllimport) bool solve(int puzzle[], int solution[]);
extern _declspec(dllimport) void generate(int number, int result[][81]);
Global* ShareGlobal();
