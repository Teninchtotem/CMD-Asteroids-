#pragma once
#include "Element.h"
using namespace std;

// code shell, amend as appropriate

class Jetsam: public Element
{
public:
	Jetsam(RandomNumber &rnd_, Console &console_, int boxSizeIn);
	~Jetsam();

	void printAt();
	// Description: Sandard printAt()

	void hideAt();

};