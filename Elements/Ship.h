#pragma once
#include "Element.h"
using namespace std;

// code shell, amend as appropriate

class Ship: public Element
{
private:
	char shipShape;
	int tempXCount;
	int tempYCount;
public:
	Ship(RandomNumber &rnd_, Console &console_, int boxSizeIn);
	~Ship();

	void printAt();
	// Description: This version of printAt() is constantly updating the direction and speed of the element
	// Because of the way that the ship moves

	void hideAt();

};