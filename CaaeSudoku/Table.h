﻿#pragma once
#include"SdkBuffer.h"
#include "FileHandler.h"
#ifndef TBL_H
#define TBL_H

extern const unsigned int BufferSize;
class Table
{
private:
    int cells[9][9];
public:
    /*
    1.This class is used to generate or solve sudokus in large amount.
    2.Use Generate to generate and it overrides with different destinations.Solve also overrides.
    */
    Table();
    void Generate(unsigned int total, SdkBuffer* sdb);
    void GenerateRandomly(unsigned int total, SdkBuffer * sdb);
    void Generate(unsigned int total, FileHandler* fh);
    void GenerateRandomly(unsigned int total, FileHandler* fh);
    bool Solve(SdkBuffer* sdb);
    void Solve(SdkBuffer * sdbSrc, SdkBuffer * sdbDst);
    void Solve(FileHandler* src, FileHandler*dst);
    void DigRandomCells(SdkBuffer* pBoards, unsigned int lower, unsigned int upper, bool isAnswerUnique);
    ~Table();
private:
    /*
    */
    void digSpecNum(int[][9], unsigned int num);
    void digSpecNumUniquely(int[][9], unsigned int num);
    unsigned int startSolving(unsigned int maxAnswer, SdkBuffer*pResult);
    void solve(int subt, int num, unsigned int &total, unsigned int &top, SdkBuffer* pResult);
    int* lookUp(int rst, int cst, int num);
};
#endif // !TBL_H