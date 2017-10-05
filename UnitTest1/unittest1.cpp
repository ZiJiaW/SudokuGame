#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\CaaeSudoku\ArgumentHandler.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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
                ah.GetState() == ArgumentHandler::State::GEN&
                ah.GetLower() == 0 &
                ah.GetUpper() == 0 &
                ah.GetPathName() == NULL&
                ah.GetDifficulty() == ArgumentHandler::Difficulty::UNS&
                ah.GetUnique() == false;
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
                ah.GetState() == ArgumentHandler::State::GEG&
                ah.GetLower() == 0 &
                ah.GetUpper() == 0 &
                ah.GetPathName() == NULL&
                ah.GetDifficulty() == ArgumentHandler::Difficulty::NORMAL&
                ah.GetUnique() == false;
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
                ah.GetState() == ArgumentHandler::State::GEG&
                ah.GetLower() == 0 &
                ah.GetUpper() == 0 &
                ah.GetPathName() == NULL&
                ah.GetDifficulty() == ArgumentHandler::Difficulty::NORMAL&
                ah.GetUnique() == false;
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
                ah.GetState() == ArgumentHandler::State::GEG&
                ah.GetLower() == 23 &
                ah.GetUpper() == 29 &
                ah.GetPathName() == NULL&
                ah.GetDifficulty() == ArgumentHandler::Difficulty::UNS&
                ah.GetUnique() == true;
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
                ah.GetState() == ArgumentHandler::State::GEG&
                ah.GetLower() == 23 &
                ah.GetUpper() == 29 &
                ah.GetPathName() == NULL&
                ah.GetDifficulty() == ArgumentHandler::Difficulty::UNS&
                ah.GetUnique() == true;
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
                ah.GetState() == ArgumentHandler::State::GEG&
                ah.GetLower() == 22 &
                ah.GetUpper() == 37 &
                ah.GetPathName() == NULL&
                ah.GetDifficulty() == ArgumentHandler::Difficulty::UNS&
                ah.GetUnique() == false;
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
                ah.GetState() == ArgumentHandler::State::GEG&
                ah.GetLower() == 22 &
                ah.GetUpper() == 37 &
                ah.GetPathName() == NULL&
                ah.GetDifficulty() == ArgumentHandler::Difficulty::UNS&
                ah.GetUnique() == false;
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
                ah.GetState() == ArgumentHandler::State::GEG&
                ah.GetLower() == 0 &
                ah.GetUpper() == 0 &
                ah.GetPathName() == NULL&
                ah.GetDifficulty() == ArgumentHandler::Difficulty::UNS&
                ah.GetUnique() == true;
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
                ah.GetState() == ArgumentHandler::State::GEG&
                ah.GetLower() == 0 &
                ah.GetUpper() == 0 &
                ah.GetPathName() == NULL&
                ah.GetDifficulty() == ArgumentHandler::Difficulty::UNS&
                ah.GetUnique() == true;
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
                ah.GetState() == ArgumentHandler::State::SOV&
                ah.GetLower() == 0 &
                ah.GetUpper() == 0 &
                strcmp(ah.GetPathName(), "100.txt") == 0 &
                ah.GetDifficulty() == ArgumentHandler::Difficulty::UNS&
                ah.GetUnique() == false;
            Assert::AreEqual(r, true);
        }
	};
}