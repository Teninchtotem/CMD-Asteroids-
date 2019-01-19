#include "Ship.h"

Ship::Ship(RandomNumber &rnd_, Console &console_, int boxSizeIn)
	:Element (rnd, console, boxSizeIn)
{
	tempXCount = 0;
	tempYCount = 0;
	shipShape = '|';
	energy = 20;
	speed = 0;
	xDiff = 0;
	xCount = 0;
	yDiff = 0;
	yCount = 0;
	x = boxSize/2;
	y = boxSize/2;
	calculateTragectory(direction, speed);
	identifier = '|';
	
	type = 3;
}
Ship::~Ship()
{
	shipShape = 0;
	tempXCount = 0;
	tempYCount = 0;
}
void Ship::printAt()
{
	// move cursor to the current x, y coordinates
	console.gotoXY(x+1,y+1);

	//Ship is always white on a black background
	console.setColour(Console::WHITE, Console::BLACK);


	//Rounding xCount and yCount to 1 decimal place to make ship movement smooth
	if(xCount < 0)
		tempXCount = (xCount - 0.05)* 10;
	else
		tempXCount = (xCount + 0.05)* 10;

	xCount = tempXCount;
	xCount = xCount/10;

	if(yCount < 0)
		tempYCount = (yCount - 0.05)* 10;
	else
		tempYCount = (yCount + 0.05)* 10;

	yCount = tempYCount;
	yCount = yCount/10;

	//Calculating speed based on xCount and yCount
	speed = sqrt((xCount*xCount)+(yCount*yCount));

	//Calculating Direction based on xCount and yCount (It gets pretty messy maths wise, just trust me when I say it works :D )
	if((yCount > -0.000000009)&&(yCount < 0.000000009))
	{
		if(xCount < 0)
			direction = 180;
		else
			direction = 0;
	}
	else if((xCount > -0.000000009)&&(xCount < 0.000000009))
	{
		if(yCount < 0)
			direction = 90;
		else
			direction = 270;
	}
	else
	{
		direction = atan(yCount/xCount);
		direction = direction/(M_PI/180);


		if(direction < 0)
			direction = -direction;

		if(xCount < 0)
			if(-yCount < 0)
				direction = 180 + direction;
			else
				direction = 180 - direction;
		else
			if(-yCount < 0)
				direction = 360 - direction; 
	}
	//Setting the shipShape to make the ship face the right way
	if((direction < 23)||(direction > 338))
		shipShape = '-';
	else if(direction < 68)
		shipShape = '/';
	else if(direction < 113)
		shipShape = '|';
	else if(direction < 158)
		shipShape = 92;
	else if(direction < 203)
		shipShape = '-';
	else if(direction < 248)
		shipShape = '/';
	else if(direction < 293)
		shipShape = '|';
	else if(direction <= 338)
		shipShape = 92;

	// output identifier
	cout << shipShape;

	// change colour back to default  
	console.setColour(Console::DEFAULT, Console::BLACK);
}
void Ship::hideAt()
{
	console.gotoXY(x+1,y+1);
	cout << " ";
	console.setColour(Console::DEFAULT, Console::BLACK);
}
