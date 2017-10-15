#include "stdafx.h"
#include <string>
#include "Core.h"
#include "Table.h"
#include "DifficultyEvaluation.h"
using namespace std;
int sum(int a, int b)
{
	return a + b;
}
void generate(int number, int mode, int result[][81])
{
	if (number < 0)
	{
		throw invalid_argument("The argument of \"-n\" shouldn`t be less than zero");
	}
	else if (number > gMaxGenRanAmount)
	{
		throw invalid_argument("The argument of \"-n\" shouldn`t be bigger than 10000");
	}
	else if (mode < 1 || mode>3)
	{
		throw invalid_argument("The argument of \"-m\" should be in the range of [1,3]");
	}
	else if(result==NULL)
	{
		throw invalid_argument("The argument of result shouldn`t be NULL");
	}
	Table table;
	SdkBuffer board(1);
	int buffer[9][9];
	Difficulty diff;
	DifficultyEvaluation diev;
	for (int i = 0; i < number; i++)
	{
		table.GenerateRandomly(1, &board);
		table.DigRandomCells(&board, gRange[mode-1][0], gRange[mode-1][1], true);
		board.Get(0, buffer);
		diff=diev.Evaluate(buffer);
		if (diff != mode)
		{
			i--;
			board.Clear();
			continue;
		}
		board.ToArray(result + i);
		board.Clear();
	}
}
void generate(int number, int lower, int upper, bool unique, int result[][81])
{
	if (number < 0)
	{
		throw invalid_argument("The argument of \"-n\" shouldn`t be less than zero");
	}
	else if (number > gMaxGenRanAmount)
	{
		throw invalid_argument("The argument of \"-n\" shouldn`t be bigger than 10000");
	}
	else if (lower < gGenRangeLower || lower >gGenRangeUpper ||
			 upper < gGenRangeLower || upper >gGenRangeUpper)
	{
		throw invalid_argument("The argument of \"-r\" should be in the range of [20,55]");
	}
	else if (lower > upper)
	{
		throw invalid_argument("The argument of \"-r\" should be like lower~upper.");
	}
	else if (result == NULL)
	{
		throw invalid_argument("The argument of result shouldn`t be NULL");
	}
	Table table;
	SdkBuffer *buffer=new SdkBuffer(gBufferSize);
	for (unsigned int i = 0; i < number; i += gBufferSize)
	{
		if (i + gBufferSize > number)
		{
			table.GenerateRandomly(number - i, buffer);
		}
		else
		{
			table.GenerateRandomly(gBufferSize, buffer);
		}
		table.DigRandomCells(buffer, lower, upper, unique);
		buffer->ToArray(result + i);
		buffer->Clear();
	}
}
bool solve(int puzzle[], int solution[81])
{
	if (puzzle == NULL)
	{
		throw invalid_argument("The argument of puzzle shouldn`t be NULL");
	}
	else if (solution == NULL)
	{
		throw invalid_argument("The argument of solution shouldn`t be NULL");
	}
	int cells[9][9];
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (puzzle[i * 9 + j] < 0 || puzzle[i * 9 + j]>9)
			{
				string info = "The argument puzzle has a wrong format.Exception at pos";
				char buffer[10];
				info += buffer;
				sprintf_s(buffer, " [%d,%d]", i, j);
				throw invalid_argument(info);
			}
			cells[i][j] = puzzle[i * 9 + j];
		}
	}
	int result;
	SdkBuffer board(1);
	board.Fill(cells);
	Table table;
	result=table.Solve(&board);
	board.Pop(solution);
	if (result == 0)//no answer 
	{
		return false;
	}
	else
	{
		return true;
	}
}
void generate(int number, int result[][81])
{
	if (number < 0)
	{
		throw invalid_argument("The argument of \"-n\" shouldn`t be less than zero");
	}
	else if (number > gMaxGenAmount)
	{
		throw invalid_argument("The argument of \"-n\" shouldn`t be bigger than 10000");
	}
	else if (result == NULL)
	{
		throw invalid_argument("The argument of result shouldn`t be NULL");
	}
	Table table;
	SdkBuffer* buffer=new SdkBuffer(gBufferSize);
	for (unsigned int i = 0; i < number; i += gBufferSize)
	{
		if (i + gBufferSize > number)
		{
			table.GenerateRandomly(number - i, buffer);
		}
		else
		{
			table.GenerateRandomly(gBufferSize, buffer);
		}
		buffer->ToArray(result + i);
		buffer->Clear();
	}
	
}