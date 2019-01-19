#pragma once
#include "Board.h"
#include "Console.h"
#define _WIN32_WINNT 0x500
using namespace std;

// This solution demonstrates the use of a STL vector and list in addition to using arrays.

int main()
{
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	//Creating board and running it, the parameter is how large the box should be
	//Example: board(40) gives you a 40x40 box
	Board board(50);
	board.display();
}

