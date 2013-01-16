#include <cstdlib>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <GL/glut.h>
#include <Windows.h>

#include "Game.h"

using namespace std;

Game::Game()
{
	isGameReady = false;
	isPlayerWinner = false;
	isAIWinner = false;
	keyHasBeenChosen = false;
	descriptionString = "You are about to enter the arena. What will be your strategy?";
	choice0 = "(1) Aim for the middle.";
	choice1 = "(2) Aim low.";
	choice2 = "(3) Aim high.";
	choiceRandom = "(4) I will wing it!";
	returnToMenu = "(Esc) Main Menu";

	//For generating random numbers
	srand((unsigned)time(0)); 
}

Game::~Game()
{
	delete[] descriptionString;
	delete[] choice0;
	delete[] choice1;
	delete[] choice2;
	delete[] choiceRandom;
	delete[] returnToMenu;
}

bool Game::getIsGameReady() { return isGameReady; }
bool Game::getIsPlayerWinner(){ return isPlayerWinner; }
bool Game::getIsAIWinner() { return isAIWinner; }

void Game::display()
{	
	renderBitmapString(-15, 10, GLUT_BITMAP_TIMES_ROMAN_24, descriptionString);
	renderBitmapString(-15, 6, GLUT_BITMAP_TIMES_ROMAN_24, choice0);
	renderBitmapString(-15, 4, GLUT_BITMAP_TIMES_ROMAN_24, choice1);
	renderBitmapString(-15, 2, GLUT_BITMAP_TIMES_ROMAN_24, choice2);
	renderBitmapString(-15, 0, GLUT_BITMAP_TIMES_ROMAN_24, choiceRandom);
	renderBitmapString(-15, -2, GLUT_BITMAP_TIMES_ROMAN_24, returnToMenu);
}

void Game::handleKeyPress(unsigned char key, int x, int y)
{
	if(!keyHasBeenChosen)
	{
		switch (key)
		{
			case '1':
				initialiseGameParameters(0);
				PlaySound(TEXT("Sounds/fanfare3.WAV"), NULL, SND_FILENAME);
				keyHasBeenChosen = true;
				break;
			case '2':
				initialiseGameParameters(1);
				PlaySound(TEXT("Sounds/fanfare3.WAV"), NULL, SND_FILENAME);
				keyHasBeenChosen = true;
				break;
			case '3':
				initialiseGameParameters(2);
				PlaySound(TEXT("Sounds/fanfare3.WAV"), NULL, SND_FILENAME);
				keyHasBeenChosen = true;
				break;
			case '4':
				initialiseGameParameters((rand() % 3));
				PlaySound(TEXT("Sounds/fanfare3.WAV"), NULL, SND_FILENAME);
				keyHasBeenChosen = true;
				break;
		
			default:
				break;
		}
	}
}

void Game::initialiseGameParameters(int playerDecision)
{	
	int aiDecision = rand() % 3;

	if( (playerDecision == 2 && aiDecision == 0) ||
		(playerDecision == 0 && aiDecision == 1) ||
		(playerDecision == 1 && aiDecision == 2) )
	{
		isPlayerWinner = false;
		isAIWinner = true;
	}
	else if( (playerDecision == 0 && aiDecision == 2) ||
			 (playerDecision == 1 && aiDecision == 0) ||
			 (playerDecision == 2 && aiDecision == 1) )
	{
		isPlayerWinner = true;
		isAIWinner = false;
	}
	else //playerDecision == aiDecision
	{
		isPlayerWinner = true;
		isAIWinner = false;
	}
	isGameReady = true;
}

void Game::reset()
{
	isGameReady = false;
	isPlayerWinner = false;
	isAIWinner = false;
	keyHasBeenChosen = false;
}

void Game::renderBitmapString(float x, float y, void *font, char *string)
{
	char* p;

	glRasterPos2f(x, y);

	for (p = string; *p; p++)
	{				
		glutBitmapCharacter(font, *p);
	}
}

