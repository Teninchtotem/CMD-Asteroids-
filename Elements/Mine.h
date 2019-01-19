#pragma once
#include "Element.h"
using namespace std;

// code shell, amend as appropriate

class Mine: public Element
{
private:
	int timer;
	int beginTimer;
public:
	Mine(RandomNumber &rnd_, Console &console_, int boxSizeIn);
	~Mine();

	void printAt();
	// Description: This version of printAt() takes care of powering down the element after 50 "ticks"

	void hideAt();

};