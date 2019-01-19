#include "Bomb.h"

Bomb::Bomb(RandomNumber &rnd_, Console &console_, int boxSizeIn)
	:Element (rnd, console, boxSizeIn)
{
	//Timer is how many 'ticks' of the board the Bomb will last. Increese to make the Bombs live longer
	timer = 3;
	xDiff = 0;
	xCount = 0;
	yDiff = 0;
	yCount = 0;
	energy = 10000;
	identifier = '#';

	type = 5;
}
Bomb::~Bomb()
{
	timer = 0;
}
void Bomb::printAt()
{
	//Counting down untill the Bomb dies and powering down
	timer--;
	if(timer <= 0)
		powerDown = 1;

	// move cursor to the current x, y coordinates
	console.gotoXY(x+1,y+1);

	if(timer % 2 == 1)
		console.setColour(Console::RED, Console::RED);
	else
		console.setColour(Console::YELLOW, Console::YELLOW);

	// output identifier
	cout << identifier;

	// change colour back to default
	console.setColour(Console::DEFAULT, Console::BLACK);
}
void Bomb::hideAt()
{
	console.gotoXY(x+1,y+1);
	cout << " ";
	console.setColour(Console::DEFAULT, Console::BLACK);
}
