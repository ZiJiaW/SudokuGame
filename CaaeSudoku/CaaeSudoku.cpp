#include "stdafx.h"
#include <iostream>
#include "FileHandler.h"
#include "ArgumentHandler.h"
#include "SdkBuffer.h"
#include "Table.h"
#include <time.h>
using namespace std;
typedef ArgumentHandler::State state;
const unsigned  int BufferSize = 10000;
//const int MaxCounts = 1000000;
int main(int argc, char**args)
{
#ifdef DEBUG
    clock_t start = clock();
#endif // DEBUG
    ArgumentHandler* ah = new ArgumentHandler();
    ah->ParseInput(argc, args);
    state st = ah->GetState();
    if (st == state::INV)
    {
        delete ah;
        return 1;
    }
    FileHandler *fh = new FileHandler();
    Table *tb = new Table();
    if (st == state::GEN)
    {
        do {
            unsigned int count = ah->GetCount();
            /*if (count > MaxCounts)
            {
                cout << "Count is too big" << endl;
                break;
            }*/
            if (!fh->Open("sudoku.txt", "w")) {
                cout << "File IO error!" << endl;
                break;
            }
            tb->SetZero();
            tb->Set(0, 0, 6);
            tb->Generate(count, fh);
        } while (false);
    }
    else
    {
        FileHandler* dst = new FileHandler();
        do {
            if (dst->Open("sudoku.txt", "w") && fh->Open(ah->GetPathName(), "r"))
            {
                tb->Solve(fh, dst);
            }
            else
            {
                cout << "Error happend when trying to read puzzle file" << endl;
            }
        } while (false);
        dst->Close();
        delete dst;
    }
    fh->Close();
    delete fh;
    delete tb;
#ifdef  DEBUG
    cout << "elapsed" << (float(clock()) - start) / 1000;
    getchar();
#endif //  DEBUG
    return 0;
}