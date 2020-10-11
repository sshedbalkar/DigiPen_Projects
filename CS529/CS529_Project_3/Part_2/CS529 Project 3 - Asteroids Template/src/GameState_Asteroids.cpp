// ---------------------------------------------------------------------------
// Project Name		:	Asteroid Game
// File Name		:	GameState_Play.cpp
// Author			:	Sun Tjen Fam
// Creation Date	:	2008/01/31
// Purpose			:	implementation of the 'play' game state
// History			:
// - 2008/01/31		:	- initial implementation
// ---------------------------------------------------------------------------

#include "main.h"

// ---------------------------------------------------------------------------
// Defines

#define GAME_OBJ_NUM_MAX			32			//The total number of different objects (Shapes)
#define GAME_OBJ_INST_NUM_MAX		2048		//The total number of different game object instances


#define SHIP_INITIAL_NUM			3			// initial number of ship lives
#define SHIP_SIZE					3.0f		// ship size
#define SHIP_ACCEL_FORWARD			50.0f		// ship forward acceleration (in m/s^2)
#define SHIP_ACCEL_BACKWARD			-100.0f		// ship backward acceleration (in m/s^2)
#define SHIP_ROT_SPEED				(2.0f * PI)	// ship rotation speed (degree/second)

#define BULLET_SPEED				100.0f		// bullet speed (m/s)

// ---------------------------------------------------------------------------
enum TYPE
{
	// list of game object types
	TYPE_SHIP = 0, 
	TYPE_BULLET,
	TYPE_ASTEROID,

	TYPE_NUM
};

// ---------------------------------------------------------------------------
// object flag definition

#define FLAG_ACTIVE		0x00000001

// ---------------------------------------------------------------------------
// Struct/Class definitions

//Game object structure
struct GameObj
{
	unsigned long		type;		// object type
	AEGfxTriList*		pMesh;		// This will hold the triangles which will form the shape of the object
};

// ---------------------------------------------------------------------------

//Game object instance structure
struct GameObjInst
{
	GameObj*			pObject;	// pointer to the 'original' shape
	unsigned long		flag;		// bit flag or-ed together
	float				scale;		// scaling value of the object instance
	AEVec2				posCurr;	// object current position
	AEVec2				velCurr;	// object current velocity
	float				dirCurr;	// object current direction

	AEMtx33				transform;	// object transformation matrix: Each frame, calculate the object instance's transformation matrix and save it here
};

// ---------------------------------------------------------------------------
// Static variables

// list of original object
static GameObj				sGameObjList[GAME_OBJ_NUM_MAX];				// Each element in this array represents a unique game object (shape)
static unsigned long		sGameObjNum;								// The number of defined game objects

// list of object instances
static GameObjInst			sGameObjInstList[GAME_OBJ_INST_NUM_MAX];	// Each element in this array represents a unique game object instance (sprite)
static unsigned long		sGameObjInstNum;							// The number of used game object instances

// pointer ot the ship object
static GameObjInst*			spShip;										// Pointer to the "Ship" game object instance

// number of ship available (lives 0 = game over)
static long					sShipLives;									// The number of lives left

// the score = number of asteroid destroyed
static unsigned long		sScore;										// Current score

// ---------------------------------------------------------------------------

// functions to create/destroy a game object instance
static GameObjInst*			gameObjInstCreate (unsigned long type, float scale, AEVec2* pPos, AEVec2* pVel, float dir);
static void					gameObjInstDestroy(GameObjInst* pInst);

// ---------------------------------------------------------------------------

// "Load" function of this state
void GameStateAsteroidsLoad(void)
{
	// zero the game object array
	memset(sGameObjList, 0, sizeof(GameObj) * GAME_OBJ_NUM_MAX);
	// No game objects (shapes) at this point
	sGameObjNum = 0;

	// zero the game object instance array
	memset(sGameObjInstList, 0, sizeof(GameObjInst) * GAME_OBJ_INST_NUM_MAX);
	// No game object instances (sprites) at this point
	sGameObjInstNum = 0;

	// The ship object instance hasn't been created yet, so this "spShip" pointer is initialized to 0
	spShip = 0;

	// load/create the mesh data (game objects / Shapes)
	GameObj* pObj;

	// =====================
	// create the ship shape
	// =====================

	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_SHIP;

	AEGfxTriStart();
	AEGfxTriAdd(
		-0.5f,  0.5f, 0x01FF0000, 
		-0.5f, -0.5f, 0xFFFF0000,
		 0.5f,  0.0f, 0xFFFFFFFF); 

	pObj->pMesh = AEGfxTriEnd();
	AE_ASSERT_MESG(pObj->pMesh, "failed to create object!!");


	// =======================
	// create the bullet shape
	// =======================



	// =========================
	// create the asteroid shape
	// =========================
}

// ---------------------------------------------------------------------------

// "Initialize" function of this state
void GameStateAsteroidsInit(void)
{
	// create the main ship
	spShip = gameObjInstCreate(TYPE_SHIP, SHIP_SIZE, 0, 0, 0.0f);
	AE_ASSERT(spShip);
	
	// CREATE THE INITIAL ASTEROIDS INSATNCES USING THE "GAMEOBJINSTCREATE" FUNCTION

	// reset the score and the number of ship
	sScore      = 0;
	sShipLives    = SHIP_INITIAL_NUM;
}

// ---------------------------------------------------------------------------

// "Update" function of this state
void GameStateAsteroidsUpdate(void)
{
	// =========================
	// update according to input
	// =========================

	// This input handling moves the ship without any velocity nor acceleration
	// It should be changed when implementing the Asteroids project
	if (AEInputCheckCurr(DIK_UP))
	{
		AEVec2 added;
		AEVec2Set(&added, cosf(spShip->dirCurr), sinf(spShip->dirCurr));
		AEVec2Add(&spShip->posCurr, &spShip->posCurr, &added);
	}

	if (AEInputCheckCurr(DIK_DOWN))
	{
		AEVec2 added;
		AEVec2Set(&added, -cosf(spShip->dirCurr), -sinf(spShip->dirCurr));
		AEVec2Add(&spShip->posCurr,	 &spShip->posCurr, &added);
	}

	if (AEInputCheckCurr(DIK_LEFT))
	{
		spShip->dirCurr += SHIP_ROT_SPEED * (float)(gAEFrameTime);
		spShip->dirCurr =  AEWrap(spShip->dirCurr, -PI, PI);
	}

	if (AEInputCheckCurr(DIK_RIGHT))
	{
		spShip->dirCurr -= SHIP_ROT_SPEED * (float)(gAEFrameTime);
		spShip->dirCurr =  AEWrap(spShip->dirCurr, -PI, PI);
	}


	// Shoot a bullet if space is triggered (Create a new object instance)
	//if (AEInputCheckTriggered(DIK_SPACE))


	// ==================================================
	// update physics of all active game object instances
	//	-- Positions are updated here
	// ==================================================
	

	// ===================================
	// update active game object instances
	// Example:
	//		-- Wrap specific object instances around the world
	//		-- Removing the bullets as they go out of bounds
	//		-- If you have a homing missile for example, compute its new orientation (Homing missiles are not required for the Asteroids project)
	//		-- Update particle effects (Not required for the Asteroids project)
	// ===================================
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;
		
		// check if the object is a ship
		if (pInst->pObject->type == TYPE_SHIP)
		{
			// warp the ship from one end of the screen to the other
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, gAEWinMinX - SHIP_SIZE, gAEWinMaxX + SHIP_SIZE);
			pInst->posCurr.y = AEWrap(pInst->posCurr.y, gAEWinMinY - SHIP_SIZE, gAEWinMaxY + SHIP_SIZE);
		}

		// Wrap asteroids here


		// Remove bullets that go out of bounds
	}


	// ====================
	// check for collision
	// ====================
	
	/*
	for each object instance: oi1
		if oi1 is not active
			skip

		if oi1 is an asteroid
			for each object instance oi2
				if(oi2 is not active or oi2 is an asteroid)
					skip

				if(oi2 is the ship)
					Check for collision between ship and asteroids (Rectangle - Rectangle)
					Update game behavior accordingly
					Update "Object instances array"
				else
				if(oi2 is a bullet)
					Check for collision between ship and asteroids (Rectangle - Point)
					Update game behavior accordingly
					Update "Object instances array"
	*/

	// ==============================================
	// calculate the matrix for all objects instances
	// ==============================================

	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;
		AEMtx33		 trans, rot, scale;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// Compute the scaling matrix
		// Compute the rotation matrix 
		// Compute the translation matrix
		// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix

	}
}

// ---------------------------------------------------------------------------

void GameStateAsteroidsDraw(void)
{
	char strBuffer[1024];

	// draw all object instances in the list
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;
		

		// Set the current object instance's transform matrix using "AEGfxSetTransform"
		// Draw the shape used by the current object instance using "AEGfxTriDraw"
	}

	sprintf_s(strBuffer, "Score: %d", sScore);
	AEGfxPrint(10, 10, -1, strBuffer);

	sprintf_s(strBuffer, "Ship Left: %d", sShipLives >= 0 ? sShipLives : 0);
	AEGfxPrint(600, 10, -1, strBuffer);

	// display the game over message
	if (sShipLives < 0)
		AEGfxPrint(280, 260, 0xFFFFFFFF, "       GAME OVER       ");
}

// ---------------------------------------------------------------------------

void GameStateAsteroidsFree(void)
{
	// kill all object instances in the array using "gameObjInstDestroy"
}

// ---------------------------------------------------------------------------

void GameStateAsteroidsUnload(void)
{
	// free all mesh data (shapes) of each object using "AEGfxTriFree"
}

// ---------------------------------------------------------------------------

GameObjInst* gameObjInstCreate(unsigned long type, float scale, AEVec2* pPos, AEVec2* pVel, float dir)
{
	AEVec2 zero = { 0.0f, 0.0f };

	AE_ASSERT_PARM(type < sGameObjNum);
	
	// loop through the object instance list to find a non-used object instance
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// check if current instance is not used
		if (pInst->flag == 0)
		{
			// it is not used => use it to create the new instance
			pInst->pObject	= sGameObjList + type;
			pInst->flag		= FLAG_ACTIVE;
			pInst->scale	= scale;
			pInst->posCurr	= pPos ? *pPos : zero;
			pInst->velCurr	= pVel ? *pVel : zero;
			pInst->dirCurr	= dir;
			
			// return the newly created instance
			return pInst;
		}
	}

	// cannot find empty slot => return 0
	return 0;
}

// ---------------------------------------------------------------------------

void gameObjInstDestroy(GameObjInst* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->flag == 0)
		return;

	// zero out the flag
	pInst->flag = 0;

	// Do other stuff if needed
}
// ---------------------------------------------------------------------------