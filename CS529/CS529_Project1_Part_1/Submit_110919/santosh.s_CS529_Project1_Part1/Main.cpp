/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name		: Main.cpp
Purpose			: Application Main
Language		: C++/MS C++ 2010 Express
Platform		: Microsoft Visual C++ 2010/i7 2630QM, 8GB DDR3 1600MHz, 3GB NVIDIA GT 555M/Windows 7 Home Premium
Project			: CS529_SantoshShedbalkar_Project1_1
Author			: Santosh Shedbalkar/santosh.s/60001111
Creation date	: 09/19/2011
- End Header --------------------------------------------------------*/

#include "System.h"
#include "GameStateManager.h"
#include "GameStateList.h"
#include "Global.h"

// ----------------------------------------------------
// Function declarations

void initializeApp(void);
void exitApp(void);
void gameLoop(void);

// ----------------------------------------------------
// Application entry point

int main(int argc, const char* argv[])
{
	initializeApp();
	gameLoop();
	exitApp();

	return 0;
}
void gameLoop(void)
{
	// ----------------------------------------------------
	// implementation of the game loop in the simplest form

	while(getCurrent() != LEVEL_QUIT)
	{
		if(getCurrent() == LEVEL_RESTART)
		{
			setCurrent(setNext(getPrev()));
		}
		else
		{
			GSM_Update();
			levelLoad();
		}
		levelInitialize();
		
		while(getCurrent() == getNext())
		{
			levelUpdate();
			levelDraw();
		}

		levelFree();

		if(getNext() != LEVEL_RESTART)
		{
			levelUnload();
		}

		setPrev(getCurrent());
		setCurrent(getNext());
	}
}
void initializeApp(void)
{
	// ---------------------------------------------------
	// Application initialization

	System_Initialize();
	Global_initialize();
	GSM_Initialize(LEVEL_1);
}
void exitApp(void)
{
	// ----------------------------------------------------
	// Application exit

	Global_destroy();
	System_Exit();
}