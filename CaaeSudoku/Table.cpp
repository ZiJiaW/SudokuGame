#include "stdafx.h"
#include "Table.h"
#include <vector>
using namespace std;
extern const unsigned  int BufferSize;
Table::Table()
{
    total = 0; //the number of soultion found by far
    top = 0;  //the searching process will stop when 'total' reaches to 'top' 
    SetZero();//set all elements to zero
}
void Table::Generate(unsigned int max, SdkBuffer* sdb)
{
    //set some initial argument 
    this->total = 0;
    this->top = max;
    //clear the buffer
    sdb->Clear();
    //limit 'max' to the capacity 
    if (max > sdb->GetCapacity())
        max = sdb->GetCapacity();
    pCurrentBuffer = sdb;
    //start solving
    solve(0, 1);
}
void Table::Set(int row, int col, int num)
{
    //set one element
    cells[row][col] = num;
}
void Table::SetZero()
{
    //initiate all elements
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            cells[i][j] = 0;
        }
    }
}
bool Table::Solve(SdkBuffer* sdb)
{
    total = 0;
    top = 1;
    pCurrentBuffer = sdb;
    startSolving();
    //total==1 means we found a solution
    if (total == 1)return true;
    else return false;
}
void Table::Generate(unsigned int total, FileHandler*fh)
{
    //allocate a new buffer 
    SdkBuffer * sdb = new SdkBuffer(BufferSize);
    //generate sudoku solution and write to the file
    for (unsigned int i = 0; i < total; i += BufferSize)
    {
        sdb->Clear();
        if (total - i <= BufferSize)
            Generate(total - i, sdb);
        else
            Generate(BufferSize, sdb);
        fh->WriteSdb(sdb);
    }
    delete sdb;
}
void Table::Solve(SdkBuffer* sdbSrc, SdkBuffer* sdbDst)
{
    sdbDst->Clear();
    while (sdbSrc->GetSize()>0)
    {
        sdbSrc->Pop(cells);
        Solve(sdbDst);
    }
}
void Table::Solve(FileHandler* src, FileHandler*dst)
{
    SdkBuffer*  sdbSrc = new SdkBuffer(BufferSize);
    SdkBuffer* sdbDst = new SdkBuffer(BufferSize);
    while (src->HasNext())
    {
        src->ReadSdb(sdbSrc);
        Solve(sdbSrc, sdbDst);
        dst->WriteSdb(sdbDst);
    }
}

int*  Table::lookUp(int rst, int  cst, int num)
{
    int *result = new int[10];
    int index = 0;
    int ron = 0, con = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            ron = i + rst * 3;
            con = j + cst * 3;
            if (cells[ron][con] == num)
            {
                return NULL;
            }
            if (cells[ron][con] != 0)
                continue;
            bool pass = true;
            for (int t = 0; t < 9; t++)
            {
                if (cells[ron][t] == num || cells[t][con] == num)
                {
                    pass = false;
                    break;
                }
            }
            if (pass)
            {
                result[index++] = ron * 9 + con;
            }
        }
    }
    //	printf("%d", index);
    result[index] = -1;
    return result;
}
void Table::startSolving()
{
    solve(0, 1);
}
void Table::solve(int subt, int num)
{
    //this function works recursively to fill number 1-9 one by one to all the 9 sub tables

    //subt index subtable ,which is a 3x3 palace. It`s index starts from 0 to 8
    //num (1-9)  is the current  number we try to fiil in  


    //	printf("%d %d\n", subt, num);
    //if we get enough solutions ,just exit
    if (total == top)
        return;
    //It signals that all numbers are filled.   
    else if (num == 10)
    {
        total += 1;
        pCurrentBuffer->Fill(cells);
        return;
    }
    //we should try the next number while that all palaces are reached
    else  if (subt == 9)
    {
        solve(0, num + 1);
        return;
    }
    //Row or Column of SubTable
    int rst = subt / 3;
    int cst = subt % 3;
    //suitable cells 
    int * suitcells = lookUp(rst, cst, num);
    //randomsort(suitcells);
    if (suitcells == NULL)
    {
        solve(subt + 1, num);
        return;
    }
    int index = 0;
    //Row or Column of Number
    int ron = 0;
    int con = 0;
    while (suitcells[index] != -1 && total != top)
    {
        ron = suitcells[index] / 9;
        con = suitcells[index] % 9;
        cells[ron][con] = num;
        solve(subt + 1, num);
        cells[ron][con] = 0;//todo
        index++;
    }
    delete[] suitcells;
}
Table::~Table()
{

}