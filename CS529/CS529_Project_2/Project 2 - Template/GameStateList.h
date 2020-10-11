// ----------------------------------------------------------------------------
// Project Name		:	Cage
// File Name		:	GameStateList.h
// Author			:	Dan Weiss
// Creation Date	:	September 7, 2010
// Purpose			:	Contains an enumeration of all the possible states that
//						are implemented in the game.
// History			:
// - 2010/09/07		:	Initial Implementation
// ----------------------------------------------------------------------------

#ifndef GAMESTATELIST__H__
#define GAMESTATELIST__H__

enum
{
  GS_LEVEL_CAGE = 0,

  GS_LEVEL_NUM,			// Level Counter

  GS_RESTART,
  GS_QUIT
};

#endif