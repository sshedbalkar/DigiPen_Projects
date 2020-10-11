// ----------------------------------------------------------------------------
// Project Name		:	Cage
// File Name		:	LevelCage.cpp
// Author			:	Dan Weiss, Antoine Abi Chakra
// Creation Date	:	September 7, 2010
// Purpose			:	File that contains the implemented functions for the cage level 's
//						game state.  The functions here are set as the function
//						pointers active during the level 1 game state in the game
//						state manager.
// History			:
// - 2010/09/07		:	Initial Implementation
// ----------------------------------------------------------------------------

#include "LevelCage.h"
#include "GameStateManager.h"
#include "Graphics.h"
#include "Math2D.h"
#include "stdio.h"

#define TEST_PART_2				0

#define DRAW_DEBUG				1									// Set this to 1 in order to draw debug data
#define LINE_SEGMENTS_NUM		5
#define BALL_RADIUS				12
#define PILLARS_NUM				6


extern double					gFrameTime;

static Vector2D					gRoomPoints[LINE_SEGMENTS_NUM * 2];
static LineSegment2D			gRoomLineSegments[LINE_SEGMENTS_NUM];
static Vector2D					gPillarsCenters[PILLARS_NUM];
static float					gPillarsRadii[PILLARS_NUM];
static LineSegment2D			gPillarsWalls[PILLARS_NUM / 2];

static Vector2D					gBallPosition;
static Vector2D					gBallVelocity;


//-----------------------------------------------------------------------------
// Function: GameStateLevel1Load
//
// Purpose: Handles all functionality associated with the first load of the
//          game state.
//-----------------------------------------------------------------------------
void GameStateLevelCageLoad()
{
	// Building map boundaries
	Vector2DSet(gRoomPoints[0], 100, 200);		Vector2DSet(gRoomPoints[1], 150, 550);
	Vector2DSet(gRoomPoints[2], 150, 550);		Vector2DSet(gRoomPoints[3], 650, 550);
	Vector2DSet(gRoomPoints[4], 650, 550);		Vector2DSet(gRoomPoints[5], 700, 200);
	Vector2DSet(gRoomPoints[6], 700, 200);		Vector2DSet(gRoomPoints[7], 400, 50);
	Vector2DSet(gRoomPoints[8], 400, 50);		Vector2DSet(gRoomPoints[9], 100, 200);

	// Build line segments
	for(int i = 0; i < LINE_SEGMENTS_NUM; ++i)
		BuildLineSegment2D(gRoomLineSegments[i], gRoomPoints[i*2], gRoomPoints[i*2 + 1]);

#if(TEST_PART_2)
	// Pillars
	Vector2DSet(gPillarsCenters[0], 180.0f, 280.0f);
	gPillarsRadii[0] = 15;
	
	Vector2DSet(gPillarsCenters[1], 280.0f, 200.0f);
	gPillarsRadii[1] = 20;
	
	Vector2DSet(gPillarsCenters[2], 610.0f, 320.0f);
	gPillarsRadii[2] = 15;

	Vector2DSet(gPillarsCenters[3], 550.0f, 200.0f);
	gPillarsRadii[3] = 15;
	
	Vector2DSet(gPillarsCenters[4], 340.0f, 450.0f);
	gPillarsRadii[4] = 20;
	
	Vector2DSet(gPillarsCenters[5], 470.0f, 480.0f);
	gPillarsRadii[5] = 15;

	// Pillars walls
	for(int i = 0; i < PILLARS_NUM/2; ++i)
		BuildLineSegment2D(gPillarsWalls[i], gPillarsCenters[i*2], gPillarsCenters[i*2 + 1]);

#endif
}

//-----------------------------------------------------------------------------
// Function: GameStateLevel1Init
//
// Purpose: Handles all functionality associated with initialization of the
//          game state.  This can (and often will be) called without load being
//          called, allowing us to use this function to safe reset the state.
//-----------------------------------------------------------------------------
void GameStateLevelCageInit()
{	
	Vector2DSet(gBallPosition, 490.0f, 300.0f);
	Vector2DSet(gBallVelocity, 0.0f, 200.0f);
}

//-----------------------------------------------------------------------------
// Function: GameStateLevel1Update
//
// Purpose: Handles all functionality associated with updating the current game
//          state.  This function will get called once per frame.
//-----------------------------------------------------------------------------
void GameStateLevelCageUpdate()
{
	// Update the positions of objects

	Vector2D newBallPos;
	Vector2DScaleAdd(newBallPos, gBallVelocity, gBallPosition, (float)gFrameTime);
	Vector2D intersectionPoint;

	float smallestT = -1.0f;
	Vector2D closestIntersectionPoint, r;

	// Collision with line segments
	for(int i = 0; i < LINE_SEGMENTS_NUM; ++i)
	{
		float t = ReflectAnimatedCircleOnStaticLineSegment(gBallPosition, newBallPos, BALL_RADIUS, gRoomLineSegments[i], intersectionPoint, r);

		if(t > 0.0f && (t < smallestT || smallestT < 0.0f))
		{
			closestIntersectionPoint = intersectionPoint;
			smallestT = t;
		}
	}

#if(TEST_PART_2)
	// Collision with pillars (Static circles)
	for(int i = 0; i < PILLARS_NUM; ++i)
	{
		float t = ReflectAnimatedCircleOnStaticCircle(gBallPosition, newBallPos, BALL_RADIUS, gPillarsCenters[i], gPillarsRadii[i], intersectionPoint, r);

		if(t > 0.0f && (t < smallestT || smallestT < 0.0f))
		{
			closestIntersectionPoint = intersectionPoint;
			smallestT = t;
		}
	}

	// Collision with pillars' walls (Line segments between the static circles)
	for(int i = 0; i < PILLARS_NUM/2; ++i)
	{
		float t = ReflectAnimatedCircleOnStaticLineSegment(gBallPosition, newBallPos, BALL_RADIUS, gPillarsWalls[i], intersectionPoint, r);

		if(t > 0.0f && (t < smallestT || smallestT < 0.0f))
		{
			closestIntersectionPoint = intersectionPoint;
			smallestT = t;
		}
	}
#endif

	if(smallestT > 0.0)
	{
		Vector2DAdd(gBallPosition, closestIntersectionPoint, r);
		Vector2DNormalize(r, r);
		Vector2DScale(gBallVelocity, r,  Vector2DLength(gBallVelocity));
	}
	
	Vector2DScaleAdd(gBallPosition, gBallVelocity, gBallPosition, (float)gFrameTime);
}

//-----------------------------------------------------------------------------
// Function: GameStateLevel1Draw
//
// Purpose: Handles all functionality associated with rendering the curreent
//          frame.  This function will get called once per frame.
//-----------------------------------------------------------------------------
void GameStateLevelCageDraw()
{
	GFX_BeginDraw();

	// Drawing the map
	GFX_DrawLine((int)gRoomPoints[0].x, (int)gRoomPoints[0].y, (int)gRoomPoints[1].x, (int)gRoomPoints[1].y, 0x00000000);
	GFX_DrawLine((int)gRoomPoints[2].x, (int)gRoomPoints[2].y, (int)gRoomPoints[3].x, (int)gRoomPoints[3].y, 0x00000000);
	GFX_DrawLine((int)gRoomPoints[4].x, (int)gRoomPoints[4].y, (int)gRoomPoints[5].x, (int)gRoomPoints[5].y, 0x00000000);
	GFX_DrawLine((int)gRoomPoints[5].x, (int)gRoomPoints[6].y, (int)gRoomPoints[7].x, (int)gRoomPoints[7].y, 0x00000000);
	GFX_DrawLine((int)gRoomPoints[8].x, (int)gRoomPoints[8].y, (int)gRoomPoints[9].x, (int)gRoomPoints[9].y, 0x00000000);

#if(TEST_PART_2)
	// Drawing the pillars
	for(int i = 0; i < PILLARS_NUM; ++i)
		GFX_DrawEllipse((int)gPillarsCenters[i].x, (int)gPillarsCenters[i].y, (int)gPillarsRadii[i], (int)gPillarsRadii[i], 0x00000000);

	// Drawing the pillars walls
	for(int i = 0; i < PILLARS_NUM/2; ++i)
		GFX_DrawLine((int)gPillarsWalls[i].mP0.x, (int)gPillarsWalls[i].mP0.y, (int)gPillarsWalls[i].mP1.x, (int)gPillarsWalls[i].mP1.y, 0x00000000);

#endif

	// Drawing the ball
	GFX_DrawEllipse((int)gBallPosition.x, (int)gBallPosition.y, BALL_RADIUS, BALL_RADIUS, 0x007fFF);

#if(DRAW_DEBUG)
	Vector2D midPoint, normalEnd;
	// Debug - Drawing the normal vectors of the room's walls
	for(int i = 0; i < LINE_SEGMENTS_NUM; ++i)
	{
		Vector2DAdd(midPoint, gRoomLineSegments[i].mP0, gRoomLineSegments[i].mP1);
		Vector2DScale(midPoint, midPoint, 0.5f);
		Vector2DScaleAdd(normalEnd, gRoomLineSegments[i].mN, midPoint, 30.0f);

		GFX_DrawLine(midPoint.x, midPoint.y, normalEnd.x, normalEnd.y, 0x00000000);
	}

#if(TEST_PART_2)
	// Debug - Drawing the normal vectors of the pillars' walls
	for(int i = 0; i < PILLARS_NUM/2; ++i)
	{
		Vector2DAdd(midPoint, gPillarsWalls[i].mP0, gPillarsWalls[i].mP1);
		Vector2DScale(midPoint, midPoint, 0.5f);
		Vector2DScaleAdd(normalEnd, gPillarsWalls[i].mN, midPoint, 30.0f);

		GFX_DrawLine(midPoint.x, midPoint.y, normalEnd.x, normalEnd.y, 0x00000000);
	}
#endif

#endif
	

	//GFX_DrawRectangle(100, 100, 100, 100, 0x00FF0000);
	//GFX_DrawEllipse(200, 200, 100, 100, 0x0000FF00);
	//GFX_DrawLine(250, 250, 300, 400, 0x000000FF);

	char txt[256];
	memset(txt, 0, 256*sizeof(char));
	sprintf_s(txt, "%f", gFrameTime);
	GFX_DrawText(0, 20, txt, strlen(txt), 0x00000000);

	GFX_DrawFrameRate(0, 0, 0x00000000);

	GFX_EndDraw();
}

//-----------------------------------------------------------------------------
// Function: GameStateLevel1Free
//
// Purpose: Handles all functionality associated with freeing data in the
//          current state.  Because this function can be called without unload
//          being called, it should only free data that gets set in the Init
//          function for the state.
//-----------------------------------------------------------------------------
void GameStateLevelCageFree()
{

}

//-----------------------------------------------------------------------------
// Function: GameStateLevel1Unload
//
// Purpose: Handles all functionality associated with complete unloading of the
//          state.  Any data that is not unloaded by the Free function of the
//          state should be handled here.
//-----------------------------------------------------------------------------
void GameStateLevelCageUnload()
{
 
}