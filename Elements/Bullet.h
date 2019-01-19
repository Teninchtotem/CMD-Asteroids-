#pragma once
#include "Element.h"
using namespace std;

// code shell, amend as appropriate

class Bullet: public Element
{
private:
	int timer;
public:
	Bullet(RandomNumber &rnd_, Console &console_, int boxSizeIn);
	~Bullet();

	void printAt();
	// Description: This version of printAt() takes care of powering down the element after 50 "ticks"

	void hideAt();

};