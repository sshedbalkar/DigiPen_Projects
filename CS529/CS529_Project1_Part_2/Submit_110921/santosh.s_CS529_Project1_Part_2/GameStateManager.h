/* Start Header -------------------------------------------------------
Copyright (C) 2011 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name		: GameStateManager.h
Purpose			: Header file defining API implemented by GameStateManager.cpp
Language		: C++/MS C++ 2010 Express
Platform		: Microsoft Visual Studio 2010/i7 2630QM, 8GB DDR3 1600MHz, 3GB NVIDIA GT 555M/Windows 7 Home Premium
Project			: CS529_SantoshShedbalkar_Project1_2
Author			: Santosh Shedbalkar/santosh.s/60001111
Creation date	: 09/19/2011
- End Header --------------------------------------------------------*/
// ---------------------------------------------------------------------------
// Functions to handle intialization before a new state is entered

void GSM_Initialize(int);
void GSM_Update(void);

// ---------------------------------------------------------------------------
// getter setter methods for three important variables which determine the game flow

int getNext(void);
int setNext(int);
int getCurrent(void);
int setCurrent(int);
int getPrev(void);
int setPrev(int);

// ---------------------------------------------------------------------------
// functions that altogether handle a given state

int levelLoad(void);
int levelInitialize(void);
int levelUpdate(void);
int levelDraw(void);
int levelFree(void);
int levelUnload(void);