#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H
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
    
};

Global* ShareGlobal();
#endif // !GLOBAL_H
