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
    // ���� -1 ��ʾû�з��ִ��󣬷��� 0-8 ��ʾĳ���й����ڴ���
    int CheckLine();
    int CheckColumn();
    int CheckCell();
    // ��ʼ��ʱ��¼ʱ��
    int time;
    // ��Ѽ�¼
    int bestRecordEasy;
    int bestRecordMedium;
    int bestRecordHard;
    // ���ɵ�ǰ�����Ľ⣬������ʾ����
    void GenerateSolution();
    int solution[9][9];

};
extern _declspec(dllimport) int sum(int a, int b);
extern _declspec(dllimport) void generate(int number, int mode, int result[][81]);
extern _declspec(dllimport) void generate(int number, int lower, int upper, bool unique, int result[][81]);
extern _declspec(dllimport) bool solve(int puzzle[], int solution[]);
extern _declspec(dllimport) void generate(int number, int result[][81]);
Global* ShareGlobal();
