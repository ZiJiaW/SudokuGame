#pragma once
#include "SdkBuffer.h"
#ifndef FH_H
#define FH_H

class FileHandler
{
private:
    FILE * file;
public:
    FileHandler();
    bool Open(const char *pathname, const char*mode);
    void Close();
    void WriteSdb(SdkBuffer* sdb);
    void ReadSdb(SdkBuffer* sdb);
    bool HasNext();
    ~FileHandler();
};
#endif // !FH_H