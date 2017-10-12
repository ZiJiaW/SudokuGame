#pragma once
#include "stdafx.h"
#define DLLOUTPUT
#ifdef DLLOUTPUT
_declspec(dllexport) int sum(int a, int b);
_declspec(dllexport) void generate(int number, int mode, int result[][81]);
_declspec(dllexport) void generate(int number, int lower, int upper, bool unique, int result[][81]);
_declspec(dllexport) bool solve(int puzzle[81], int solution[81]);
_declspec(dllexport) void generate(int number, int result[][81]);
#endif 
