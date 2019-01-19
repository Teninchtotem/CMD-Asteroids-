#include "Element.h"
#include <iostream>
using namespace std;

Element::Element(RandomNumber &rnd_, Console &console_, int boxSizeIn) 
	: rnd(rnd_), console(console_)
{
	//Randomising variables where appropriate and setting others releative to them
	this->solidWalls = false;
	this->type = 0;
	this->boxSize = boxSizeIn;
	this->x = rnd.random(1,boxSize - 1);

	int min = (boxSize/2) - 7;
	int max = (boxSize/2) + 7;

	while(x > min && x < max)
		this->x = rnd.random(1,boxSize - 1);
	while(y > min && y < max)
		this->y = rnd.random(1,boxSize - 1);

	this->y = rnd.random(1,boxSize - 1);
	this->xLast = this->x;
	this->yLast = this->y;
	this->direction = rnd.random(0,359);
	this->speed = rnd.random(1,10);
	this->energy = rnd.random(1,60);
	this->identifier = this->nextIdentifier;
	this->nextIdentifier++;
	this->powerDown = false;

	//Setting speed to a fraction of the origeonal (speed should not normally be greater than 1)
	this->speed = this->speed/10;

	//Calcualting Tragectory with randomised variables
	calculateTragectory(this->direction, this->speed);
}
Element::~Element()
{
	solidWalls = 0;
	type = 0;
	boxSize = 0;
	x = 0;
	xLast = 0;
	xDiff = 0;
	xCount = 0;
	y = 0;
	yLast = 0;
	yDiff = 0;
	yCount = 0;
	powerDown = 0;
	energy = 0;
	direction = 0;
	speed = 0;
	identifier = 0;
}
void Element::setSolidWalls(bool solid)
{
	solidWalls = solid;
}
void Element::calculateTragectory(double directionIn, double speedIn)
{
	direction = directionIn;
	speed = speedIn;

	xDiff = 0;
	xCount = 0;

	yDiff = 0;
	yCount = 0;

	//Converting direction to radians
	direction = direction*(M_PI/180);

	//Calculating x and y offset per tick
	xCount = cos(direction)*speed;
	yCount = (sin(direction)*speed) * -1;

	//Converting direction back to degrees
	direction = direction/(M_PI/180);
}
void Element::print(void)
{
	cout << identifier << endl;
	cout << x << endl;
	cout << y << endl;
}
void Element::moveElementBack()
{ 
	x = xLast;
	y = yLast;
}
void Element::printAt()
{

}
void Element::previousIdentifier()
{
	nextIdentifier--;
}
int Element::getType()
{
	return type;  
}
void Element::setIdentifier(char identIn)
{
	identifier = identIn;
}
void Element::hideAt()
{

}
char Element::getIdentifier()
{
	return identifier;
}
void Element::setXCount(double XCountIn)
{
	xCount = XCountIn;

	//Capping, if greater than 1 (positive or negative)
	if(xCount > 1)
		xCount = 1;
	else if(xCount < -1)
		xCount = -1;
}
void Element::setYCount(double yCountIn)
{
	yCount = yCountIn;

	//Capping, if greater than 1 (positive or negative)
	if(yCount > 1)
		yCount = 1;
	else if(yCount < -1)
		yCount = -1;
}
double Element::getXCount()
{
	return xCount;
}
double Element::getYCount()
{
	return yCount;
}
void Element::moveElement()
{
	//Recording where it was this turn for future reference
	xLast = x;
	yLast = y;

	//Moving Object for this "tick"
	xDiff += xCount;

	if(xDiff >= 1)
	{
		x++;
		xDiff--;
	}
	else if(xDiff <= -1)
	{
		x--;
		xDiff++;
	}

	yDiff += yCount;

	if(yDiff >= 1)
	{
		y++;
		yDiff--;
	}
	else if(yDiff <= -1)
	{
		y--;
		yDiff++;
	}

	//Looping the box so that it will come back the other side
	if(!solidWalls)
	{
		if(x < 0)
			x += boxSize;
		if(y < 0)
			y += boxSize;
		if(x >= boxSize)
			x -= boxSize;
		if(y >= boxSize)
			y -= boxSize;
	}
	//Reflecting off the edge of the box
	else
	{
			bool triggerX = false;
			bool triggerY = false;
			 
			if(x < 0)
				triggerX = true;
			if(y < 0)
				triggerY = true;
			if(x >= boxSize)
				triggerX = true;
			if(y >= boxSize)
				triggerY = true;

			if(triggerX || triggerY)
			{
				moveElementBack();

				if(type < 4)
				{
					if(triggerX && triggerY)
						direction -= 180;
					else if(triggerX)
					{
						direction = 90 - (direction - 90);
					}
					else if(triggerY)
					{
						direction = 360 - direction;
					}

					if(direction < 0)
						direction += 360;

					calculateTragectory(direction, speed);
					moveElement();
				}
				else
					powerDown = true;
			}
	}
}
void Element::calculateColission(double otherObjectdirection, double otherObjectSpeed, int otherObjectEnergy)
{
	//Calculating new energy after colission:
	//			Difference in energy is done as damage to the Element with the least energy
	//			The other Element takes half this much damage
	if(energy < otherObjectEnergy)
		energy -= (otherObjectEnergy - energy)/2;
	else
		energy -= (energy- otherObjectEnergy)/4;
	
	//Activating powerdown if energy is below 0 (above 100 is to catch the bullet and automatically kill it in any colission)
	if((energy <= 0)||(energy > 100))
		powerDown = true;
	
	//Calculating new tragectory based off the other objects information
	calculateTragectory(otherObjectdirection, otherObjectSpeed);
	
}
void Element::powerUp()
{
	powerDown = false;
}
void Element::resetNextIdentifier()
{
	nextIdentifier = 'A';
}
int Element::getX()
{
	return x;
}
int Element::getY()
{
	return y;
}
int Element::getEnergy()
{
	return energy;
}
double Element::getDirection()
{	
	return direction;
}
double Element::getSpeed()
{
	return speed;
}
void Element::setX(int xIn)
{
	x = xIn;
}
void Element::setY(int yIn)
{
	y = yIn;
}
void Element::setEnergy(int energyIn)
{
	energy = energyIn;
}
void Element::setDirection(int directionIn)
{
	direction = directionIn;
}
void Element::setSpeed(int speedIn)
{
	speed = speedIn/10;
}
bool Element::getPowerDown()
{
	return powerDown;
}
// definition of static attribute
char Element::nextIdentifier = 'A';