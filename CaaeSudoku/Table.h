#pragma once
#include"SdkBuffer.h"
#include "FileHandler.h"
extern const unsigned int BufferSize;
class Table
{
private:
    int cells[9][9];
    int total;
    int top;
    SdkBuffer* pCurrentBuffer;
public:
    void Table::startSolving();
    Table();
    /*@Modifies:this
    Set all elements to zero ,which stands for not being filled.
    */
    void SetZero();

    /*@Requires:0<=row,col<9 and 1<=num<=9
    @Modifies:this
    Set one element to the value given
    */
    void Set(int row, int col, int num);

    /*@Requires:length(num)>=9 and any element i in num[][] ==> 1<=i<=9
    @Modifies:this
    Set the table with the two-dimension array given.
    */
    void Set(int num[][9]);

    /*@Requires:total>0 and total <=100000 and sdb!=null and total<sdb->GetCapacity()
    @Modifies:sdb
    Generate sudoku solution and write to a file.
    */
    void Generate(unsigned int total, FileHandler* fh);

    /*@Requires:sdb!=nulls
    @Effects:/result==true <==> We can find at least one solution
    @Effects:/result==false <==> We can`t find any solution
    Solve the problem in the table
    */
    bool Solve(SdkBuffer* sdb);

    /*Solve the problems in src buffer and write the answers to the dst buffer
    Notice:it will clear the dst buffer firtst and clear src buffer as an effect
    */
    void Solve(SdkBuffer * sdbSrc, SdkBuffer * sdbDst);

    /*@Requires:total>0 and total <=100000 and sdb!=null
    @Modifies:sdb
    Generate sudoku solution to the sdb
    */
    void Generate(unsigned int total, SdkBuffer* sdb);

    /*@Requires:src!=null,dst!=null
    @Modifies:src,dst
    Solve the problem set given by the src and output the result to dst.
    */
    void Solve(FileHandler* src, FileHandler*dst);
    ~Table();
private:
    /*@Requires : 0<rst, cst<3 1 <= num <= 9
    @Effects : / result == null <= = > there is one number in Palace(rst, cst) equals to num
    @Effects : / result == {-1 } <= = > there is no suitable place for num in Palace(rst, cst)
    @Effects : len(/ result) >= 1 && / result != {-1} <= = > there is one or more suitable place for num in Palace(rst, cst)*/
    int* lookUp(int rst, int cst, int num);




    void solve(int subt, int num);
};