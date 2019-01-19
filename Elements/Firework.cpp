#include "Firework.h"

Firework::Firework(RandomNumber &rnd_, Console &console_, int boxSizeIn)
	:Element (rnd, console, boxSizeIn)
{
	//Timer is how many 'ticks' of the board the Firework will last. Increese to make the Fireworks live longer
	timer = rnd.random(3,10);
	xDiff = 0;
	xCount = 0;
	yDiff = 0;
	yCount = 0;
	x = rnd.random(6 , boxSize - 6);
	y = rnd.random(6 , boxSize - 6);
	energy = 10000;
	identifier = 57;

	type = 6;
}
Firework::~Firework()
{
	timer = 0;
}
void Firework::printAt()
{
	//Counting down untill the Firework dies and powering down
	timer--;
	if(timer <= 0)
		powerDown = 1;

	// move cursor to the current x, y coordinates
	console.gotoXY(x+1,y+1);

	//Invisible
	console.setColour(Console::BLACK, Console::BLACK);

	// output identifier
	cout << identifier;

	// change colour back to default
	console.setColour(Console::DEFAULT, Console::BLACK);
}
void Firework::hideAt()
{
	console.gotoXY(x+1,y+1);
	cout << " ";
	console.setColour(Console::DEFAULT, Console::BLACK);
}
