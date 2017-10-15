#include "stdafx.h"
#include <math.h>
#include <vector>
#include <ctime>
#include <set>
#include "Table.h"
#include "CaaeSudoku.h"

using namespace std;
//extern const unsigned  int gBufferSize;
Table::Table()
{
	setZero();
}
void Table::Generate(unsigned int max, SdkBuffer* sdb)
{
	//limit 'max' to the capacity
	unsigned int left = sdb->GetCapacity() - sdb->GetSize();
	if (max > left)max = left;
	//start solving
	setZero();
	startSolving(max, sdb);
}

void Table::Generate(unsigned int total, FileHandler*fh)
{
	//$todo(Felix):add argument checking in debug mode (total >0 ,fh!=null)
	//allocate a new buffer 
	SdkBuffer * sdb = new SdkBuffer(gBufferSize);
	//generate sudoku solution and write to the file
	for (unsigned int i = 0; i < total; i += gBufferSize)
	{
		sdb->Clear();
		if (total - i <= gBufferSize)
			Generate(total - i, sdb);
		else
			Generate(gBufferSize, sdb);
		fh->WriteSdb(sdb);
	}
	delete sdb;
}
bool IsDiffer(int line[9], int(*record)[9], int nowsize)
{
	for (int i = 0; i < nowsize; i++)
	{
		bool r = true;
		for (int j = 0; j < 9; j++)
		{
			r &= line[j] == record[i][j];
		}
		if (r)
			return false;
	}
	return true;
}
void Shuffle(int line[9], int(*record)[9], int nowsize)
{
	int r, mid;
	do {
		for (int i = 8; i >= 0; i--)
		{
			r = rand() % (i + 1);//0-i
			mid = line[i];
			line[i] = line[r];
			line[r] = mid;
		}
	} while (!IsDiffer(line, record, nowsize));
}
void Table::GenerateRandomly(unsigned int total, SdkBuffer* sdb)
{
    srand(clock());
	int firstLine[] = { 1,2,3,4,5,6,7,8,9 };
	int(*record)[9] = new int[total][9];
	setZero();
	for (unsigned int i = 0; i < total; i++)
	{
		Shuffle(firstLine,record,i);
		for (int j = 0; j < 9; j++)
		{
			cells[0][j] = firstLine[j];
            record[i][j] = firstLine[j];
		}
		startSolving(1, sdb);
	}
	delete[] record;
}
void Table::GenerateRandomly(unsigned int total, FileHandler*fh)
{
	SdkBuffer* sdb=new SdkBuffer(gBufferSize);
	//generate sudoku solution and write to the file
	for (unsigned int i = 0; i < total; i += gBufferSize)
	{
		sdb->Clear();
		if (total - i <= gBufferSize)
			GenerateRandomly(total - i, sdb);
		else
			GenerateRandomly(gBufferSize, sdb);
		fh->WriteSdb(sdb);
	}
}
void Table::DigRandomCells(SdkBuffer* pBoards,unsigned int lower, 
                           unsigned int upper, bool isAnswerUnique)
{
	srand(clock());
	for (unsigned int i = 0; i < pBoards->GetSize(); i++)
	{
		pBoards->Get(i, cells);
		int r = rand() % (upper - lower+1) + lower;
		if (isAnswerUnique)
		{
			digSpecNumUniquely(cells,r);
		}
		else
		{
			digSpecNum(cells, r);
		}
		pBoards->Set(i,cells);
	}
}

unsigned int Table::Solve(SdkBuffer* pBoard)
{
	unsigned int total=0;
	SdkBuffer result(1);
	for (unsigned int i = 0; i < pBoard->GetSize(); i++)
	{
		pBoard->Get(i, cells);
		int r=startSolving(1, &result);
		if (r != 0) {
			result.Pop(cells);
			pBoard->Set(i, cells);
			total += 1;
		}
	}
	return total;
}

void Table::Solve(FileHandler* src, FileHandler*dst)
{
	//$todo(Felix):add argument checking in debug mode
	SdkBuffer sdb(gBufferSize);
	while (src->HasNext())
	{
		sdb.Clear();
		src->ReadSdb(&sdb);
		Solve(&sdb);
		dst->WriteSdb(&sdb);
	}
}
int Table::Solvable(SdkBuffer* pBoard, int index)
{
	pBoard->Get(index, cells);
	return startSolving(1, NULL);
}
void Table::digSpecNum(int table[][9] , unsigned int num)
{
	srand(clock());
	int index[81];
	for (int i = 0; i < 81; i++)
	{
		index[i] = i;
	}
	for (int i = 80; i >= 1; i--)
	{
		int r = rand()%(i+1);
		int mid = index[r];
		index[r] = index[i];
		index[i] = mid;
	}
	for (unsigned int i = 0; i < num; i++)
	{
		unsigned int row = index[i] / 9;
		unsigned int col = index[i] % 9;
		table[row][col] = 0;
	}
}
void Refill(SdkBuffer* solution, int rawCells[][9])
{
	int maxDiff = 0;
	int maxDiffPos = 0;
	int minAppearVal = 0;
	int row=0, col=0;
	int cells[9][9];
	for (int i = 0; i < 81; i++)
	{
		row = i / 9;
		col = i % 9;
		int values[9] = { 0,0,0,0,0,0,0,0,0};
		for (int j = 0; j < solution->GetSize(); j++)
		{
			solution->Get(j, cells);
			values[cells[row][col]-1]+=1;
		}
		int diff=0, minCount=0,minValue=0;
		for (int i = 0; i < 9; i++)
		{
			if (values[i] != 0)
			{
				diff++;
				if (minCount == 0 || values[i] < minCount)
				{
					minCount = values[i];
					minValue = i + 1;
				}
			}
		}
		if (diff > maxDiff)
		{
			maxDiff = diff;
			maxDiffPos = i;
			minAppearVal = minValue;
		}
	}
	row = maxDiffPos / 9;
	col = maxDiffPos % 9;
	rawCells[row][col] = minAppearVal;
}
void Table::digSpecNumUniquely(int table[][9], unsigned int num)
{
	const int  maxSolution = 10;
	//TODO : Do we need to check whether the sudoku is valid?
	srand(clock());
	for (unsigned int i = 0; i < num; i++)
	{
		int r = rand() % 81;
		int row = r / 9;
		int col = r % 9;
		if (table[row][col] == 0)
		{
			i -= 1;
			continue;
		}
		
		int value = table[row][col];
		table[row][col] = 0;

		//Set
		SdkBuffer sdb(1);
		sdb.Fill(table);
		sdb.Pop(cells);
		SdkBuffer solution(maxSolution);
		int total=startSolving(maxSolution,&solution);
		if (total >1)
		{
			i--;
			Refill(&solution, table);
			//table[row][col] = value;
			continue;
		}
	}
}
int*  Table::lookUp(int rst, int  cst, int num)
{
	//$todo(Felix):add more comments
	//$todo(Felix):improve its effeciency
	int *result=new int[10];
	int index=0;
	int ron=0,con=0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			ron = i + rst * 3;
			con = j + cst * 3;
			if (cells[ron][con] == num)
			{
				delete result;
				return NULL;
			}
			if (cells[ron][con] != 0)
				continue;
			bool pass=true;
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
				result[index++] = ron*9+con;
			}
		}
	}
//	printf("%d", index);
	result[index] = -1;
	return result;
}
unsigned int Table::startSolving(unsigned int maxAnswer,SdkBuffer* pResult)
{
	unsigned int total=0;
	solve(0,1, total,maxAnswer,pResult);
	return total;
}
void Table::solve(int subt, int num,unsigned int &total,unsigned int&top,SdkBuffer* pCurrentBuffer)
{
	//this function works recursively to fill number 1-9 one by one to all the 9 sub tables
	//num (1-9)  is the current  number we try to fill in  
	
 //	printf("%d %d\n", subt, num);
	//if we get enough solutions ,just exit
	if (total == top)
		return;
	//It signals that all numbers are filled.   
	else if (num == 10)
	{
		total += 1;
		if(pCurrentBuffer!=NULL)
		{
			pCurrentBuffer->Fill(cells);
			}
		return;
	}
	//we should try the next number while that all palaces are reached
	else  if (subt == 9)
	{
		solve(0, num + 1,total,top,pCurrentBuffer);
		return;
	}
	//Row or Column of SubTable
	int rst = subt / 3;
	int cst = subt % 3;
	//suitable cells 
	int * suitcells = lookUp(rst, cst, num);
	if (suitcells == NULL)
	{
		solve(subt + 1, num,total,top,pCurrentBuffer);
		return;
	}
	int index = 0;
	//Row or Column of Number
	int ron = 0;
	int con = 0;
	while (suitcells[index] != -1 && total!=top)
	{
		ron = suitcells[index] / 9;
		con = suitcells[index] % 9;
		cells[ron][con] = num;
		solve(subt + 1, num,total,top,pCurrentBuffer);
		cells[ron][con] = 0;
		index++;
	}
	delete[] suitcells;
}
void Table::setZero()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			cells[i][j] = 0;
		}
	}
}
Table::~Table()
{

}
