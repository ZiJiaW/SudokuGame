#include "stdafx.h"
#include "FileHandler.h"
#include "ArgumentHandler.h"
#include "SdkBuffer.h"
#include "Table.h"
#include "DifficultyEvaluation.h"
#include <time.h>
using namespace std;
typedef ArgumentHandler::State state;
const unsigned  int BufferSize = 10000;
/*
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

*/
// for debug

int main()
{
    /*int p[9][9] = {
        {4,0,0,8,0,9,1,0,0},
        {0,0,7,0,0,0,0,9,0},
        {9,5,0,0,2,0,0,0,7},
        {1,0,0,0,9,0,0,0,3},
        {3,9,2,4,0,7,8,0,0},
        {6,0,0,0,3,0,0,0,9},
        {7,2,0,0,8,0,0,6,0},
        {0,1,0,0,0,0,2,0,0},
        {0,0,3,1,0,2,0,0,4}
    };*/
    /*int p[9][9] = {
        {9,6,0,0,1,0,0,3,0},
        {3,0,2,0,0,0,8,0,4},
        {0,7,0,0,0,0,0,9,6},
        {0,0,0,3,0,8,0,0,0},
        {6,0,9,0,0,0,0,8,5},
        {0,0,0,4,0,9,0,0,0},
        {0,2,0,5,8,4,0,6,0},
        {5,0,8,0,0,0,2,0,7},
        {0,4,0,0,9,0,3,5,0}
    };*/
    int p[9][9] = {
        {0,0,5,0,0,4,0,0,0},
        {0,0,0,0,6,0,0,9,0},
        {3,0,0,0,0,0,0,0,7},
        {0,0,0,0,4,0,0,0,0},
        {0,0,8,0,0,0,4,0,0},
        {5,4,1,0,0,0,0,0,9},
        {2,0,0,0,0,0,0,0,3},
        {0,0,7,4,0,0,0,0,0},
        {0,0,0,0,0,3,0,0,0}
    };
    
   /* int p[9][9] = {
        {4,0,0,3,1,9,0,0,6},
        {0,0,1,0,0,0,9,0,0},
        {0,6,7,4,0,0,0,2,1},
        {7,0,0,0,5,0,0,0,4},
        {0,0,0,1,4,2,0,0,0},
        {2,0,0,0,7,0,0,0,8},
        {0,2,0,0,0,0,0,6,0},
        {0,0,4,0,0,0,8,0,0},
        {1,0,0,5,0,8,0,0,7}
    };*/
    
   /* int p[9][9] = {
        {0,9,0,3,0,0,0,0,8},
        {0,0,7,0,0,4,1,2,0},
        {0,4,0,0,0,6,0,5,0},
        {0,0,9,0,4,0,0,0,1},
        {4,0,0,7,0,3,0,0,5},
        {8,0,0,0,6,0,2,0,0},
        {9,5,0,8,0,0,0,1,0},
        {0,3,4,6,0,0,9,0,0},
        {6,0,0,0,0,9,5,3,0}
    };*/
    DifficultyEvaluation de;
    cout << de.Evaluate(p) << endl;
    int q[9][9];
    de.GetPuzzle(q);
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (j == 8)
                cout << q[i][j] << endl;
            else
                cout << q[i][j] << ' ';
        }
    }
    return 0;
}