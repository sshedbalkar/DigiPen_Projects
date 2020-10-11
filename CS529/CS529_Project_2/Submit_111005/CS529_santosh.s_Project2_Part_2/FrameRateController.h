// ----------------------------------------------------------------------------
// Project Name		:	Game State Manager
// File Name		  :	FrameRateController.h
// Author			    :	Dan Weiss
// Creation Date	:	September 22, 2010
// Purpose			  :	Declares functions necessary for the frame rate controller.
//                  Also defines FRC_USE_CONTROLLED which allows for toggling the
//                  CONTROLLED capabilities on and off for testing.
// History			  :
// - 2010/09/22		:	Initial Implementation
// ----------------------------------------------------------------------------

#ifndef FRAMERATECONTROLLER__H__
#define FRAMERATECONTROLLER__H__

#define FRC_USE_CONTROLLED 1

extern double        gFrameRateControlled;
extern double        gFrameTimeControlled;
extern double        gFrameTime;
extern double        gTimeCounter;
extern double        gFrameRate;
extern double        gFrameRateAverage;
extern unsigned long gFrameCounter;

void FRC_StartFrame();
void FRC_EndFrame();

void FRC_Initialize();
void FRC_Reset();

#endif