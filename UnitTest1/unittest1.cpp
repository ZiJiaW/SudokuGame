#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\CaaeSudoku\ArgumentHandler.h"
#include "..\CaaeSudoku\Core.h"
#include "..\CaaeSudoku\DifficultyEvaluation.h"
#include "..\CaaeSudoku\Table.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
void Transform(int p[9][9], int q[81])
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            q[9 * i + j] = p[i][j];
        }
    }
}
bool IsInRange(int lower, int upper, int result[][81], int size)
{
    for (int i = 1; i < size; i++)
    {
        int count = 0;
        for (int k = 0; k < 81; k++)
        {
            if (result[i][k] == 0)
            {
                count++;
            }
        }
        if (count<lower || count>upper)
            return false;
    }
    return true;
}
bool IsTrueAnswer(int p[81], int s[81])
{
    bool r = true;
    for (int i = 0; i < 81; i++)
    {
        r &= (p[i] != 0 ? p[i] == s[i] : true);
    }
    return r;
}
bool IsValid(int s[81])
{
    int pr[9][9];
    for (int i = 0; i < 81; i++)
    {
        pr[i / 9][i % 9] = s[i];
    }
    bool r = true;
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            for (int k = 0; k < 9; ++k)
            {
                r &= (k != j ? pr[i][j] != pr[i][k] : true);
                r &= (k != i ? pr[i][j] != pr[k][j] : true);
            }
            int a = 3 * (i / 3);
            int b = 3 * (j / 3);
            for (int p = a; p < a + 3; ++p)
            {
                for (int q = b; q < b + 3; ++q)
                {
                    r &= ((p != i&&q != j) ? pr[i][j] != pr[p][q] : true);
                }
            }
        }
    }
    return r;
}
bool IsDiffer(int p[81], int q[81])
{
    bool r = true;
    for (int i = 0; i < 81; i++)
    {
        r &= p[i] == q[i];
    }
    return !r;
}
bool IsDiffer(int pr[][81],int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (!IsDiffer(pr[i], pr[j]))
                return false;
        }
    }
    return true;
}
namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		// test ArgumentHandler
		TEST_METHOD(TestMethod1)
		{
			// -c 98897
            int argc = 3;
            char *argv[] = { "sudoku.exe","-c","98897" };
            ArgumentHandler ah;
            ah.ParseInput(argc, argv);
            bool r = ah.GetCount() == 98897 &
                ah.GetState() == State::GEN&
                ah.GetLower() == 0 &
                ah.GetUpper() == 0 &
                ah.GetPathName() == NULL&
                ah.GetDifficulty() == Difficulty::UNS;
            Assert::AreEqual(r, true);
		}
        TEST_METHOD(TestMethod2)
        {
            // -n 9999 -m 2
            int argc = 5;
            char *argv[] = { "sudoku.exe", "-n","9999", "-m", "2" };
            ArgumentHandler ah;
            ah.ParseInput(argc, argv);
            bool r = ah.GetCount() == 9999 &
                ah.GetState() == State::GEG_M&
                ah.GetLower() == 0 &
                ah.GetUpper() == 0 &
                ah.GetPathName() == NULL&
                ah.GetDifficulty() == Difficulty::NORMAL;
            Assert::AreEqual(r, true);
        }
        TEST_METHOD(TestMethod3)
        {
            // -m 2 -n 7542
            int argc = 5;
            char *argv[] = { "sudoku.exe", "-m", "2", "-n","7542" };
            ArgumentHandler ah;
            ah.ParseInput(argc, argv);
            bool r = ah.GetCount() == 7542 &
                ah.GetState() == State::GEG_M&
                ah.GetLower() == 0 &
                ah.GetUpper() == 0 &
                ah.GetPathName() == NULL&
                ah.GetDifficulty() == Difficulty::NORMAL;
            Assert::AreEqual(r, true);
        }
        TEST_METHOD(TestMethod4)
        {
            // -n 124 -r 23~29 -u
            int argc = 6;
            char *argv[] = { "sudoku.exe", "-n","124", "-r", "23~29", "-u" };
            ArgumentHandler ah;
            ah.ParseInput(argc, argv);
            bool r = ah.GetCount() == 124 &
                ah.GetState() == State::GEG_RU&
                ah.GetLower() == 23 &
                ah.GetUpper() == 29 &
                ah.GetPathName() == NULL&
                ah.GetDifficulty() == Difficulty::UNS;
            Assert::AreEqual(r, true);
        }
        TEST_METHOD(TestMethod5)
        {
            // -r 23~29 -u -n 124 
            int argc = 6;
            char *argv[] = { "sudoku.exe", "-r", "23~29", "-u" ,"-n","124" };
            ArgumentHandler ah;
            ah.ParseInput(argc, argv);
            bool r = ah.GetCount() == 124 &
                ah.GetState() == State::GEG_RU&
                ah.GetLower() == 23 &
                ah.GetUpper() == 29 &
                ah.GetPathName() == NULL&
                ah.GetDifficulty() == Difficulty::UNS;
            Assert::AreEqual(r, true);
        }
        TEST_METHOD(TestMethod6)
        {
            // -r 22~37 -n 1675
            int argc = 5;
            char *argv[] = { "sudoku.exe", "-r", "22~37", "-n","1675" };
            ArgumentHandler ah;
            ah.ParseInput(argc, argv);
            bool r = ah.GetCount() == 1675 &
                ah.GetState() == State::GEG_R&
                ah.GetLower() == 22 &
                ah.GetUpper() == 37 &
                ah.GetPathName() == NULL&
                ah.GetDifficulty() == Difficulty::UNS;
            Assert::AreEqual(r, true);
        }
        TEST_METHOD(TestMethod7)
        {
            // -n 1675 -r 22~37
            int argc = 5;
            char *argv[] = { "sudoku.exe", "-n", "1675", "-r", "22~37"};
            ArgumentHandler ah;
            ah.ParseInput(argc, argv);
            bool r = ah.GetCount() == 1675 &
                ah.GetState() == State::GEG_R&
                ah.GetLower() == 22 &
                ah.GetUpper() == 37 &
                ah.GetPathName() == NULL&
                ah.GetDifficulty() == Difficulty::UNS;
            Assert::AreEqual(r, true);
        }
        TEST_METHOD(TestMethod8)
        {
            // -n 11 -u
            int argc = 4;
            char *argv[] = { "sudoku.exe", "-n", "11", "-u" };
            ArgumentHandler ah;
            ah.ParseInput(argc, argv);
            bool r = ah.GetCount() == 11 &
                ah.GetState() == State::GEG_U&
                ah.GetLower() == 0 &
                ah.GetUpper() == 0 &
                ah.GetPathName() == NULL&
                ah.GetDifficulty() == Difficulty::UNS;
            Assert::AreEqual(r, true);
        }
        TEST_METHOD(TestMethod9)
        {
            // -u -n 11 
            int argc = 4;
            char *argv[] = { "sudoku.exe", "-u","-n", "11" };
            ArgumentHandler ah;
            ah.ParseInput(argc, argv);
            bool r = ah.GetCount() == 11 &
                ah.GetState() == State::GEG_U&
                ah.GetLower() == 0 &
                ah.GetUpper() == 0 &
                ah.GetPathName() == NULL&
                ah.GetDifficulty() == Difficulty::UNS;
            Assert::AreEqual(r, true);
        }
        TEST_METHOD(TestMethod10)
        {
            // -s 100.txt
            int argc = 3;
            char *argv[] = { "sudoku.exe", "-s","100.txt" };
            ArgumentHandler ah;
            ah.ParseInput(argc, argv);
            bool r = ah.GetCount() == 0 &
                ah.GetState() == State::SOV&
                ah.GetLower() == 0 &
                ah.GetUpper() == 0 &
                strcmp(ah.GetPathName(), "100.txt") == 0 &
                ah.GetDifficulty() == Difficulty::UNS;
            Assert::AreEqual(r, true);
        }
        // test API generetor
        TEST_METHOD(TestMethod11)
        {
            int p[9][9] = {
                { 0,0,0,0,0,0,8,0,0 },
                { 0,1,0,2,3,5,4,0,0 },
                { 0,0,9,0,6,0,0,5,3 },
                { 0,6,0,3,0,0,0,4,0 },
                { 0,9,5,0,8,0,1,3,0 },
                { 0,7,0,0,0,2,0,8,0 },
                { 9,5,0,0,2,0,3,0,0 },
                { 0,0,3,1,7,6,0,9,0 },
                { 0,0,7,0,0,0,0,0,0 }
            };
            int puzzle[81];
            Transform(p, puzzle);
            int solution[81];
            bool r = solve(puzzle, solution);
            r &= IsTrueAnswer(puzzle, solution);
            r &= IsValid(solution);
            Assert::AreEqual(r, true);
        }
        TEST_METHOD(TestMethod12)
        {
            int p[9][9] = {
                { 8,0,0,0,0,0,0,0,3 },
                { 0,0,2,3,5,0,0,0,0 },
                { 0,6,0,0,8,9,0,0,0 },
                { 0,8,0,0,0,2,3,0,0 },
                { 0,9,3,0,0,0,1,7,0 },
                { 0,0,5,6,0,0,0,8,0 },
                { 0,0,0,5,6,0,0,1,0 },
                { 0,0,0,0,2,7,4,0,0 },
                { 6,0,0,0,0,0,0,0,7 }
            };
            int puzzle[81];
            Transform(p, puzzle);
            int solution[81];
            bool r = solve(puzzle, solution);
            r &= IsTrueAnswer(puzzle, solution);
            r &= IsValid(solution);
            Assert::AreEqual(r, true);
        }
        TEST_METHOD(TestMethod13)
        {
            int size = 1000;
            int(*result)[81] = new int[size][81];
            generate(size, result);
            bool r = IsDiffer(result, size);
            for (int i = 0; i < size; i++)
                r &= IsValid(result[i]);
            delete[] result;
            Assert::AreEqual(r, true);
        }
        TEST_METHOD(Test14)
        {
            // -n 100 -r 23~29 
            int size = 1000;
            int(*result)[81] = new int[size][81];
            generate(size, 23, 29, false, result);
            bool r = IsInRange(23, 29, result, size);
            int solution[81];
            for (int i = 0; i < size; i++)
            {
                r &= solve(result[i], solution);
                r &= IsValid(solution);
            }
            Assert::AreEqual(r, true);
        }
        TEST_METHOD(Test15)
        {
            // -n 100 -r 40~50 -u
            int size = 100;
            int(*result)[81] = new int[size][81];
            generate(size, 40, 50, true, result);
            bool r = IsInRange(40, 50, result, size);
            int solution[81];
            for (int i = 0; i < size; i++)
            {
                r &= solve(result[i], solution);
                r &= IsValid(solution);
            }
            Table table;
            for (int k = 0; k < size; k++)
            {
                for (int i = 0; i < 81; i++)
                {
                    table.cells[i / 9][i % 9] = result[k][i];
                }
                r &= table.startSolving(2, NULL) == 1;
            }

            Assert::AreEqual(r, true);
        }
        TEST_METHOD(Test16)
        {
            // -n 1000 -m 1
            int size = 10;
            int(*result)[81] = new int[size][81];
            generate(size, Difficulty::EASY, result);
            DifficultyEvaluation e;
            bool r = true;
            int p[9][9];
            for (int i = 0; i < size; i++)
            {
                for (int k = 0; k < 81; k++)
                {
                    p[k / 9][k % 9] = result[i][k];
                }
                r &= e.Evaluate(p) == Difficulty::EASY;
            }
            int solution[81];
            for (int i = 0; i < size; i++)
            {
                r &= solve(result[i], solution);
                r &= IsValid(solution);
            }
            Table table;
            for (int k = 0; k < size; k++)
            {
                for (int i = 0; i < 81; i++)
                {
                    table.cells[i / 9][i % 9] = result[k][i];
                }
                r &= table.startSolving(2, NULL) == 1;
            }
            Assert::AreEqual(r, true);
        }
	};
}