#include "Flotsam.h"

Flotsam::Flotsam(RandomNumber &rnd_, Console &console_, int boxSizeIn)
	:Element (rnd, console, boxSizeIn)
{
	type = 1;
}
Flotsam::~Flotsam()
{

}
void Flotsam::printAt()
{
	// move cursor to the current x, y coordinates
	console.gotoXY(x+1,y+1);
	// change colour proportionate to the energy
	if(energy<10)
		console.setColour(Console::WHITE, Console::BLUE);
	else if(energy<20)
		console.setColour(Console::WHITE, Console::GREEN);
	else if(energy<30)
		console.setColour(Console::WHITE, Console::AQUA);
	else if(energy<40)
		console.setColour(Console::WHITE, Console::YELLOW);
	else if(energy<50)
		console.setColour(Console::WHITE, Console::RED);
	else
		console.setColour(Console::WHITE, Console::PURPLE);

	// output identifier
	cout << identifier;

	// change colour back to default
	console.setColour(Console::DEFAULT, Console::BLACK);
}
void Flotsam::hideAt()
{
	console.gotoXY(x+1,y+1);
	cout << " ";
	console.setColour(Console::DEFAULT, Console::BLACK);
}
