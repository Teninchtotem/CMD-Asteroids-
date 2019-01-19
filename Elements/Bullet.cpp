#include "Bullet.h"

Bullet::Bullet(RandomNumber &rnd_, Console &console_, int boxSizeIn)
	:Element (rnd, console, boxSizeIn)
{
	//Timer is how many 'ticks' of the board the Bullet will last. Increese to make the bullets live longer
	timer = boxSize*1.2;
	xDiff = 0;
	xCount = 0;
	yDiff = 0;
	yCount = 0;
	energy = 10000;
	identifier = '#';

	type = 7;
}
Bullet::~Bullet()
{
	timer = 0;
}
void Bullet::printAt()
{
	//Counting down untill the Bullet dies and powering down
	timer--;
	if(timer <= 0)
		powerDown = 1;

	// move cursor to the current x, y coordinates
	console.gotoXY(x+1,y+1);
	// change colour (Bullet is always white on white)
	if(timer % 2 == 1)
		console.setColour(Console::RED, Console::RED);
	else if(timer < 10)
		console.setColour(Console::LIGHT_PURPLE, Console::LIGHT_PURPLE);
	else
		console.setColour(Console::WHITE, Console::WHITE);

	// output identifier
	cout << identifier;

	// change colour back to default
	console.setColour(Console::DEFAULT, Console::BLACK);
}
void Bullet::hideAt()
{
	console.gotoXY(x+1,y+1);
	cout << " ";
	console.setColour(Console::DEFAULT, Console::BLACK);
}
