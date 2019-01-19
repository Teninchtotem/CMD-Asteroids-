#include "Board.h"

//Reduce flickering, maybe make things like "Lives" or "HP" permanent on the board

Board::Board(int boxSizeIn)
{
	this->boxSize = boxSizeIn;

	//Creating the 2d colission grid dynamicaly
	collissionDetection = new char*[this->boxSize];

	for( int i = 0 ; i < this->boxSize ; i++ )
		collissionDetection[i] = new char[this->boxSize];
	gameReset = false;
	survivalMode = false;
	newLevelTime = false;
	newLevelTimer = 0;
	solidSaySo = false;
	solidWalls = false;
	discoWasTrue = true;
	discoMode = false;
	turns = 0;
	mineDelay = 0;
	gameOver = false;
	reloadSegments = 0;
	reloadTime = 1000;
	reloading = clock();
	bulletId = 0;
	bombId = 0;
	mute = false;
	gameOverSoundPlayed = false;
	funWin = rnd.random(50,100);
	funWinStore = funWin;
	funWin = funWin + 100;
	lives = 2;
	firstErrorPlace = 0;
	pause = false;
	notQuit = true;
	pauseLocation = (boxSize/2) - 2;
	firstErrorCatch = 0;
	firstErrorPlace = 65;
	points = 0;
	anouncerMark = this->boxSize + 7;
	healthMark = this->boxSize + 3;

	colissionObject1 = 0;
	colissionObject2 = 0;

	for(int i = 0; i < this->boxSize; i++)
	{
		for(int j = 0; j < this->boxSize; j++)
		{
			collissionDetection[i][j] = 0;
		}
	}

	flotsamsAndJetsams = (this->boxSize / 5);

	if(flotsamsAndJetsams > 13)
		flotsamsAndJetsams = 13;


	outputInstructions();
}
Board::~Board()
{
	survivalMode = 0;
	newLevelTime = 0;
	newLevelTimer = 0;
	solidSaySo = 0;
	discoMode = 0;
	mineDelay = 0;
	turns = 0;
	bombId = 0;
	gameOver = 0;
	reloadSegments = 0;
	reloadTime = 0;
	bulletId = 0;
	reloading = 0;
	gameOverSoundPlayed = 0;
	mute = 0;
	funWin = 0;
	funWinStore = 0;
	lives = 0;
	pause = 0;
	notQuit = 0;
	pauseLocation = 0;
	firstErrorCatch = 0;
	firstErrorPlace = 0;
	points = 0;
	flotsamsAndJetsams = 0;
	anouncerMark = 0;
	healthMark = 0;
	colissionObject1 = 0;
	colissionObject2 = 0;
	for(int i = 0; i < this->boxSize; i++)
	{
		for(int j = 0; j < this->boxSize; j++)
		{
			collissionDetection[i][j] = 0;
		}
	}
	boxSize = 0;
}
void Board::drawBox(int width, int height)
{
	int randomColour = 0;
	for(int i = 0; i < width; i++)
	{
		if(!discoWasTrue)
			setRandomColour();
		console.gotoXY(i, 0);
		cout << "#";
		if(!discoWasTrue)
			setRandomColour();
		console.gotoXY(i, height);
		cout << "#";
	}
	for(int i = 0; i < height + 1; i++)
	{
		if(!discoWasTrue)
			setRandomColour();
		console.gotoXY(0, i);
		cout << "#";
		if(!discoWasTrue)
			setRandomColour();
		console.gotoXY(width, i);
		cout << "#";
	}
	if(discoWasTrue)
	{
		discoWasTrue = false;
		console.gotoXY(0,0);
	}
	console.setColour(console.DEFAULT, console.BLACK);
}
void Board::clearList()
{
	objects.front()->resetNextIdentifier();

	while(!objects.empty())
	{
		objects.front()->hideAt();
		collissionDetection[objects.front()->getX()][objects.front()->getY()] = 0;

		delete objects.front();
		objects.pop_front();
	}
}
void Board::populateList()
{
	//// use a loop to add elements to objects
	for(int i = 0; i < flotsamsAndJetsams; i++)
	{
		objects.push_back(new Flotsam(rnd, console, this->boxSize));
		objects.back()->setSolidWalls(solidWalls);
	}
	for(int i = 0; i < flotsamsAndJetsams; i++)
	{
		objects.push_back(new Jetsam(rnd, console, this->boxSize));
		objects.back()->setSolidWalls(solidWalls);
	}
	// Adding the player
	objects.push_back(new Ship(rnd, console, this->boxSize));
	objects.back()->setSolidWalls(solidWalls);
}
void Board::boardReset()
{
	//Fix - Stopping the player from messing up the game during the restart
	GetAsyncKeyState(VK_SPACE);
	GetAsyncKeyState(VK_LMENU);
	
	if(solidWalls)
		solidSaySo = true;

	if(!newLevelTime)
	{
		points = 0;
		funWin = rnd.random(50,100);
		funWinStore = funWin;
		funWin = funWin + 100;
		lives = 2;
	}

	system("CLS");
	clearList();
	populateList();

	if(!discoMode && !discoWasTrue)
		discoWasTrue = true;

	drawHUD();
	newLevelTime = false;
	turns = 0;
	mineDelay = 0;
	anouncerMark = this->boxSize + 7;
	gameOver = false;
	gameOverSoundPlayed = false;
	EqualizingElements();
	obj = objects.begin();
}
void Board::selectPlayer()
{
	obj = objects.end();
	obj--;
	while((*obj)->getIdentifier() != '|')
		obj--;
}
void Board::checkForPlayerInput()
{	
	bool pauseWasTrue = false;

	//Muting/Unmuting the game
	if(GetAsyncKeyState(VK_SHIFT))
	{
		console.gotoXY(0,healthMark - 1);
		mute = !mute;
		if(mute)
			cout << "M";
		else
			cout << " ";
	}
	//Making the walls solid
	if(GetAsyncKeyState(VK_INSERT) || solidSaySo)
	{
		console.gotoXY(2,healthMark - 1);
		if(!solidSaySo)
			solidWalls = !solidWalls;
		if(solidWalls)
		{
			cout << "S";
		}
		else
		{
			cout << " ";
		}
		obj = objects.begin();

		if(!discoMode && !solidWalls)
			discoWasTrue = true;

		while(obj != objects.end())
		{
			(*obj)->setSolidWalls(solidWalls);
			obj++;
		}
		drawBox(boxSize + 1, boxSize + 1);

		solidSaySo = false;
	}

	//Displaying Menu
	if(GetAsyncKeyState(VK_DELETE))
	{
		bool gameTypeChange = survivalMode;
		solidSaySo = true;
		GetAsyncKeyState(VK_RMENU);
		system("CLS");
		outputInstructions();
		system("CLS");
		if(gameReset)
		{
			boardReset();
			gameReset = false;
		}
		if(!discoMode && !discoWasTrue)
			discoWasTrue = true;
		drawHUD();
		pauseWasTrue = true;
	}

	//Quiting the game
	if(GetAsyncKeyState(VK_ESCAPE))		
		notQuit = false;

	//Pausing when player presses enter
	if(GetAsyncKeyState(VK_RETURN))
	{
		pauseWasTrue = true;
		pause = true;
		console.gotoXY(pauseLocation, (boxSize/2) + 1);
		cout << "PAUSED";
		console.gotoXY(0,0);
	}
	while(pause)
	{
		Sleep(500);
		if(GetAsyncKeyState(VK_RETURN))
			pause = false;
		else if(GetAsyncKeyState(VK_ESCAPE))
		{
			pause = false;
			notQuit = false;
		}
		else if(GetAsyncKeyState(VK_BACK))
		{
			pause = false;
			boardReset();
		}
	}

	//Restarting the game when the player presses backspace
	if(GetAsyncKeyState(VK_BACK))
	{
		if(mute == false)
			PlaySound(L"Go.wav",NULL,SND_ASYNC|SND_FILENAME);
		boardReset();
	}
	//Player Controls
	selectPlayer();
	if(!(*obj)->getPowerDown())
	{
		//Grabbing keys and changing the Ships stats
		if(GetAsyncKeyState(VK_LEFT))
			(*obj)->setXCount((*obj)->getXCount() - 0.1);
		if(GetAsyncKeyState(VK_RIGHT))
			(*obj)->setXCount((*obj)->getXCount() + 0.1);
		if(GetAsyncKeyState(VK_DOWN))
			(*obj)->setYCount((*obj)->getYCount() + 0.1);
		if(GetAsyncKeyState(VK_UP))
			(*obj)->setYCount((*obj)->getYCount() - 0.1);

		//Laying/Detonating a mine
		if(GetAsyncKeyState(VK_LMENU) && !gameOver)
		{
			if(objects.back()->getIdentifier() != 58)
			{
				//Causing a short delay so the player doest set a mine just as the last one is destroyed
				if(mineDelay - turns < -4)
				{
					if(mute == false)
						PlaySound(L"LayMine.wav", NULL, SND_ASYNC|SND_FILENAME);

					int mineX = (*obj)->getX();
					int mineY = (*obj)->getY();

					if((*obj)->getXCount() > 0)
						mineX--;
					else if((*obj)->getXCount() < 0)
						mineX++;

					if((*obj)->getYCount() > 0)
						mineY--;
					else if((*obj)->getYCount() < 0)
						mineY++;

					if((*obj)->getYCount() == 0 && (*obj)->getXCount() == 0)
						mineX--;

					objects.push_back(new Mine(rnd, console, this->boxSize));
					objects.back()->setX(mineX);
					objects.back()->setY(mineY);
					objects.back()->moveElement();
				}
			}
			//Detonating Mine
			else
			{
				mineDelay = turns;

				obj = objects.end();
				obj--;

				if((*obj)->getEnergy() <= 10000)
				{
					createExplosion();
					if(mute == false)
						PlaySound(L"PowerDown.wav", NULL, SND_ASYNC|SND_FILENAME);

					delete *obj;
					objects.erase(obj--);
				}
			}
			selectPlayer();
		}


		//Creating a bullet and Calculating the reload bar (Can't fire when reloading)
		if(clock() - reloading > reloadTime)
		{
			//Player cant shoot when the game has finished
			if(GetAsyncKeyState(VK_SPACE) && !gameOver)
			{			
				reloading = clock();
				objects.push_front(new Bullet(rnd, console, this->boxSize));
				objects.front()->setIdentifier(35 + bulletId);
				objects.front()->setX((*obj)->getX());
				objects.front()->setY((*obj)->getY());
				objects.front()->setSolidWalls(solidWalls);
				double bulletSpeed = 2;
				objects.front()->calculateTragectory((*obj)->getDirection(), sqrt(bulletSpeed));
				objects.front()->moveElement();
				if(mute == false)
					PlaySound(L"Lazer.wav", NULL, SND_ASYNC|SND_FILENAME);
				reloadSegments = 0;
				console.gotoXY(boxSize - 9,healthMark - 1);
				cout << "          ";
				if(bulletId > 20)
					bulletId = 0;
				else
					bulletId++;
			}
			else
			{
				reloadSegments = 10;
			}
		}
		else
		{
			int cutHolder = (clock() - reloading)/(reloadTime/10);
			reloadSegments = cutHolder;

			//Fix - to stop player fireing twice if they press enter while reloading
			GetAsyncKeyState(VK_SPACE);
		}

		//Displaying the Reload Bar
		console.gotoXY(boxSize - 9,healthMark - 1);

		if(reloadSegments < 10)
			console.setColour(Console::RED, Console::BLACK);
		else
			console.setColour(Console::GREEN, Console::BLACK);

		for(int i = 0; i < reloadSegments; i++)
			cout << '|';
	}
	//Player has died, outputs as appropriate (with system("pause") to simulate respawn time)
	else
	{
		console.gotoXY(4,healthMark + 1);
		console.setColour(Console::RED, Console::BLACK);
		cout << "DEAD";
		console.setColour(Console::DEFAULT, Console::BLACK);

		if(lives > 0)
		{
			lives--;
			(*obj)->hideAt();
			(*obj)->setEnergy(20);
			(*obj)->setXCount(0);
			(*obj)->setYCount(0);
			(*obj)->setDirection(0);
			(*obj)->setSpeed(0);
			(*obj)->setX(boxSize/2);
			(*obj)->setY(boxSize/2);
			(*obj)->powerUp();
			console.gotoXY(pauseLocation - 2, (boxSize/2) + 1);
			cout << "Respawning";
			console.gotoXY(0,0);
			pauseWasTrue = true;

			if(mute == false)
				PlaySound(L"GetReady.wav", NULL, SND_ASYNC|SND_FILENAME);

			bool tempPause = true;
			int tempTimer = clock();
			while(clock() - tempTimer < 3000 && tempPause)
			{
				if(GetAsyncKeyState(VK_ESCAPE))
				{
					tempPause = false;
					notQuit = false;
				}
				else if(GetAsyncKeyState(VK_BACK))
				{
					tempPause = false;
					boardReset();
				}
				Sleep(100);
			}
			//FIX - Stops the player shooting or laying a mine if they pressed the keys when respawning
			GetAsyncKeyState(VK_SPACE);
			GetAsyncKeyState(VK_LMENU);
		}
		//Player is out of lives so output game over stuff
		else
		{
			if(gameOverSoundPlayed == false)
			{
				gameOverSoundPlayed = true;
				if(mute == false)
					PlaySound(L"GameOver.wav", NULL, SND_ASYNC|SND_FILENAME);
			}
			console.gotoXY(pauseLocation - 2, boxSize/2);
			console.setColour(Console::RED, Console::BLACK);
			cout << "GAME OVER";
			console.gotoXY(pauseLocation - 2, (boxSize/2) + 2);
			cout << "Score: ";
			console.setColour(Console::YELLOW, Console::BLACK);
			cout << points;
			console.setColour(Console::DEFAULT, Console::BLACK);
		}
		if(lives >= 0)
		{
			//Deleting all mines, bullets and explosions (Makes the game a tad less frustrating)
			obj = objects.begin();
			while(obj != objects.end())
			{
				if((*obj)->getType() > 3)
				{
					(*obj)->hideAt();
					collissionDetection[(*obj)->getX()][(*obj)->getY()] = 0;

					delete *obj;
					objects.erase(obj++);
				}
				else
					obj++;
			}
			selectPlayer();
		}
	}
	//Output Health and Points
	console.setColour(Console::DEFAULT, Console::BLACK);
	console.gotoXY(7,healthMark);
	cout << "             ";
	console.gotoXY(4,healthMark + 1);
	cout << "          ";
	console.gotoXY(18,healthMark + 3);
	cout << "        ";
	console.gotoXY(7,healthMark);
	console.setColour(Console::GREEN, Console::BLACK);
	for(int i = 0; i < lives; i++)
		cout << "<3 ";
	console.gotoXY(4,healthMark + 1);
	if((*obj)->getEnergy() < 10)
		console.setColour(Console::RED, Console::BLACK);
	else
		console.setColour(Console::LIGHT_BLUE, Console::BLACK);
	if((*obj)->getEnergy() <= 0)
		cout << "DEAD";
	else
		cout << (*obj)->getEnergy();
	console.gotoXY(8,healthMark + 2);
	console.setColour(Console::YELLOW, Console::BLACK);
	cout << points; 
	console.gotoXY(18,healthMark + 3);
	if(!survivalMode)
		cout << calculateBoardEnergy();
	console.setColour(Console::DEFAULT, Console::BLACK);

	//If the game paused, this will clear up anything written in the middle of the screen
	if(pauseWasTrue)
	{
		if(mute == false)
			PlaySound(L"Go.wav", NULL, SND_ASYNC|SND_FILENAME);

		reloading = clock();
		console.gotoXY(boxSize - 9,healthMark - 1);
		cout << "          ";

		pauseWasTrue = false;
		console.gotoXY(pauseLocation - 2, (boxSize/2) + 1);
		cout << "          ";
	}
}
void Board::outputInstructions()
{
	bool solidStore = solidWalls;
	solidWalls = false;

	if(mute == false)
	{
		if(discoMode)
			PlaySound(L"Funk.wav", NULL, SND_LOOP|SND_ASYNC|SND_FILENAME);
		else
			PlaySound(L"IntroGame.wav", NULL, SND_LOOP|SND_ASYNC|SND_FILENAME);
	}

	//Outputting instructions for the player
	cout << "\n\n\n" << endl;
	cout << setw(43) << "ASTEROIDS" << endl;
	cout << setw(46) << "/^^^^^^^^^^^^^^^" << char(92) << endl;
	cout << "\n\n\n\tThis is a very basic version of asteroids, the aim is to fly around \n\tAnd avoid being hit while shooting as many asteroids as possible" << endl;
	cout << "\n\tAlso, Play some music in the background to make it more fun\n\tIm not good enough to play 2 sounds at once, but windows is! :D" << endl;
	cout << endl << endl << endl;
	cout << "\t\t\t\t   CONTROLS" << endl << endl;
	cout << setw(30) << "Up Arrow: " << "  |   Increese upward velocity" << endl;
	cout << setw(30) << "Down Arrow: " << "  |   Increese Downward velocity" << endl;
	cout << setw(30) << "Left Arrow: " << "  |   Increese velocity to the left" << endl;
	cout << setw(30) << "Right Arrow: " << "  |   Increese velocity to the right" << endl;
	cout << setw(33) << "|" << endl;
	cout << setw(30) << "Space Bar: " << "  |   Shoots a photon torpedo" << endl;
	cout << setw(33) << "|" << endl;
	cout << setw(30) << "Left CTRL: " << "  |   (hold) Speeds up game" << endl;
	cout << setw(33) << "|" << endl;
	cout << setw(30) << "Right CTRL: " << "  |   (hold) Slows down game" << endl;
	cout << setw(33) << "|" << endl;
	cout << setw(30) << "Left ALT: " << "  |   Creates/Detonates a Mine" << endl;
	cout << setw(33) << "|" << endl;
	cout << setw(30) << "ESC: " << "  |   Quits the game" << endl;
	cout << setw(33) << "|" << endl;
	cout << setw(30) << "BackSpace: " << "  |   Restarts the game" << endl;
	cout << setw(33) << "|" << endl;
	cout << setw(30) << "Shift: " << "  |   Mutes/Un-Mutes the game" << endl;
	cout << setw(33) << "|" << endl;
	cout << setw(30) << "Insert: " << "  |   Makes the edges of the map solid" << endl;
	cout << setw(33) << "|" << endl;
	cout << setw(30) << "Enter: " << "  |   Pauses/Un-Pauses the game" << endl;
	cout << setw(33) << "|" << endl;
	cout << setw(30) << "Delete: " << "  |   Opens this page during gameplay" << endl << endl << endl << endl;
	cout << "\t\t    And for the love of god FULL SCREEN IT" << endl << endl;
	cout << "\t\t\t  Press Enter to continue..." << endl;
	cout << "\t\t (Creates default game if not already playing)" << endl << endl;
	cout << "\t\t   Press Backspace to start a normal game..." << endl;
	cout << "\t\t    Press Space to start a survivor game..." << endl;

	console.gotoXY(0, 0);

	if(!discoWasTrue && !discoMode)
		discoWasTrue = true;

	bool exitMenu = false;

	while(!exitMenu)
	{
		if(discoMode || discoWasTrue)
		{
			if(!discoWasTrue)
			{
				for(int i = 0; i < 2; i++)
				{
					int tempX = 0;
					int tempY = 0;

					switch(i)
					{
					case 0: tempX = 13; tempY = 1; break;
					case 1: tempX = 60; tempY = 1; break;
					}
					console.gotoXY(tempX, tempY++);
					cout << "  |";

					setRandomColour();
					console.gotoXY(tempX, tempY);
					cout << char(92);
					setRandomColour();
					console.gotoXY(tempX + 4, tempY);
					cout << "/";
					console.setColour(console.DEFAULT, console.BLACK);

					console.gotoXY(tempX + 1, tempY++);
					cout << "_|_";

					setRandomColour();
					console.gotoXY(tempX - 1, tempY);
					cout << char(92);
					setRandomColour();
					console.gotoXY(tempX + 5, tempY);
					cout << "/";
					console.setColour(console.DEFAULT, console.BLACK);

					console.gotoXY(tempX, tempY++);
					cout << "/   " << char(92);

					setRandomColour();
					console.gotoXY(tempX - 2, tempY);
					cout << "-";
					setRandomColour();
					console.gotoXY(tempX + 6, tempY);
					cout << "-";
					console.setColour(console.DEFAULT, console.BLACK);

					console.gotoXY(tempX - 1, tempY++);
					cout << "|     |";

					setRandomColour();
					console.gotoXY(tempX - 1, tempY);
					cout << "/";
					setRandomColour();
					console.gotoXY(tempX + 5, tempY);
					cout << char(92);
					console.setColour(console.DEFAULT, console.BLACK);

					console.gotoXY(tempX, tempY++);
					cout << char(92) << "___/";

					setRandomColour();
					console.gotoXY(tempX, tempY);
					cout << "/";
					setRandomColour();
					console.gotoXY(tempX + 4, tempY);
					cout << char(92);
					setRandomColour();
					console.gotoXY(tempX + 2, tempY);
					cout << "|";
					console.setColour(console.DEFAULT, console.BLACK);
				}
			}
			else
			{
				for(int i = 0; i < 2; i++)
				{
					int tempX = 0;
					int tempY = 0;

					switch(i)
					{
					case 0: tempX = 12; tempY = 1; break;
					case 1: tempX = 63; tempY = 1; break;
					}
					console.gotoXY(tempX-5, tempY++);
					cout << "             ";
					console.gotoXY(tempX-5, tempY++);
					cout << "             ";
					console.gotoXY(tempX-5, tempY++);
					cout << "             ";
					console.gotoXY(tempX-5, tempY++);
					cout << "             ";
					console.gotoXY(tempX-5, tempY++);
					cout << "             ";
					console.gotoXY(tempX-5, tempY);
					cout << "             ";
				}
			}
			drawBox(79,56);

		}

		if(GetAsyncKeyState(VK_SHIFT))
		{
			if(mute == false)
				PlaySound(L"CancelSound.wav",NULL,SND_ASYNC|SND_FILENAME);
			else if(discoMode)
				PlaySound(L"Funk.wav", NULL, SND_LOOP|SND_ASYNC|SND_FILENAME);
			else
				PlaySound(L"IntroGame.wav", NULL, SND_LOOP|SND_ASYNC|SND_FILENAME);

			mute = !mute;
			Sleep(500);
		}
		if(GetAsyncKeyState(VK_SPACE))
		{
			exitMenu = true;
			survivalMode = true;
			gameReset = true;
		}
		if(GetAsyncKeyState(VK_BACK))
		{
			exitMenu = true;
			survivalMode = false;
			gameReset = true;
		}
		if(GetAsyncKeyState(VK_RMENU))
		{
			discoMode = !discoMode;
			discoWasTrue = true;

			if(mute == false)
			{
				if(discoMode == false)
					PlaySound(L"IntroGame.wav",NULL,SND_LOOP|SND_ASYNC|SND_FILENAME);
				else
					PlaySound(L"Funk.wav", NULL, SND_LOOP|SND_ASYNC|SND_FILENAME);
			}
			Sleep(500);
		}
		if(GetAsyncKeyState(VK_ESCAPE))
		{
			exitMenu = true;
			notQuit = false;
		}
		if(GetAsyncKeyState(VK_RETURN))
		{
			exitMenu = true;
		}
	}
	if(notQuit)
	{
		Sleep(500);
		system("CLS");

		//Fix - Stopping the player from messing up the game during the instructions
		GetAsyncKeyState(VK_RETURN);
		GetAsyncKeyState(VK_BACK);
		GetAsyncKeyState(VK_SPACE);
		GetAsyncKeyState(VK_LMENU);
		GetAsyncKeyState(VK_ESCAPE);
		GetAsyncKeyState(VK_DELETE);
		GetAsyncKeyState(VK_UP);
		GetAsyncKeyState(VK_DOWN);
		GetAsyncKeyState(VK_LEFT);
		GetAsyncKeyState(VK_RIGHT);
		GetAsyncKeyState(VK_INSERT);
		GetAsyncKeyState(VK_RMENU);
	}
	solidWalls = solidStore;
}
void Board::setRandomColour()
{
	int randomColour = rnd.random(1,12);

	if(solidWalls && !discoMode)
		console.setColour(console.BLACK, console.WHITE);
	else if(!solidWalls)
	{
		switch(randomColour)
		{
		case 1: console.setColour(console.LIGHT_PURPLE,console.BLACK);	break;
		case 2: console.setColour(console.AQUA,console.BLACK);			break;
		case 3: console.setColour(console.BLUE,console.BLACK);			break;
		case 4: console.setColour(console.GREEN,console.BLACK);			break;
		case 5: console.setColour(console.PURPLE,console.BLACK);		break;
		case 6: console.setColour(console.YELLOW,console.BLACK);		break;
		case 7: console.setColour(console.LIGHT_GREEN,console.BLACK);	break;
		case 8: console.setColour(console.LIGHT_RED,console.BLACK);		break;
		case 9: console.setColour(console.LIGHT_YELLOW,console.BLACK);	break;
		case 10: console.setColour(console.LIGHT_BLUE,console.BLACK);	break;
		case 11: console.setColour(console.LIGHT_AQUA,console.BLACK);	break;
		case 12: console.setColour(console.RED,console.BLACK);			break;
		}
	}
	else 
	{
		switch(randomColour)
		{
		case 1: console.setColour(console.LIGHT_PURPLE,console.WHITE);	break;
		case 2: console.setColour(console.AQUA,console.WHITE);			break;
		case 3: console.setColour(console.BLUE,console.WHITE);			break;
		case 4: console.setColour(console.GREEN,console.WHITE);			break;
		case 5: console.setColour(console.PURPLE,console.WHITE);		break;
		case 6: console.setColour(console.YELLOW,console.WHITE);		break;
		case 7: console.setColour(console.LIGHT_GREEN,console.WHITE);	break;
		case 8: console.setColour(console.LIGHT_RED,console.WHITE);		break;
		case 9: console.setColour(console.LIGHT_YELLOW,console.WHITE);	break;
		case 10: console.setColour(console.LIGHT_BLUE,console.WHITE);	break;
		case 11: console.setColour(console.LIGHT_AQUA,console.WHITE);	break;
		case 12: console.setColour(console.RED,console.WHITE);			break;
		}
	}
}
void Board::drawHUD()
{
	console.gotoXY(boxSize - 21,healthMark - 1);
	cout << "Reload Bar <          >";


	console.gotoXY(0,healthMark);
	cout << "Lives: " << endl;
	cout << "HP: " << endl;
	cout << "Points: " << endl;
	if(!survivalMode)
		cout << "Points Remaining: ";
	else
		cout << "					";

	if(mute)
	{
		console.gotoXY(0,healthMark - 1);
		if(mute)
			cout << "M";
	}
}
void Board::display()
{
	if(notQuit)
	{
		gameReset = false;
		populateList();
		drawHUD();
		EqualizingElements();

		if(!discoMode && !discoWasTrue)
			discoWasTrue = true;

		if(mute == false)
			PlaySound(L"Go.wav", NULL, SND_ASYNC|SND_FILENAME);
	}
	//Loops untill the player quits
	while(notQuit)
	{
		if(discoMode || discoWasTrue)
			drawBox(boxSize+1, boxSize+1);

		checkForPlayerInput();

		//Set itorator to the first element
		obj = objects.begin();
		firstErrorPlace = ((*obj)->getIdentifier());

		//Annouce Game Over (if game over is true)
		if(gameOver)
		{
			if(gameOverSoundPlayed == false)
			{
				funWin += lives*25;
				funWinStore += lives*25;
				gameOverSoundPlayed = true;
				if(mute == false)
					PlaySound(L"Win.wav", NULL, SND_ASYNC|SND_FILENAME);
				while(obj != objects.end())
				{
					if((*obj)->getType() > 3)
					{
						(*obj)->hideAt();
						collissionDetection[(*obj)->getX()][(*obj)->getY()] = 0;

						delete *obj;
						objects.erase(obj++);
					}
					else
						obj++;
				}
			}
			obj = objects.begin();
			console.gotoXY(pauseLocation - 1, boxSize/2);
			console.setColour(Console::BLUE, Console::BLACK);
			cout << "YOU WIN";
			console.gotoXY(pauseLocation - 2, (boxSize/2) + 2);
			cout << "Score: ";
			console.setColour(Console::YELLOW, Console::BLACK);
			cout << points;

			if(objects.front()->getIdentifier() != 57)
				objects.push_front(new Firework(rnd, console, this->boxSize));

			console.setColour(Console::DEFAULT, Console::BLACK);
			if(funWin >= 0)
			{
				console.setColour(Console::BLUE, Console::BLACK);
				console.gotoXY(pauseLocation - 2, (boxSize/2) + 3);
				cout << "Bonus:     ";
				console.gotoXY(pauseLocation + 5, (boxSize/2) + 3);

				console.setColour(Console::YELLOW, Console::BLACK);
				if(funWin > funWinStore)
					cout << funWinStore;
				else
					cout << funWin;

				if(funWin <= funWinStore)
				{
					points = points + 2;
					newLevelTime = true;
					newLevelTimer = clock();
				}

				funWin = funWin - 2;

				console.setColour(Console::DEFAULT, Console::BLACK);

			}
			else
			{
				console.gotoXY(pauseLocation - 2, (boxSize/2) + 3);
				cout << "           ";
			}
		}

		//trueGameOver is acurate after a whole turn where as gameOver is not
		bool trueGameOver = gameOver;

		//Default is game over
		gameOver = true;

		//Loop for all Elements
		while(obj!=objects.end())
		{
			//Game Over cancelled if and Flotsam/Jetsam are detected
			if(((*obj)->getIdentifier() > 64)&&((*obj)->getIdentifier() < 91))
				gameOver = false;	

			//Resetting current location and Hiding Element
			(*obj)->hideAt();
			collissionDetection[(*obj)->getX()][(*obj)->getY()] = 0;

			//If the element is still alive
			if(!(*obj)->getPowerDown())
			{
				//Move it
				(*obj)->moveElement();

				//If the new space is clear
				if(collissionDetection[(*obj)->getX()][(*obj)->getY()] == 0)
				{
					//Set your identifier there and re-appear
					collissionDetection[(*obj)->getX()][(*obj)->getY()] = (*obj)->getIdentifier();
					(*obj)->printAt();

					if((*obj)->getType() == 1 || (*obj)->getType() == 2)
					{
						if(discoMode)
						{
							bool tempStore = solidWalls;
							solidWalls = false;
							setRandomColour();
							solidWalls = tempStore;
							console.gotoXY((*obj)->getX() + 1, (*obj)->getY() + 1);
							if((*obj)->getType() == 1)
								cout << char(1);
							else if((*obj)->getType() == 2)
								cout << char(2);
							console.setColour(console.DEFAULT, console.BLACK);
						}
					}
				}
				else //Colission
				{
					if(mute == false)
					{
						int randomHitTest = 0;

						randomHitTest = rnd.random(1,2);
						switch(randomHitTest)
						{
						case 1: PlaySound(L"Hit1.wav", NULL, SND_ASYNC|SND_NOSTOP|SND_FILENAME);	break;
						case 2: PlaySound(L"Hit2.wav", NULL, SND_ASYNC|SND_NOSTOP|SND_FILENAME);	break;
						}
					}
					//Setting the 2 colission objects for later referencing
					colissionObject1 = collissionDetection[(*obj)->getX()][(*obj)->getY()];
					colissionObject2 = (*obj)->getIdentifier();

					//Moving the object that has collided back a space and re-printing it
					(*obj)->moveElementBack();
					(*obj)->printAt();
					if((*obj)->getType() == 1 || (*obj)->getType() == 2)
					{
						if(discoMode)
						{
							bool tempStore = solidWalls;
							solidWalls = false;
							setRandomColour();
							solidWalls = tempStore;
							console.gotoXY((*obj)->getX() + 1, (*obj)->getY() + 1);
							if((*obj)->getType() == 1)
								cout << char(1);
							else if((*obj)->getType() == 2)
								cout << char(2);
						}
					}

					//CAusing an explosion if both objects are bullets
					if(colissionObject1 < 60 && colissionObject2 < 60)
						createExplosion();

					//Taking down information of the current object for later use
					double colissionObject2Direction = (*obj)->getDirection();
					double colissionObject2Speed = (*obj)->getSpeed();
					int colissionObject2Energy = (*obj)->getEnergy();

					console.gotoXY(0,anouncerMark);

					//Setting the list iterator to the other Element
					obj = objects.begin();
					while((*obj)->getIdentifier() != colissionObject1)
					{
						obj++;
					}

					//Taking down information of the other object for later use
					double colissionObject1Direction = (*obj)->getDirection();
					double colissionObject1Speed = (*obj)->getSpeed();
					int colissionObject1Energy = (*obj)->getEnergy();

					//Calculating new information for Colission Object 1 (Other Object)
					if(trueGameOver)
						(*obj)->calculateColission(colissionObject2Direction, colissionObject2Speed, colissionObject1Energy);
					else
						(*obj)->calculateColission(colissionObject2Direction, colissionObject2Speed, colissionObject2Energy);

					console.setColour(Console::DEFAULT, Console::BLACK);

					testPowerDown(colissionObject1, colissionObject2, colissionObject1Energy);

					//Setting itirator back to the first Element in the colission
					obj = objects.begin();
					while((*obj)->getIdentifier() != colissionObject2)
					{
						obj++;
					}

					//Calculating new information for Colission Object 2 (Current Object)
					if(trueGameOver)
						(*obj)->calculateColission(colissionObject1Direction, colissionObject1Speed, colissionObject2Energy);
					else
						(*obj)->calculateColission(colissionObject1Direction, colissionObject1Speed, colissionObject1Energy);

					testPowerDown(colissionObject2, colissionObject1, colissionObject2Energy);

					console.setColour(Console::BLACK, Console::BLACK);
				}
			}
			//Deleting Bullet after time out
			else
			{
				if(((*obj)->getIdentifier() < 60 || (*obj)->getIdentifier() > 95) && (*obj)->getIdentifier() != '|')
				{
					//Detonating Fireworks for the winning screen
					if((*obj)->getIdentifier() == 57 || (*obj)->getType() == 7)
					{
						createExplosion();
						if(mute == false)
							PlaySound(L"PowerDown.wav", NULL, SND_ASYNC|SND_NOSTOP|SND_FILENAME);
					}
					firstErrorCatch = 1;
					delete *obj;
					objects.erase(obj++);
					firstErrorPlace = objects.front()->getIdentifier();
				}
			}
			if(firstErrorCatch == 1)
				firstErrorCatch = 0;
			else
				obj++;
		}
		turns++;

		if(newLevelTime)
			if(clock() - newLevelTimer > 5000)
				boardReset();

		//Delaying refresh time (Varies based on any buttons held)
		if(GetAsyncKeyState(VK_LCONTROL) < 0){}
		else if(GetAsyncKeyState(VK_RCONTROL) < 0)
			Sleep(500);
		else 
			Sleep(100);

	}
	if(mute == false)
		PlaySound(L"Goodbye.wav", NULL, SND_FILENAME);
}
int Board::calculateBoardEnergy()
{
	int trueBoardScore = 0;

	obj = objects.begin();
	while(obj != objects.end())
	{
		if((*obj)->getIdentifier() > 64 && (*obj)->getIdentifier() < 95)
			trueBoardScore += (*obj)->getEnergy();
		obj++;
	}
	obj = objects.begin();

	return trueBoardScore;
}
void Board::EqualizingElements()
{
	obj = objects.end();
	obj--;	
	while((*obj)->getIdentifier() < 65 || (*obj)->getIdentifier() > 95)
	{
		obj--;
	}

	int netBoardScore = ((*obj)->getIdentifier() - 64) * 30;

	int difference = calculateBoardEnergy() - netBoardScore;
	int differenceChanger = 0;

	if(difference < 0)
		differenceChanger = 1;
	else
		differenceChanger = -1;

	while(difference != 0)
	{
		obj = objects.begin();

		while(obj != objects.end())
		{
			if((*obj)->getIdentifier() > 60 && (*obj)->getIdentifier() < 95)
			{
				if(difference != 0)
				{
					if(difference > 0)
					{
						if((*obj)->getEnergy() < 60)
						{
							(*obj)->setEnergy((*obj)->getEnergy() + differenceChanger);
							difference += differenceChanger;
						}
					}
					else if((*obj)->getEnergy() > 5)
					{
						(*obj)->setEnergy((*obj)->getEnergy() + differenceChanger);
						difference += differenceChanger;
					}
				}
			}
			obj++;
		}
	}
}
void Board::testPowerDown(char colissionObject, char otherColissionObject, int colissionObjectEnergy)
{
	//Testing for powerDown
	if((*obj)->getPowerDown())
	{
		//Aslong as it is no the player
		if(colissionObject != '|')
		{
			//If it's not a bullet, output that it has been destroyed
			if(colissionObject > 60 && colissionObject < 95)
			{
				console.setColour(Console::DEFAULT, Console::BLACK);
				console.gotoXY(0,anouncerMark);
				cout << "                                                   ";
				console.gotoXY(0,anouncerMark);
				cout << (*obj)->getIdentifier() << " has been destroyed" << endl;
				if(survivalMode)
				{
					char newIdent = (*obj)->getIdentifier();
					if((*obj)->getType() == 1)
						objects.push_front(new Flotsam(rnd, console, this->boxSize));
					else
						objects.push_front(new Jetsam(rnd, console, this->boxSize));

					objects.front()->setSolidWalls(solidWalls);
					objects.front()->setIdentifier(newIdent);
				}
				////Uncomment for scrolling updates (Also do the same a few lines below)
				/*anouncerMark++;*/
			}
			//Hide object and free up the space it was on
			(*obj)->hideAt();
			collissionDetection[(*obj)->getX()][(*obj)->getY()] = 0;


			//Play "blow up sound" and add objects points if the other object is a bullet/mine/explosion
			if((otherColissionObject < 60 || otherColissionObject > 90) && otherColissionObject != '|')
			{
				if(colissionObject > 60 && colissionObject < 95)
				{
					points += colissionObjectEnergy;
					createExplosion();
				}

				//Catch to stop 2 explosions that are next to eachother causing a rapid mess of overlapping explosions
				if(mute == false)
				{
					int tempTypeTest = otherColissionObject - colissionObject;
					if(tempTypeTest < 0)
						tempTypeTest = tempTypeTest * -1;

					if(tempTypeTest > 20 || otherColissionObject < 60)
					{
						PlaySound(L"PowerDown.wav", NULL, SND_ASYNC|SND_FILENAME);
					}
				}
			}
			delete *obj;

			//Test if this element is the first in the list (FirstErrorPlace allows for deleting of the first element without throwing errors)
			if(colissionObject == firstErrorPlace)
			{
				firstErrorCatch = 1;
				objects.erase(obj++);
				firstErrorPlace = objects.front()->getIdentifier();
			}
			else
			{
				objects.erase(obj--);
			}
		}
		else
		{
			console.setColour(Console::DEFAULT, Console::BLACK);
			console.gotoXY(0,anouncerMark);
			cout << "                                                   ";
			console.gotoXY(0,anouncerMark);
			cout << "Player has been destroyed" << endl;
			//Uncomment for scrolling updates (Also do the same a few lines up)
			/*anouncerMark++;*/
		}
	}
}
void Board::createExplosion()
{
	//Creates 8 "Bomb"s each going of in a different direction
	for(int i = 0; i < 8; i++)
	{
		int direction =  i * 45;

		objects.push_front(new Bomb(rnd, console, this->boxSize));
		objects.front()->setIdentifier(96 + bombId);
		objects.front()->setX((*obj)->getX());
		objects.front()->setY((*obj)->getY());
		objects.front()->calculateTragectory(direction, 1.5);
		objects.front()->setSolidWalls(solidWalls);

		if(bombId > 20)
			bombId = 0;
		else
			bombId++;
	}
}



