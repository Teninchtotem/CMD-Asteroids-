#pragma once
#include "RandomNumber.h"
#include "Console.h"
#include <math.h>
#define M_PI 3.14159265358979323846
#include <iostream>
#include <conio.h>
using namespace std;

// code shell, amend as appropriate

class Element
{
protected:
	RandomNumber &rnd;
	Console &console;

	bool solidWalls;
	int type;
	int boxSize;
	int x;
	int xLast;
	double xDiff;
	double xCount;
	int y;
	int yLast;
	double yDiff;
	double yCount;
	bool powerDown;
	int energy;
	double direction;
	double speed;
	char identifier;
	static char nextIdentifier;

public:
	Element(RandomNumber &rnd_, Console &console_, int boxSizeIn);
	~Element();
	// precondition: references to RandomNumber and Console objects are provided, along with the size of the box that will be made
	// postcondition: Element object created and private members initialised as well as a box being drawn that surrounds the board
	// example of use: Element element(rnd, console);

	void print(void);
	// precondition: none
	// postcondition: identifier, x and y are sent to the standard output
	// example of use: element.print();

	virtual void printAt(void) = 0;
	// precondition: none
	// postcondition: text background is changed proportionate to its energy in the following order
	// BLUE, GREEN, AQUA, YELLOW, RED, PURPLE, e.g. an object with 23 energy would have an AQUA background
	// object's identifier is sent to the standard output at its x, y coordinates
	// example of use: element.printAt();

	virtual void hideAt(void) = 0;
	// Precondition: none
	// Description: At the location of the element, the identifier will be replaced with a space (" ")

	void moveElement(); 
	// Description: Updates x and y co-ordinates to where it will be in 1 "tick" 
	
	void moveElementBack();
	// Description: Updates x and y co-ordinates to where they were last "tick"

	void previousIdentifier();
	//Sets the next identifier back one

	void calculateTragectory(double directionIn, double speedIn);
	// Precondition: Two doubles, direction (0-359) and speed (0-1)
	// Postcondition: xCount and yCount updated
	// Description: Calculates new direction and speed for the element based on the inputs
	// Example: calculateTragectory(180, 0.5)
	
	int getType();
	//Description: Will return a value coresponding to what type of element it is

	void calculateColission(double otherObjectdirection, double otherObjectSpeed, int otherObjectEnergy);
	// Precondition: Requires information on the object that the element is coliding with (direction, speed, energy)
	// Postcondition: calculates new tragectory
	// Description: calculates new tragectory from information based on the other element in the colission
	// Example: calculateColission(180, 0.5, 40)

	void powerUp();
	// Description: Restarts an element to make it so it is back to being powered on

	void resetNextIdentifier();
	//Description: Sets the next identifier back to 'A'

	//Getters
	int getX();
	double getXCount();
	int getY();
	double getYCount();
	int getEnergy();
	double getDirection();
	double getSpeed();
	char getIdentifier();
	bool getPowerDown();

	//Setters
	void setSolidWalls(bool solid);
	void setX(int xIn);
	void setXCount(double XCountIn);
	void setY(int yIn);
	void setYCount(double yCountIn);
	void setEnergy(int energyIn);
	void setDirection(int directionIn);
	void setSpeed(int speedIn);
	void setIdentifier(char identIn);
};