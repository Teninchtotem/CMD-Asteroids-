#include "Mine.h"

Mine::Mine(RandomNumber &rnd_, Console &console_, int boxSizeIn)
	:Element (rnd, console, boxSizeIn)
{
	type = 4;

	xDiff = 0;
	xCount = 0;
	yDiff = 0;
	yCount = 0;
	energy = 10007;
	identifier = 58;
}
Mine::~Mine()
{

}
void Mine::printAt()
{
	energy--;
	if(energy < 9500)
		energy = 10000;

	// move cursor to the current x, y coordinates
	console.gotoXY(x+1,y+1);

	// change colour (Mine is starts to blink when primed)
	if(energy <= 10000)
	{
		if(energy % 10 == 0)
			console.setColour(Console::WHITE, Console::WHITE);
		else
			console.setColour(Console::RED, Console::RED);
	}
	else
		console.setColour(Console::RED, Console::RED);


	// output identifier
	cout << identifier;

	// change colour back to default
	console.setColour(Console::DEFAULT, Console::BLACK);
}
void Mine::hideAt()
{
	console.gotoXY(x+1,y+1);
	cout << " ";
	console.setColour(Console::DEFAULT, Console::BLACK);
}
