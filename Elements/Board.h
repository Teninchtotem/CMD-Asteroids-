#pragma once
#include "Flotsam.h"
#include "Jetsam.h"
#include "Ship.h"
#include "Bullet.h"
#include "Firework.h"
#include "Mine.h"
#include "Bomb.h"
#include "Console.h"
#include "RandomNumber.h"
#include "Element.h"
#include "vector"
#include <list>
#include <iostream>
#include <iomanip>
using namespace std;

// code shell, amend as appropriate

class Board
{
private:
	bool gameReset;
	bool survivalMode;
	bool newLevelTime;
	int newLevelTimer;
	bool solidSaySo;
	bool solidWalls;
	bool discoWasTrue;
	bool discoMode;
	int mineDelay;
	int turns;
	int bombId;
	bool gameOver;
	int reloadSegments;
	int reloadTime;
	int bulletId;
	double reloading;
	bool gameOverSoundPlayed;
	bool mute;
	int funWin;
	int funWinStore;
	int lives;
	bool pause;
	bool notQuit;
	int pauseLocation;
	bool firstErrorCatch;
	char firstErrorPlace;
	int points;
	int flotsamsAndJetsams;
	int boxSize;
	int anouncerMark;
	int healthMark;
	char **collissionDetection;
	char colissionObject1;
	char colissionObject2;
	list<Element*> objects;
	list<Element*>::iterator obj;
	Console console;
	RandomNumber rnd;

public: 
	Board(int boxSizeIn);
	 //a constructor that populates the Element container with an appropriate amount 
	//of flotsam and jetsam (max 13 of each) as well as a ship and initializing variables.
	
	~Board();
	//Sets everything to 0

	void setRandomColour();
	//Sets the console to a random colour

	void drawHUD();
	//Outputs everything like "HP: " to the screen

	int calculateBoardEnergy();
	//Description: Calculates how many points the player can possibly earn
	//Postcondition: Returns total energy of all flotsam and jetsam (total points player can possibly earn)

	void testPowerDown(char colissionObject, char otherColissionObject, int colissionObjectEnergy);
	//Precondition: The Identifier of the Element in question and it's energy as well as the object it collided with's Identifier.
	//Description: Tests to see if the Element in question is powered down, and outputs appropriatly.

	void selectPlayer();
	//Description: Sets the itorator to the player
	
	void EqualizingElements();
	//Description: Increeses or decreeses all of the elements equally untill the board has the correct net worth (Total points avaliable)

	void outputInstructions();
	//Description: Outputs the instructions and waits for the player to be ready

	void checkForPlayerInput();
	//Description: Checkts to see if the player is pressing any keys that do stuff. Then does it.
	//Also Outputs player related stats such as HP and Lives

	void boardReset();
	//Description: Resets the board to a new game

	void drawBox(int width, int height);
	//Description: Draws a box to fit the "game area"

	void populateList();
	//Creates a list filled with Flotsam and Jetsam as well as a Space Ship

	void clearList();
	//Clears the already created list

	void createExplosion();
	//Description: Created an explosion at the current loaction of the itorators object

	void display();		//a method that displays and updates all objects
	//by calling each object’s printAt(), hideAt() and moveElement() method. It also takes into account any colissions that happen	
};