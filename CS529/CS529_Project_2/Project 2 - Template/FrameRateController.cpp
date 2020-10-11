// ----------------------------------------------------------------------------
// Project Name			:	Game State Manager
// File Name			:	FrameRateController.cpp
// Author			    :	Dan Weiss
// Creation Date		:	September 22, 2010
// Purpose				:	Implements the functions declared for the frame rate
//							controller. Sets up behavior that allows for capped or
//							uncapped frame rates, as well as tracking of all necessary
//							FRC variables.
// History				:
// - 2010/09/22			:	Initial Implementation
// ----------------------------------------------------------------------------

//Needed for QueryPerformanceCounter
#include <windows.h>

#include "FrameRateController.h"

//-----------------------------------------------------------------------------
// Variables used for tracking data over the lifetime of the game.
//-----------------------------------------------------------------------------
double        gFrameRateControlled;
double        gFrameTimeControlled;

double        gFrameTime;
double        gTimeCounter;

double        gFrameRate;
double        gFrameRateAverage;
unsigned long gFrameCounter;

//-----------------------------------------------------------------------------
// Variables used for tracking data over the lifetime of a single frame.
//-----------------------------------------------------------------------------
double        gFrameTimeStart;
double        gFrameTimeEnd;

//-----------------------------------------------------------------------------
// Function: GetTime
//
// Purpose: Utility function that gets the current time in a format that can
//          be used to determine frame times.  This is an implementation that
//          is dependent on the Windows API, but can be achieved in other
//          operating systems through the use of clock() and the CLOCKS_PER_SEC
//          macro in time.h
//
// Return: The current time.
//-----------------------------------------------------------------------------
double GetTime()
{
  __int64 f, t;
	double  r, r0, r1;

	QueryPerformanceFrequency((LARGE_INTEGER*)(&f));
	QueryPerformanceCounter  ((LARGE_INTEGER*)(&t));

	r0 = double(t / f);
	r1 = (t - ((t / f) * f)) / (double)(f);
	r  = r0 + r1;

	return r;
}

//-----------------------------------------------------------------------------
// Function: FRC_Initialize
//
// Purpose: Initializes the variables used by the frame rate controller.
//-----------------------------------------------------------------------------
void FRC_Initialize()
{
  gFrameRateControlled = gFrameRate = 60.0;
  gFrameTime = gFrameRateAverage = gFrameTimeControlled = 1.0 / gFrameRate;
  gFrameCounter = 0;
  gTimeCounter = 0.0;
}

//-----------------------------------------------------------------------------
// Function: FRC_Reset
//
// Purpose: Resets the data used by the frame rate controller.  For the time
//          being, it's being used as a convenience function to pass through
//          the call to initialize, but it can be used in the future for
//          other purposes.
//-----------------------------------------------------------------------------
void FRC_Reset()
{
  FRC_Initialize();
}

//-----------------------------------------------------------------------------
// Function: FRC_StartFrame
//
// Purpose: Records the time at the start of the frame.
//-----------------------------------------------------------------------------
void FRC_StartFrame()
{
  gFrameTimeStart = GetTime();
}

//-----------------------------------------------------------------------------
// Function: FRC_EndFrame
//
// Purpose: Records the time at the end of the frame.  If controlled is enabled,
//          it will block all other processing until the correct amount of time
//          has elapsed.  After this is done, the function calculates all other
//          necessary data.
//-----------------------------------------------------------------------------
void FRC_EndFrame()
{
#if FRC_USE_CONTROLLED == 1
  do
  {
    gFrameTimeEnd = GetTime();
  }while(gFrameTimeEnd - gFrameTimeStart < gFrameTimeControlled);
#else
  gFrameTimeEnd = GetTime();
#endif

  gFrameTime = gFrameTimeEnd - gFrameTimeStart;
  gFrameRate = 1.0 / gFrameTime;
  gFrameCounter++;

  gTimeCounter += gFrameTime;
  gFrameRateAverage = gTimeCounter / gFrameCounter;
}