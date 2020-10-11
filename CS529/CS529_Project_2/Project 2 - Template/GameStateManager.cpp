// ----------------------------------------------------------------------------
// Project Name		:	Cage
// File Name		:	GameStateManager.cpp
// Author			:	Dan Weiss
// Creation Date	:	September 7, 2010
// Purpose			:	Sets up the function pointers used for all behavior in the
//						game state manager.  This section also keeps track of the
//						current, previous, and next states, and has processes to
//						setup the proper set of function pointers.
// History			:
// - 2010/09/07		:	Initial Implementation
// ----------------------------------------------------------------------------

#include "GameStateManager.h"
#include "LevelCage.h"

//-----------------------------------------------------------------------------
// Game state tracking variables.
//-----------------------------------------------------------------------------
unsigned int gGameStateCurr;
unsigned int gGameStatePrev;
unsigned int gGameStateNext;

//-----------------------------------------------------------------------------
// Function pointers used by all states.
//-----------------------------------------------------------------------------
void (*GameStateLoad)();
void (*GameStateInit)();
void (*GameStateUpdate)();
void (*GameStateDraw)();
void (*GameStateFree)();
void (*GameStateUnload)();

//-----------------------------------------------------------------------------
// Function: GSM_Initialize
//
// Purpose: Initializes the game state manager, and initializes the game state
//          to a preset default state.
//-----------------------------------------------------------------------------
void GSM_Initialize( unsigned initialState )
{
	gGameStateCurr = initialState;
	gGameStatePrev = initialState;
	gGameStateNext = initialState;
}

//-----------------------------------------------------------------------------
// Function: GSM_Update
//
// Purpose: Sets the function pointers based on the current state of the game.
//-----------------------------------------------------------------------------
void GSM_Update()
{
	switch(gGameStateCurr)
	{
		case GS_LEVEL_CAGE:
		GameStateLoad   = GameStateLevelCageLoad; 
		GameStateInit   = GameStateLevelCageInit;
		GameStateUpdate = GameStateLevelCageUpdate;
		GameStateDraw   = GameStateLevelCageDraw;
		GameStateFree   = GameStateLevelCageFree;
		GameStateUnload = GameStateLevelCageUnload;
		break;
	}
}