// ----------------------------------------------------------------------------
// Project Name		:	Game State Manager
// File Name		  :	GameStateManager.h
// Author			    :	Dan Weiss
// Creation Date	:	September 7, 2010
// Purpose			  :	Externs and declarations of the functionality used in the
//                  game state manager.
// History			  :
// - 2010/09/07		:	Initial Implementation
// ----------------------------------------------------------------------------

#ifndef GSM__H__
#define GSM__H__

#include "GameStateList.h"

// ---------------------------------------------------------------------------

extern unsigned int gGameStateCurr;
extern unsigned int gGameStatePrev;
extern unsigned int gGameStateNext;

// ---------------------------------------------------------------------------

extern void (*GameStateLoad)();
extern void (*GameStateInit)();
extern void (*GameStateUpdate)();
extern void (*GameStateDraw)();
extern void (*GameStateFree)();
extern void (*GameStateUnload)();

// ---------------------------------------------------------------------------

void GSM_Initialize( unsigned initialState );
void GSM_Update();

#endif