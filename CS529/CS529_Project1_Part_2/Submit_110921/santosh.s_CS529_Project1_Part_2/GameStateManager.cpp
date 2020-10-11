/* Start Header -------------------------------------------------------
Copyright (C) 2011 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name		: GameStateManager.cpp
Purpose			: Game state manager
Language		: C++/MS C++ 2010 Express
Platform		: Microsoft Visual Studio 2010/i7 2630QM, 8GB DDR3 1600MHz, 3GB NVIDIA GT 555M/Windows 7 Home Premium
Project			: CS529_SantoshShedbalkar_Project1_2
Author			: Santosh Shedbalkar/santosh.s/60001111
Creation date	: 09/19/2011
- End Header --------------------------------------------------------*/

#include "GameStateManager.h"
#include "Global.h"
#include "GameStateList.h"

// ---------------------------------------------------------------------------
// Variables that guide the flow of the game

int next, current, prev;

// ---------------------------------------------------------------------------
// Function pointers to various game state handlers provided by corresponding levels

int (*load) (void);
int (*initialize) (void);
int (*update) (void);
int (*draw) (void);
int (*free) (void);
int (*unload) (void);

// ---------------------------------------------------------------------------
// Initializes the GSM to a valid level id from where the execution begins

void GSM_Initialize(int levelID)
{
	output_To_File("GSM: Initialize\n");
	next = current = prev = levelID;
}

// ---------------------------------------------------------------------------
// Updates the function pointers to corresponding functions provided by the current level

void GSM_Update(void)
{
	output_To_File("GSM: Update\n");
	//
	if(current > -1 && current != LEVEL_QUIT && current != LEVEL_RESTART)
	{

		// ---------------------------------------------------------------------------
		// fetch the level handlers' structure which corresponds to the current level

		struct LevelHandlers *lvl = &levels[current];
		
		// ---------------------------------------------------------------------------
		// update function pointers

		load = lvl->load;
		initialize = lvl->initialize;
		update = lvl->update;
		draw = lvl->draw;
		free = lvl->free;
		unload = lvl->unload;
	}
}

// ---------------------------------------------------------------------------
// various game state handlers which act as wrapper functions for function pointers

int levelLoad(void)
{
	printf("levelLoad\n");
	return load();
}

int levelInitialize(void)
{
	printf("levelInitialize\n");
	return initialize();
}

int levelUpdate(void)
{
	printf("levelUpdate\n");
	return update();
}

int levelDraw(void)
{
	printf("levelDraw\n");
	return draw();
}

int levelFree(void)
{
	printf("levelFree\n");
	return free();
}

int levelUnload(void)
{
	printf("levelUnload\n");
	return unload();
}

// ---------------------------------------------------------------------------
// getter setter functions for the game state variables

int getNext(void)
{
	return next;
}

int setNext(int val)
{
	return next = val;
}

int getCurrent(void)
{
	return current;
}

int setCurrent(int val)
{
	return current = val;
}

int getPrev(void)
{
	return prev;
}

int setPrev(int val)
{
	return prev = val;
}
