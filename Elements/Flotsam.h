#pragma once
#include "Element.h"
using namespace std;

// code shell, amend as appropriate

class Flotsam: public Element
{
public:
	Flotsam(RandomNumber &rnd_, Console &console_, int boxSizeIn);
	~Flotsam();

	void printAt();
	// Description: Standard printAt()

	void hideAt();

};