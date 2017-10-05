#pragma once
class SdkBuffer
{
private:
    char *buffer;
    unsigned int size;
    unsigned int capacity;
public:
    SdkBuffer(unsigned int capacity);
    unsigned int GetSize();
    unsigned int GetCapacity();
    const char* GetBuffer();
    void Pop(int buffer[][9]);
    bool Fill(int table[][9]);
    void Clear();
    ~SdkBuffer();
};