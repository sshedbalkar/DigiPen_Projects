// ---------------------------------------------------------------------------
// Project Name		:	Platformer
// File Name		:	GameState_Platformer.cpp
// Author			:	Sun Tjen Fam, Samir Abou Samra, Antoine Abi Chakra
// Creation Date	:	2008/03/04
// Purpose			:	Implementation of the platform game state
// History			:
//	- 
// ---------------------------------------------------------------------------


#include "Main.h"

#define GAME_OBJ_NUM_MAX			32
#define GAME_OBJ_INST_NUM_MAX		2048

//Gameplay related variables and values
#define GRAVITY -20.0f
#define JUMP_VELOCITY 11.0f
#define MOVE_VELOCITY_HERO 4.0f
#define MOVE_VELOCITY_ENEMY 7.5f
#define ENEMY_IDLE_TIME 2.0
#define HERO_LIVES 3
static int HeroLives;
static int Hero_Initial_X;
static int Hero_Initial_Y;
static int TotalCoins;

//Flags
#define FLAG_ACTIVE			0x00000001
#define FLAG_VISIBLE		0x00000002
#define FLAG_NON_COLLIDABLE	0x00000004


enum TYPE_OBJECT
{
	TYPE_OBJECT_EMPTY,			//0
	TYPE_OBJECT_COLLISION,		//1
	TYPE_OBJECT_HERO,			//2
	TYPE_OBJECT_ENEMY1,			//3
	TYPE_OBJECT_COIN			//4
};

//State machine states
enum STATE
{
	STATE_NONE,
	STATE_GOING_LEFT,
	STATE_GOING_RIGHT
};

//State machine inner states
enum INNER_STATE
{
	INNER_STATE_ON_ENTER,
	INNER_STATE_ON_UPDATE,
	INNER_STATE_ON_EXIT
};


struct GameObj
{
	unsigned int	type;		// object type
	AEGfxTriList*	pMesh;		// pbject
};


struct GameObjInst
{
	GameObj*		pObject;	// pointer to the 'original'
	unsigned int	flag;		// bit flag or-ed together
	float			scale;
	AEVec2			posCurr;	// object current position
	AEVec2			velCurr;	// object current velocity
	float			dirCurr;	// object current direction

	AEMtx33			transform;	// object drawing matrix

	//Used to hold the current 
	int				gridCollisionFlag;

	// pointer to custom data specific for each object type
	void*			pUserData;

	//State of the object instance
	enum			STATE state;
	enum			INNER_STATE innerState;

	//General purpose counter (This variable will be used for the enemy state machine)
	double			counter;
};


// list of original objects
static GameObj			*sGameObjList;
static unsigned int		sGameObjNum;

// list of object instances
static GameObjInst		*sGameObjInstList;
static unsigned int		sGameObjInstNum;

//Binary map data
static int **MapData;
static int **BinaryCollisionArray;
static int BINARY_MAP_WIDTH;
static int BINARY_MAP_HEIGHT;
static GameObjInst *pBlackInstance;
static GameObjInst *pWhiteInstance;
static AEMtx33 MapTransform;
int GetCellValue(int X, int Y);
int CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY);
void SnapToCell(float *Coordinate);
int ImportMapDataFromFile(char *FileName);
void FreeMapData(void);


//Collision flags
#define	COLLISION_LEFT		0x00000001	//0001
#define	COLLISION_RIGHT		0x00000002	//0010
#define	COLLISION_TOP		0x00000004	//0100
#define	COLLISION_BOTTOM	0x00000008	//1000



// function to create/destroy a game object instance
static GameObjInst*	gameObjInstCreate (unsigned int type, float scale, AEVec2* pPos, AEVec2* pVel, float dir, enum STATE startState);
static void			gameObjInstDestroy(GameObjInst* pInst);

//We need a pointer to the hero's instance for input purposes
static GameObjInst *pHero;

//State machine functions
void EnemyStateMachine(GameObjInst *pInst);


void GameStatePlatformLoad(void)
{
	sGameObjList = (GameObj *)calloc(GAME_OBJ_NUM_MAX, sizeof(GameObj));
	sGameObjInstList = (GameObjInst *)calloc(GAME_OBJ_INST_NUM_MAX, sizeof(GameObjInst));
	sGameObjNum = 0;


	GameObj* pObj;

	//Creating the black object
	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_OBJECT_EMPTY;

	AEGfxTriStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF000000, 
		 0.5f,  -0.5f, 0xFF000000, 
		-0.5f,  0.5f, 0xFF000000);
	
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFF000000, 
		 0.5f,  -0.5f, 0xFF000000, 
		0.5f,  0.5f, 0xFF000000);

	pObj->pMesh = AEGfxTriEnd();

		
	
	//Creating the white object
	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_OBJECT_COLLISION;

	AEGfxTriStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 
		 0.5f,  -0.5f, 0xFFFFFFFF, 
		-0.5f,  0.5f, 0xFFFFFFFF);
	
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFFFFFF, 
		 0.5f,  -0.5f, 0xFFFFFFFF, 
		0.5f,  0.5f, 0xFFFFFFFF);

	pObj->pMesh = AEGfxTriEnd();


	//Creating the hero object
	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_OBJECT_HERO;

	AEGfxTriStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF0000FF, 
		 0.5f,  -0.5f, 0xFF0000FF, 
		-0.5f,  0.5f, 0xFF0000FF);
	
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFF0000FF, 
		 0.5f,  -0.5f, 0xFF0000FF, 
		0.5f,  0.5f, 0xFF0000FF);

	pObj->pMesh = AEGfxTriEnd();


	//Creating the enemey1 object
	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_OBJECT_ENEMY1;

	AEGfxTriStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 
		 0.5f,  -0.5f, 0xFFFF0000, 
		-0.5f,  0.5f, 0xFFFF0000);
	
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFF0000, 
		 0.5f,  -0.5f, 0xFFFF0000, 
		0.5f,  0.5f, 0xFFFF0000);

	pObj->pMesh = AEGfxTriEnd();


	//Creating the Coin object
	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_OBJECT_COIN;

	AEGfxTriStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB

	//Creating the circle shape
	float CircleAngleStep = PI/12.0f, i;
	int Parts = 12;
	for(i = 0; i < Parts; ++i)
	{
		AEGfxTriAdd(
		0.0f, 0.0f, 0xFFFFFF00, 
		cosf(i*2*PI/Parts)*0.5f,  sinf(i*2*PI/Parts)*0.5f, 0xFFFFFF00, 
		cosf((i+1)*2*PI/Parts)*0.5f,  sinf((i+1)*2*PI/Parts)*0.5f, 0xFFFFFF00);
	}

	pObj->pMesh = AEGfxTriEnd();

	//Setting intital binary map values
	MapData = 0;
	BinaryCollisionArray = 0;
	BINARY_MAP_WIDTH = 0;
	BINARY_MAP_HEIGHT = 0;

	//Importing Data
	if(!ImportMapDataFromFile("Exported.txt"))
		gGameStateNext = GS_QUIT;


	//Computing the matrix which take a point out of the normalized coordinates system
	//of the binary map
	/***********
	Compute a transformation matrix and save it in "MapTransform".
	This transformation transforms any point from the normalized coordinates system of the binary map.
	Later on, when rendering each object instance, we should concatenate "MapTransform" with the
	object instance's own transformation matrix

	Compute a translation matrix (-Grid width/2, -Grid height/2) and save it in "trans"
	Compute a scaling matrix and save it in "scale)
	Concatenate scale * translate and save the result in "MapTransform"
	***********/
	AEMtx33 scale, trans;
}

void GameStatePlatformInit(void)
{
	int i, j;

	pHero = 0;
	pBlackInstance = 0;
	pWhiteInstance = 0;
	TotalCoins = 0;

	//Create an object instance representing the black cell.
	//This object instance should not be visible. When rendering the grid cells, each time we have
	//a non collision cell, we position this instance in the correct location and then we render it
	pBlackInstance = gameObjInstCreate(TYPE_OBJECT_EMPTY, 1.0f, 0, 0, 0.0f, STATE_NONE);
	pBlackInstance->flag ^= FLAG_VISIBLE;
	pBlackInstance->flag |= FLAG_NON_COLLIDABLE;

	//Create an object instance representing the white cell.
	//This object instance should not be visible. When rendering the grid cells, each time we have
	//a collision cell, we position this instance in the correct location and then we render it
	pWhiteInstance = gameObjInstCreate(TYPE_OBJECT_COLLISION, 1.0f, 0, 0, 0.0f, STATE_NONE);
	pWhiteInstance->flag ^= FLAG_VISIBLE;
	pWhiteInstance->flag |= FLAG_NON_COLLIDABLE;

	//Setting the inital number of hero lives
	HeroLives = HERO_LIVES;

	GameObjInst *pInst;
	AEVec2 Pos;
	//creating the main character, the enemies and the coins according to their initial positions in MapData

	/***********
	Loop through all the array elements of MapData (which was initialized in the "GameStatePlatformLoad" function
	from the .txt file
		if the element represents a collidable or non collidable area
			don't do anything

		if the element represents the hero
			Create a hero instance
			Set its position depending on its array indices in MapData
			Save its array indices in Hero_Initial_X and Hero_Initial_Y (Used when the hero dies and its position needs to be reset)

		if the element represents an enemy
			Create an enemy instance
			Set its position depending on its array indices in MapData
			
		if the element represents a coin
			Create a coin instance
			Set its position depending on its array indices in MapData
			
	***********/
	for(i = 0; i < BINARY_MAP_WIDTH; ++i)
		for(j = 0; j < BINARY_MAP_HEIGHT; ++j)
		{
		}
}

void GameStatePlatformUpdate(void)
{
	int i, j;
	GameObjInst *pInst;

	//Handle Input
	/***********
	if right is pressed
		Set hero velocity X to MOVE_VELOCITY_HERO
	else
	if left is pressed
		Set hero velocity X to -MOVE_VELOCITY_HERO
	else
		Set hero velocity X to 0

	if space is pressed AND Hero is colliding from the bottom
		Set hero velocity Y to JUMP_VELOCITY

	if q is pressed
		Exit game
	***********/


	//Update object instances physics and behavior
	for(i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		GameObjInst *pInst = sGameObjInstList + i;

		// skip non-active object
		if (0 == (pInst->flag & FLAG_ACTIVE))
			continue;


		/****************
		Apply gravity
			Velcoty Y = Gravity * Frame Time + Velocity Y

		If object instance is an enemy
			Apply enemy state machine
		****************/
	}

	//Update object instances positions
	for(i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		GameObjInst *pInst = sGameObjInstList + i;

		// skip non-active object
		if (0 == (pInst->flag & FLAG_ACTIVE))
			continue;

		/**********
		update the position using: P1 = V1*t + P0
		**********/
	}

	//Check for grid collision
	for(i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		GameObjInst *pInst = sGameObjInstList + i;

		// skip non-active object instances
		if (0 == (pInst->flag & FLAG_ACTIVE))
			continue;

		/*************
		Update grid collision flag

		if collision from bottom
			Snap to cell on Y axis
			Velocity Y = 0

		if collision from top
			Snap to cell on Y axis
			Velocity Y = 0
	
		if collision from left
			Snap to cell on X axis
			Velocity X = 0

		if collision from right
			Snap to cell on X axis
			Velocity X = 0
		*************/
	}


	//Checking for collision among object instances:
	//Hero against enemies
	//Hero against coins

	/**********
	for each game object instance
		Skip if it's inactive or if it's non collidable

		If it's an enemy
			If collision between the enemy instance and the hero (rectangle - rectangle)
				Decrement hero lives
				Reset the hero's position in case it has lives left, otherwise restart the level

		If it's a coin
			If collision between the coin instance and the hero (circle - rectangle)
				Remove the coin and decrement the coin counter.
				Quit the game in case no more coins are left
	**********/
	for(i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
	}

	
	//Computing the transformation matrices of the game object instances
	for(i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		AEMtx33 scale, rot, trans;
		GameObjInst *pInst = sGameObjInstList + i;

		// skip non-active object
		if (0 == (pInst->flag & FLAG_ACTIVE))
			continue;

		AEMtx33Scale(&scale, pInst->scale, pInst->scale);
		AEMtx33Rot(&rot, pInst->dirCurr);
		AEMtx33Trans(&trans, pInst->posCurr.x, pInst->posCurr.y);

		AEMtx33Concat(&pInst->transform, &trans, &rot);
		AEMtx33Concat(&pInst->transform, &pInst->transform, &scale);
	}
}

void GameStatePlatformDraw(void)
{
	//Drawing the tile map (the grid)
	int i, j;
	AEMtx33 cellTranslation, cellFinalTransformation;

	//Drawing the tile map

	/******REMINDER*****
	You need to concatenate MapTransform with the transformation matrix of any object you want to draw.
	MapTransform transform the instance from the normalized coordinates system of the binary map
	*******************/

	/*********
	for each array element in BinaryCollisionArray (2 loops)
		Compute the cell's translation matrix acoording to its X and Y coordinates and save it in "cellTranslation"
		Concatenate MapTransform with the cell's transformation and save the result in "cellFinalTransformation"
		Send the resultant matrix to the graphics manager using "AEGfxSetTransform"

		Draw the instance's shape depending on the cell's value using "AEGfxTriDraw"
			Use the black instance in case the cell's value is TYPE_OBJECT_EMPTY
			Use the white instance in case the cell's value is TYPE_OBJECT_COLLISION
	*********/
	for(i = 0; i < BINARY_MAP_WIDTH; ++i)
		for(j = 0; j < BINARY_MAP_HEIGHT; ++j)
		{
		}



	//Drawing the object instances
	/**********
	For each active and visible object instance
		Concatenate MapTransform with its transformation matrix
		Send the resultant matrix to the graphics manager using "AEGfxSetTransform"
		Draw the instance's shape using "AEGfxTriDraw"
	**********/
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if (0 == (pInst->flag & FLAG_ACTIVE) || 0 == (pInst->flag & FLAG_VISIBLE))
			continue;
		
		//Don't forget to concatenate the MapTransform matrix with the transformation of each game object instance

	}

	char strBuffer[100];
	memset(strBuffer, 0, 100*sizeof(char));
	sprintf(strBuffer, "Frame Time:  %.6f", gAEFrameTime);
	AEGfxPrint(40, 30, -1, strBuffer);
	memset(strBuffer, 0, 100*sizeof(char));
	sprintf(strBuffer, "Lives:  %i", HeroLives);
	AEGfxPrint(650, 30, -1, strBuffer);	
}


void GameStatePlatformFree(void)
{
	// kill all object in the list
	for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
		gameObjInstDestroy(sGameObjInstList + i);
}

void GameStatePlatformUnload(void)
{
	// free all CREATED mesh
	for (u32 i = 0; i < sGameObjNum; i++)
		AEGfxTriFree(sGameObjList[i].pMesh);

	/*********
	Free the map data
	*********/
}


GameObjInst* gameObjInstCreate(unsigned int type, float scale, AEVec2* pPos, AEVec2* pVel, float dir, enum STATE startState)
{
	AEVec2 zero = { 0.0f, 0.0f };

	AE_ASSERT_PARM(type < sGameObjNum);
	
	// loop through the object instance list to find a non-used object instance
	for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// check if current instance is not used
		if (pInst->flag == 0)
		{
			// it is not used => use it to create the new instance
			pInst->pObject	 = sGameObjList + type;
			pInst->flag		 = FLAG_ACTIVE | FLAG_VISIBLE;
			pInst->scale	 = scale;
			pInst->posCurr	 = pPos ? *pPos : zero;
			pInst->velCurr	 = pVel ? *pVel : zero;
			pInst->dirCurr	 = dir;
			pInst->pUserData = 0;
			pInst->gridCollisionFlag = 0;
			pInst->state = startState;
			pInst->innerState = INNER_STATE_ON_ENTER;
			pInst->counter = 0;
			
			// return the newly created instance
			return pInst;
		}
	}
}

void gameObjInstDestroy(GameObjInst* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->flag == 0)
		return;

	// zero out the flag
	pInst->flag = 0;
}

int GetCellValue(int X, int Y)
{
	return 0;
}


int CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY)
{
	//At the end of this function, "Flag" will be used to determine which sides
	//of the object instance are colliding. 2 hot spots will be placed on each side.
	return 0;
}


void SnapToCell(float *Coordinate)
{

}

int ImportMapDataFromFile(char *FileName)
{
	return 0;
}


void FreeMapData(void)
{

}

void EnemyStateMachine(GameObjInst *pInst)
{
	/***********
	This state machine has 2 states: STATE_GOING_LEFT and STATE_GOING_RIGHT
	Each state has 3 inner states: INNER_STATE_ON_ENTER, INNER_STATE_ON_UPDATE, INNER_STATE_ON_EXIT
	Use "switch" statements to determine which state and inner state the enemy is currently in.


	STATE_GOING_LEFT
		INNER_STATE_ON_ENTER
			Set velocity X to -MOVE_VELOCITY_ENEMY
			Set inner state to "on update"

		INNER_STATE_ON_UPDATE
			If collision on left side OR bottom left cell is non collidable
				Initialize the counter to ENEMY_IDLE_TIME
				Set inner state to on exit
				Set velocity X to 0


		INNER_STATE_ON_EXIT
			Decrement counter by frame time
			if counter is less than 0 (sprite's idle time is over)
				Set state to "going right"
				Set inner state to "on enter"

	STATE_GOING_RIGHT is basically the same, with few modifications.

	***********/
}