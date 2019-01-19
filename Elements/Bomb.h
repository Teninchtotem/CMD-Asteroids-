#pragma once
#include "Element.h"
using namespace std;

// code shell, amend as appropriate

class Bomb: public Element
{
private:
	int timer;
public:
	Bomb(RandomNumber &rnd_, Console &console_, int boxSizeIn);
	~Bomb();

	void printAt();
	// Description: This version of printAt() takes care of powering down the element after 50 "ticks"

	void hideAt();

};