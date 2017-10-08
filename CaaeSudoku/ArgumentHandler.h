#pragma once
#ifndef  AH_H
#define AH_H
class ArgumentHandler
{
public:
    enum State { GEN, SOV, INV, GEG };
    enum Difficulty { UNS, EASY, NORMAL, HARD };
    ArgumentHandler();
    void ParseInput(int argc, char **argv);
    State GetState();
    unsigned int GetCount();
    char * GetPathName();
    bool GetUnique();
    Difficulty GetDifficulty();
    int GetLower();
    int GetUpper();
    ~ArgumentHandler();
private:
    State state;
    Difficulty difc;
    unsigned int count;
    char* pathname;
    bool unique;
    int lower;
    int upper;
    bool IsDigit(char *in);
    bool JudgeR(char *in);
};
#endif // ! AH_H