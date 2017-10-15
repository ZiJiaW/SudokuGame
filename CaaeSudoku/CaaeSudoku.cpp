#include "stdafx.h"
#include <iostream>
#include <time.h>
#include "FileHandler.h"
#include "ArgumentHandler.h"
#include "Table.h"
#include "SdkBuffer.h"
#include "Core.h"
#include "CaaeSudoku.h"
using namespace std;
int main(int argc, char**args)
{
#ifdef DEBUG
    clock_t start = clock();
#endif // DEBUG
	ArgumentHandler ah;
    ah.ParseInput(argc, args);
    State st = ah.GetState();
    if (st == State::INV)
    {
        return 1;
    }
	FileHandler fh;
    Table tb;
	try {
		if (st == State::GEN)
		{
				unsigned int count = ah.GetCount();
				fh.Open("sudoku.txt", "w");
				tb.GenerateRandomly(count, &fh);
		}
		else if (st == State::SOV)
		{
			FileHandler dst;
			dst.Open("sudoku.txt", "w"); 
			fh.Open(ah.GetPathName(), "r");
			tb.Solve(&fh, &dst);
			dst.Close();
		}
		//use this to store Core`s output
		int(*arrayBuffer)[81] = new int[gBufferSize][81];
	    if (st == State::GEG_R || st == State::GEG_U || st == State::GEG_RU)
		{
			bool unique = false;
			unsigned int lower = 1, upper = 1;
			if (st == State::GEG_U || st == State::GEG_RU)
			{
				unique = true;
			}
			if (st == State::GEG_R || st == State::GEG_RU)
			{
				lower = ah.GetLower();
				upper = ah.GetUpper();
			}
			fh.Open(gOutputFilePath, "w");
			SdkBuffer sdb(gBufferSize);
			unsigned int count = ah.GetCount();
			unsigned int number;
			for (int i = 0; i < count; i += gBufferSize)
			{
				number = gBufferSize;
				if (count - i < gBufferSize)
				{
					number = count - i;
				}
				generate(count, lower, upper, unique, arrayBuffer);
				sdb.InitByArray(arrayBuffer,number);
				fh.WriteSdb(&sdb);
			}
		}
		else if (st == State::GEG_M)
		{
			Difficulty diff = ah.GetDifficulty();
			fh.Open(gOutputFilePath, "w");
			SdkBuffer sdb(gBufferSize);
			unsigned int count = ah.GetCount();
			unsigned int number;
			for (int i = 0; i < count; i += gBufferSize)
			{
				number = gBufferSize;
				if (count - i < gBufferSize)
				{
					number = count - i;
				}
				generate(number, diff, arrayBuffer);
				sdb.InitByArray(arrayBuffer, number);
				fh.WriteSdb(&sdb);
			}
			
		}
		delete[] arrayBuffer;
	}
	catch(exception ex)
	{
		cout << ex.what();
		//todo(Zijia):
	}
    fh.Close();
#ifdef  DEBUG
    cout << "elapsed" << (float(clock()) - start) / 1000;
#endif //  DEBUG
    return 0;
}