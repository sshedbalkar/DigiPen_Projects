/* Start Header -------------------------------------------------------
Copyright (C) 2011 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name		: GameState_Platformer.cpp
Purpose			: Game play implementation of the Platformer game
Language		: C++/MS VS 2010 
Platform		: Microsoft Visual Studio 2010/i7 2630QM, 8GB DDR3 1600MHz, 3GB NVIDIA GT 555M/Windows 7 Home Premium
Project			: CS529_SantoshShedbalkar_Project3_2
Author			: Santosh Shedbalkar/santosh.s/60001111
Creation date	: 10/28/2011
- End Header --------------------------------------------------------*/


#include <cassert>
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <ctime>
#include "Main.h"
#include "stdlib.h"

#define GAME_OBJ_NUM_MAX			32
#define GAME_OBJ_INST_NUM_MAX		2048
#define ENEMY_INST_MAX				64
#define COIN_INST_MAX				64
#define PARTICLES_INST_MAX			768

//Gameplay related variables and values
#define GRAVITY 10.0f
#define JUMP_VELOCITY 7.0f
#define MOVE_VELOCITY_HERO 4.0f
#define MOVE_VELOCITY_ENEMY 7.5f
#define ENEMY_IDLE_TIME 500
#define HERO_LIVES 3
// ------------------------------------------------------------------
#define COIN_COLLECT_POINTS 100
#define ENEMY_KILL_POINTS 50
#define HERO_TRAIL_LIFE 1000
#define HERO_FIRE_LIFE 500
#define EXPLOSION_LIFE 500
#define BEACON_LIFE 600
#define BEACON_INTERVAL 5000
#define MAX_LEVELS 3
#define COIN_UPDATE_INTERVAL 3000
#define LEVEL_COMPLETE_POINTS 1000
// ------------------------------------------------------------------
static int HeroLives;
static int Hero_Initial_X;
static int Hero_Initial_Y;
static int TotalCoins;

//Flags
#define FLAG_ACTIVE			0x00000001
#define FLAG_VISIBLE		0x00000002
#define FLAG_NON_COLLIDABLE	0x00000004
#define FLAG_NON_GRAVITABLE	0x00000008


enum TYPE_OBJECT
{
	TYPE_OBJECT_EMPTY,			//0
	TYPE_OBJECT_COLLISION,		//1
	TYPE_OBJECT_HERO,			//2
	TYPE_OBJECT_ENEMY1,			//3
	TYPE_OBJECT_COIN,			//4
	TYPE_OBJECT_BEACON,			//5
	TYPE_PARTICLE_0,
	TYPE_PARTICLE_1,
	TYPE_PARTICLE_2
};
enum PARTICLE_TYPE
{
	ACCEL_TRAIL = 100,
	FIRE_BALL,
	EXPLOSION,
	BEACON
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
	INNER_STATE_ON_EXIT,
	INNER_STATE_WIN,
	INNER_STATE_FAIL
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
	Vector2D		posCurr;	// object current position
	Vector2D		velCurr;	// object current velocity
	float			dirCurr;	// object current direction

	Matrix2D		transform;	// object drawing matrix

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

// Data Structure of the enemy object
struct Enemy
{
	GameObjInst* pObj;
	bool alive;
};

//Data structure of the Coin object
struct Coin
{
	GameObjInst* pObj;
	bool alive;
	int iLastUpdate;
};

// Data structure of the Beacon object which acts as the gate to the next level
struct Beacon
{
	GameObjInst* pObj;
	int iLastBeaconTime;
} beacon;

// Particle instance structure
struct Particle
{
	GameObjInst* pObj;
	int life;
	int type;
};

// list of original objects
static GameObj			*sGameObjList;
static unsigned int		sGameObjNum;

// list of object instances
static GameObjInst		*sGameObjInstList;
static unsigned int		sGameObjInstNum;
static Enemy *sEnemyList;						// List of all enemies
static Coin *sCoinList;							// List of all coins
static Particle *sParticles;					// List of all particles

//Binary map data
static int **MapData;
static int **BinaryCollisionArray;
static int BINARY_MAP_WIDTH;
static int BINARY_MAP_HEIGHT;
static GameObjInst *pBlackInstance;
static GameObjInst *pWhiteInstance;
static Matrix2D MapTransform;
int GetCellValue(int X, int Y);
int CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY);
void SnapToCell(float *Coordinate);
int ImportMapDataFromFile(char *FileName);
void FreeMapData(void);

static Vector2D Gravity;
//Collision flags
#define	COLLISION_LEFT		0x00000001	//0001
#define	COLLISION_RIGHT		0x00000002	//0010
#define	COLLISION_TOP		0x00000004	//0100
#define	COLLISION_BOTTOM	0x00000008	//1000

// function to create/destroy a game object instance
static GameObjInst*	gameObjInstCreate (unsigned int type, float scale, Vector2D* pPos, Vector2D* pVel, float dir, enum STATE startState);
static void			gameObjInstDestroy(GameObjInst* pInst);

//We need a pointer to the hero's instance for input purposes
static GameObjInst *pHero;

//State machine functions
void EnemyStateMachine(GameObjInst *pInst);
// --------------------------------------------------------------------------
bool bInputAccept;
bool bAirJump;
int iGameState;
int iScore;
int iFireTime;
static int iCurrLevel;
static int iCurrTime = 0;
void split(const std::string& Str, char delimiter, std::vector<std::string>& Result);	// Splits a Strings into tokens with a delimiter and stores the substrings into the vector
int** allocate2Dint(int count_x, int count_y);							// Allocates dynamic memory for a 2D int array
void free2Dint(int** ptr_array);										// Deallocates the previously allocated 2D int array
void PrintRetrievedInformation(void);
void respawnHero(void);
void createEnemy(float posX, float posY);
void createCoin(float posX, float posY);
void createBeacon(float posX, float posY);
void gridCollision(void);
void updateObjs(void);
void applyPhysics(void);
void updatePositions(void);
void objCollision(void);
void handleInput(void);
void updateTransformations(void);
void updateEnemyStates(void);
void levelWin(void);
void levelFail(void);
void levelStart(void);
void collectCoin(Coin *coin);
void destroyCoin(Coin *coin);
void destroyEnemy(Enemy *enemy);
float randomFloat(float, float);
Particle* particleInstCreate(int, int, int, Vector2D*, Vector2D*, float);
void particleDestroy(Particle &);
void addHeroTrail(void);
void updateparticles(void);
void addHeroFireball(void);
void killEnemy(Enemy *enemy);
void killCoin(Coin *coin);
void updateEnemy(void);
void updateCoins(void);
void updateBeacon(void);
void addExplosion(GameObjInst *obj);
void addBeacon(GameObjInst *obj);
void collectBeacon(void);
int loadMap(int level);
void loadLevel(void);
void destroyHero(void);
void destroyAllCoins(void);
void destroyAllEnemies(void);
void destroyAllParticles(void);
void destroyAllObjs(void);
void unloadLevel(void);
// --------------------------------------------------------------------------

void GameStatePlatformLoad(void)
{
	srand((unsigned)time(NULL));
	sGameObjList = (GameObj *)calloc(GAME_OBJ_NUM_MAX, sizeof(GameObj));
	sGameObjInstList = (GameObjInst *)calloc(GAME_OBJ_INST_NUM_MAX, sizeof(GameObjInst));
	sEnemyList = (Enemy *)calloc(ENEMY_INST_MAX, sizeof(Enemy));
	sCoinList = (Coin *)calloc(COIN_INST_MAX, sizeof(Coin));
	sParticles = (Particle *)calloc(PARTICLES_INST_MAX, sizeof(Particle));
	sGameObjNum = 0;

	iGameState = INNER_STATE_ON_ENTER;
	bInputAccept = false;
	iCurrLevel = 1;
	//
	GameObj* pObj;

	/////////////////////////////
	//Creating the black object
	/////////////////////////////
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
		0.5f,  0.5f, 0xFF222222);

	pObj->pMesh = AEGfxTriEnd();

	////////////////////////////
	//Creating the white object
	////////////////////////////
	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_OBJECT_COLLISION;

	AEGfxTriStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFDDDDDD, 
		 0.5f,  -0.5f, 0xFFDDDDDD, 
		-0.5f,  0.5f, 0xFFDDDDDD);
	
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFDDDDDD, 
		 0.5f,  -0.5f, 0xFFDDDDDD, 
		0.5f,  0.5f, 0xFFBBBBBB);

	pObj->pMesh = AEGfxTriEnd();

	/////////////////////////////
	//Creating the hero object
	/////////////////////////////
	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_OBJECT_HERO;

	AEGfxTriStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF0000CC, 
		 0.5f,  -0.5f, 0xFF0000CC, 
		-0.5f,  0.5f, 0xFF0000CC);
	
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFF0000CC, 
		 0.5f,  -0.5f, 0xFF0000CC, 
		0.5f,  0.5f, 0xFF5555CC);

	pObj->pMesh = AEGfxTriEnd();

	////////////////////////////////
	//Creating the enemey1 object
	////////////////////////////////
	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_OBJECT_ENEMY1;

	AEGfxTriStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFCC0000, 
		 0.5f,  -0.5f, 0xFFCC0000, 
		-0.5f,  0.5f, 0xFFCC0000);
	
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFCC0000, 
		 0.5f,  -0.5f, 0xFFCC0000, 
		0.5f,  0.5f, 0xFFCC5555);

	pObj->pMesh = AEGfxTriEnd();

	//////////////////////////////
	//Creating the Coin object
	//////////////////////////////
	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_OBJECT_COIN;

	AEGfxTriStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB

	//Creating the circle shape
	float CircleAngleStep = PI/12.0f, i;
	int Parts = 12;
	unsigned int color;
	for(i = 0; i < Parts; ++i)
	{
		color = (i < (Parts/4) || i >= (3 * Parts / 4)) ? 0xFFFF5500 : 0xFFFFFF00;
		AEGfxTriAdd(
		0.0f, 0.0f, 0xFFFFFF00, 
		cosf(i*2*PI/Parts)*0.5f,  sinf(i*2*PI/Parts)*0.5f, color, 
		cosf((i+1)*2*PI/Parts)*0.5f,  sinf((i+1)*2*PI/Parts)*0.5f, 0xFFFFFF00);
	}

	pObj->pMesh = AEGfxTriEnd();
	//
	/////////////////////////////
	//Creating the BEACON object
	/////////////////////////////
	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_OBJECT_BEACON;

	AEGfxTriStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB
	AEGfxTriAdd(-0.15f, 0.15f, 0xFF222222, 0.15f, 0.15f, 0xFF222222, 0.0f, 0.5f, 0xFFFFFFFF);
	AEGfxTriAdd(-0.15f, 0.15f, 0xFF222222, -0.5f, 0.0f, 0xFFFFFFFF, -0.15f, -0.15f, 0xFF222222);
	AEGfxTriAdd(-0.15f, -0.15f, 0xFF222222, 0.0f, -0.5f, 0xFFFFFFFF, 0.15f, -0.15f, 0xFF222222);
	AEGfxTriAdd(0.15f, -0.15f, 0xFF222222, 0.5f, 0.0f, 0xFFFFFFFF, 0.15f, 0.15f, 0xFF222222);

	pObj->pMesh = AEGfxTriEnd();
	//
	// =======================
	// create the Particle shapes
	// =======================
	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_PARTICLE_0;
	AEGfxTriStart();
	AEGfxTriAdd(-0.3f, 0.0f, 0xFFFF3300, 0.3f, -0.3f, 0xFFFF3300, 0.3f, 0.3f, 0xFFFF3300);
	pObj->pMesh = AEGfxTriEnd();
	AE_ASSERT_MESG(pObj->pMesh, "failed to create object!!");

	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_PARTICLE_1;
	AEGfxTriStart();
	AEGfxTriAdd(-0.3f, 0.0f, 0xFFFF8800, 0.3f, -0.3f, 0xFFFF8800, 0.3f, 0.3f, 0xFFFF8800);
	pObj->pMesh = AEGfxTriEnd();
	AE_ASSERT_MESG(pObj->pMesh, "failed to create object!!");

	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_PARTICLE_2;
	AEGfxTriStart();
	AEGfxTriAdd(-0.3f, 0.0f, 0xFFFFCC00, 0.3f, -0.3f, 0xFFFFCC00, 0.3f, 0.3f, 0xFFFFCC00);
	pObj->pMesh = AEGfxTriEnd();
	AE_ASSERT_MESG(pObj->pMesh, "failed to create object!!");

	//Setting intital binary map values
	MapData = 0;
	BinaryCollisionArray = 0;
	BINARY_MAP_WIDTH = 0;
	BINARY_MAP_HEIGHT = 0;
}

/*
	clears all level data
*/
void unloadLevel()
{
	destroyHero();
	destroyAllCoins();
	destroyAllEnemies();
	destroyAllParticles();
	destroyAllObjs();
	FreeMapData();
	BINARY_MAP_WIDTH = BINARY_MAP_HEIGHT = 0;
}

/*
	Loads the current level
*/
void loadLevel(void)
{
	//Importing Data
	if(!loadMap(iCurrLevel))
		gGameStateNext = GS_QUIT;
	//
	float scl;
	scl = 5.4f;
	Matrix2D scale, trans;
	Matrix2DTranslate(trans, (0.0f-BINARY_MAP_WIDTH) / 2, (0.0f-BINARY_MAP_HEIGHT) / 2 - 1.0f);
	Matrix2DScale(scale, scl, scl);
	Matrix2DIdentity(MapTransform);
	Matrix2DConcat(MapTransform, trans, MapTransform);
	Matrix2DConcat(MapTransform, scale, MapTransform);

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
	TotalCoins = 0;
	for(int i = 0; i < BINARY_MAP_WIDTH; ++i)
	{
		for(int j = 0; j < BINARY_MAP_HEIGHT; ++j)
		{
			switch(MapData[j][i])
			{
				case TYPE_OBJECT_EMPTY:
					break;
				case TYPE_OBJECT_COLLISION:
					break;
				case TYPE_OBJECT_HERO:
					Hero_Initial_X = i;
					Hero_Initial_Y = j;
					break;
				case TYPE_OBJECT_ENEMY1:
					createEnemy(((float)i) + 0.5f, ((float)j) + 0.5f);
					break;
				case TYPE_OBJECT_COIN:
					createCoin(((float)i) + 0.5f, ((float)j) + 0.5f);
					break;
				case TYPE_OBJECT_BEACON:
					createBeacon(((float)i) + 0.5f, ((float)j) + 0.5f);
					break;
			}
		}
	}
	//
	Vector2DFromAngle(Gravity, -PI/2);
	Vector2DScale(Gravity, Gravity, GRAVITY);
	levelStart();
	iGameState = INNER_STATE_ON_UPDATE;
}

/*
	Loads a new map data file
*/
int loadMap(int level)
{
	char str[25];
	sprintf_s(str, "Exported_%d.txt", level);
	return ImportMapDataFromFile(str);
}

/*
	Init state
*/
void GameStatePlatformInit(void)
{
	iCurrTime = clock() * 1000 / CLOCKS_PER_SEC;
	pHero = 0;
	pBlackInstance = 0;
	pWhiteInstance = 0;
	TotalCoins = 0;
	iScore = 0;
	beacon.iLastBeaconTime = iCurrTime;

	//Setting the inital number of hero lives
	HeroLives = HERO_LIVES;

	loadLevel();
}

/*
	creates a new beacon object
*/
void createBeacon(float posX, float posY)
{
	GameObjInst *pInst;
	Vector2D pos;
	//
	Vector2DSet(pos, posX, posY);
	pInst = gameObjInstCreate(TYPE_OBJECT_BEACON, 1.0f, &pos, 0, 0, STATE_NONE);
	pInst->flag |= FLAG_NON_GRAVITABLE;
	beacon.pObj = pInst;
	beacon.iLastBeaconTime = iCurrTime;
}

/*
	respawns hero
*/
void respawnHero(void)
{
	Vector2D pos;
	Vector2DSet(pos, ((float) Hero_Initial_X) + 0.5f, ((float) Hero_Initial_Y) + 0.5f);
	pHero = gameObjInstCreate(TYPE_OBJECT_HERO, 1.0f, &pos, 0, 0, STATE_NONE);
	iFireTime = iCurrTime + HERO_FIRE_LIFE;
}

/*
	creates a new enemy object
*/
void createEnemy(float posX, float posY)
{
	GameObjInst *pInst;
	Vector2D pos;
	//
	Vector2DSet(pos, posX, posY);
	for(int i = 0; i < ENEMY_INST_MAX; ++i)
	{
		Enemy *ene = sEnemyList + i;
		if(!(ene->pObj) || !(ene->pObj->flag))
		{
			pInst = gameObjInstCreate(TYPE_OBJECT_ENEMY1, 1.0f, &pos, 0, 0, STATE_NONE);
			AE_ASSERT(pInst);
			ene->pObj = pInst;
			ene->alive = true;
			i = ENEMY_INST_MAX;
		}
	}
}

/*
	creates a new coin object
*/
void createCoin(float posX, float posY)
{
	GameObjInst *pInst;
	Vector2D pos;
	//
	Vector2DSet(pos, posX, posY);
	for(int i = 0; i < COIN_INST_MAX; ++i)
	{
		Coin *cn = sCoinList + i;
		if(!(cn->pObj) || !(cn->pObj->flag))
		{
			pInst = gameObjInstCreate(TYPE_OBJECT_COIN, 1.0f, &pos, 0, 0, STATE_NONE);
			AE_ASSERT(pInst);
			cn->pObj = pInst;
			cn->alive = true;
			cn->iLastUpdate = iCurrTime;
			++TotalCoins;
			i = COIN_INST_MAX;
		}
	}
}

/*
	Update state
*/
void GameStatePlatformUpdate(void)
{
	if(iGameState == INNER_STATE_ON_UPDATE)
	{
		iCurrTime = clock() * 1000 / CLOCKS_PER_SEC;
		handleInput();
		updateObjs();
		gridCollision();
		objCollision();
		updateTransformations();
	}
}

/*
	updates transformations of all game obj instances
*/
void updateTransformations(void)
{
	//Computing the transformation matrices of the game object instances
	for(int i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		Matrix2D scale, rot, trans, *transform;
		GameObjInst *pInst = sGameObjInstList + i;

		// skip non-active object
		if (0 == (pInst->flag & FLAG_ACTIVE))
			continue;

		transform = &(pInst->transform);
		Matrix2DIdentity(*transform);
		Matrix2DScale(scale, pInst->scale, pInst->scale);
		Matrix2DRotRad(rot, pInst->dirCurr);
		Matrix2DTranslate(trans, pInst->posCurr.x, pInst->posCurr.y);
		//
		Matrix2DConcat(*transform, scale, *transform);
		Matrix2DConcat(*transform, rot, *transform);
		Matrix2DConcat(*transform, trans, *transform);
	}
}

/*
	Input handler
*/
void handleInput(void)
{
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
	if(bInputAccept)
	{
		if (AEInputCheckCurr(DIK_LEFT))
		{
			pHero->velCurr.x = -MOVE_VELOCITY_HERO;
		}
		else if (AEInputCheckCurr(DIK_RIGHT))
		{
			pHero->velCurr.x = MOVE_VELOCITY_HERO;
		}
		else
		{
			pHero->velCurr.x = 0.0f;
		}
		//
		if (AEInputCheckTriggered(DIK_SPACE))
		{
			if((pHero->gridCollisionFlag) & COLLISION_BOTTOM)
			{
				pHero->velCurr.y = JUMP_VELOCITY;
				addHeroTrail();
				bAirJump = false;
			}
			else if(!bAirJump)
			{
				pHero->velCurr.y = JUMP_VELOCITY;
				addHeroTrail();
				bAirJump = true;
			}
		}
		if (AEInputCheckTriggered(DIK_Q))
		{
			unloadLevel();
			gGameStateNext = GS_QUIT;
		}
	}
}

/*
	Checks for collision between different objects
*/
void objCollision(void)
{
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
	Enemy *ene;
	Coin *cn;
	GameObjInst *pInst;
	bool died = false;
	if(pHero)
	{
		float scale = pHero->scale * 0.99f;
		for(int i = ENEMY_INST_MAX - 1; i > -1; --i)
		{
			ene = sEnemyList + i;
			if(!(ene->pObj) || !((ene->pObj->flag) & FLAG_ACTIVE) ||!ene->alive)
			{
				continue;
			}
			pInst = ene->pObj;
			if(StaticRectToStaticRect(pHero->posCurr, scale, scale, pInst->posCurr, pInst->scale, pInst->scale))
			{
				if(pHero->velCurr.y < -4.0f)
				{
					addExplosion(ene->pObj);
					killEnemy(ene);
				}
				else
				{
					i = -1;
					died = true;
					levelFail();
				}
			}
		}
		if(!died)
		{
			for(int i = COIN_INST_MAX - 1; i > -1; --i)
			{
				cn = sCoinList + i;
				if(!(cn->pObj) || !((cn->pObj->flag) & FLAG_ACTIVE) || !cn->alive)
				{
					continue;
				}
				pInst = cn->pObj;
				if(StaticCircleToStaticRectangle(pInst->posCurr, pInst->scale / 2, pHero->posCurr, scale, scale))
				{
					i = -1;
					collectCoin(cn);
				}
			}
			
			if(StaticCircleToStaticRectangle(beacon.pObj->posCurr, (beacon.pObj->scale)/2, pHero->posCurr, pHero->scale, pHero->scale))
			{
				collectBeacon();
			}
		}
	}
}

/*
	destroys all enemy instances
*/
void destroyAllEnemies()
{
	Enemy *ene;
	for(int i = ENEMY_INST_MAX - 1; i > -1; --i)
	{
		ene = sEnemyList + i;
		if(!(ene->pObj) || !((ene->pObj->flag) & FLAG_ACTIVE) ||!ene->alive)
		{
			continue;
		}
		destroyEnemy(ene);
	}
}

/*
	destroys all coin instances
*/
void destroyAllCoins()
{
	Coin *ene;
	for(int i = COIN_INST_MAX - 1; i > -1; --i)
	{
		ene = sCoinList + i;
		if(!(ene->pObj) || !((ene->pObj->flag) & FLAG_ACTIVE) ||!ene->alive)
		{
			continue;
		}
		destroyCoin(ene);
	}
}

/*
	destroys all particle instances
*/
void destroyAllParticles()
{
	Particle *part;
	float step = PI/18.0f;
	for(unsigned int i = 0; i < PARTICLES_INST_MAX; ++i)
	{
		part = sParticles + i;
		if(!(part->pObj) || ((part->pObj->flag & FLAG_ACTIVE) == 0))
		{
			continue;
		}
		particleDestroy(*part);
	}
}

/*
	destroys all objs
*/
void destroyAllObjs()
{
	for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;
		if (pInst->flag == 0)
		{
			continue;
		}
		gameObjInstDestroy(pInst);
	}
}

/*
	handles beacon collect event
*/
void collectBeacon()
{
	levelWin();
}

/*
	updates positions of all game objs
*/
void updatePositions(void)
{
	//Update object instances positions
	for(int i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		GameObjInst *pInst = sGameObjInstList + i;

		// skip non-active object
		if (0 == (pInst->flag & FLAG_ACTIVE))
			continue;

		/**********
		update the position using: P1 = V1*t + P0
		**********/
		Vector2DScaleAdd(pInst->posCurr, pInst->velCurr, pInst->posCurr, (float) gAEFrameTime);
	}
}

/*
	updates all game obj instances
*/
void updateObjs(void)
{
	applyPhysics();
	updateEnemy();
	updateCoins();
	updateBeacon();
	addHeroFireball();
	updatePositions();
	updateparticles();
}

/*
	updates all enemy instances
*/
void updateEnemy()
{
	GameObjInst *pInst;
	Enemy *ene;
	//
	updateEnemyStates();
	//
	for(int i = ENEMY_INST_MAX - 1; i > -1; --i)
	{
		ene = sEnemyList + i;
		if(!(ene->pObj) || !((ene->pObj->flag) & FLAG_ACTIVE))
		{
			continue;
		}
		pInst = ene->pObj;
		if(!ene->alive)
		{
			Vector2DZero(pInst->velCurr);
			pInst->scale *= 0.9f;
			if(pInst->scale < 0.1f)
			{
				destroyEnemy(ene);
			}
		}
	}
}

/*
	updates all coin instances
*/
void updateCoins()
{
	GameObjInst *pInst;
	Coin *cn;
	//
	for(int i = COIN_INST_MAX - 1; i > -1; --i)
	{
		cn = sCoinList + i;
		if(!(cn->pObj) || !((cn->pObj->flag) & FLAG_ACTIVE))
		{
			continue;
		}
		pInst = cn->pObj;
		if(!cn->alive)
		{
			Vector2DZero(pInst->velCurr);
			pInst->scale *= 0.8f;
			if(pInst->scale < 0.1f)
			{
				destroyCoin(cn);
			}
		}
		else
		{
			if((iCurrTime - cn->iLastUpdate) > COIN_UPDATE_INTERVAL)
			{
				cn->iLastUpdate = iCurrTime;
				if(randomFloat(0.0f, 1.0f) < 0.2f)
				{
					pInst->scale *= -1;
				}
			}
		}
	}
}

/*
	updates enemy state machines
*/
void updateEnemyStates(void)
{
	GameObjInst *pInst;
	Enemy *ene;
	int coll1, coll2;
	Vector2D vTemp;
	float ang = (10.5f * PI / 180.0f);
	//
	for(int i = ENEMY_INST_MAX - 1; i > -1; --i)
	{
		ene = sEnemyList + i;
		if(!(ene->pObj) || !((ene->pObj->flag) & FLAG_ACTIVE) || !(ene->alive))
		{
			continue;
		}
		pInst = ene->pObj;
		switch(pInst->state)
		{
			case STATE_NONE:
				switch(pInst->innerState)
				{
					case INNER_STATE_ON_ENTER:
						pInst->counter = iCurrTime;
						pInst->innerState = INNER_STATE_ON_UPDATE;
						break;
					case INNER_STATE_ON_UPDATE:
						if(iCurrTime - pInst->counter > ENEMY_IDLE_TIME)
						{
							pInst->innerState = INNER_STATE_ON_EXIT;
						}
						break;
					case INNER_STATE_ON_EXIT:
						pInst->state = STATE_GOING_LEFT;
						pInst->innerState = INNER_STATE_ON_ENTER;
						break;
				}
				break;
			case STATE_GOING_LEFT:
				switch(pInst->innerState)
				{
					case INNER_STATE_ON_ENTER:
						pInst->velCurr.x = -MOVE_VELOCITY_ENEMY;
						pInst->innerState = INNER_STATE_ON_UPDATE;
						break;
					case INNER_STATE_ON_UPDATE:
						pInst->dirCurr += ang;
						Vector2DScaleAdd(vTemp, pInst->velCurr, pInst->posCurr, (float) gAEFrameTime);
						coll1 = pInst->gridCollisionFlag;
						coll2 = CheckInstanceBinaryMapCollision((vTemp.x) - 1.0f, vTemp.y, pInst->scale, pInst->scale);
						if((coll1 & COLLISION_LEFT) || !(coll2 & COLLISION_BOTTOM))
						{
							pInst->velCurr.x = 0.0f;
							pInst->counter = iCurrTime;
							pInst->innerState = INNER_STATE_ON_EXIT;
						}
						break;
					case INNER_STATE_ON_EXIT:
						if((iCurrTime - pInst->counter) > ENEMY_IDLE_TIME)
						{
							pInst->innerState = INNER_STATE_ON_ENTER;
							pInst->state = STATE_GOING_RIGHT;
						}
						break;
				}
				break;
			case STATE_GOING_RIGHT:
				switch(pInst->innerState)
				{
					case INNER_STATE_ON_ENTER:
						pInst->velCurr.x = MOVE_VELOCITY_ENEMY;
						pInst->innerState = INNER_STATE_ON_UPDATE;
						break;
					case INNER_STATE_ON_UPDATE:
						pInst->dirCurr -= ang;
						Vector2DScaleAdd(vTemp, pInst->velCurr, pInst->posCurr, (float) gAEFrameTime);
						coll1 = pInst->gridCollisionFlag;
						coll2 = CheckInstanceBinaryMapCollision((vTemp.x) + 1.0f, vTemp.y, pInst->scale, pInst->scale);
						if((coll1 & COLLISION_RIGHT) || !(coll2 & COLLISION_BOTTOM))
						{
							pInst->velCurr.x = 0.0f;
							pInst->counter = iCurrTime;
							pInst->innerState = INNER_STATE_ON_EXIT;
						}
						break;
					case INNER_STATE_ON_EXIT:
						if((iCurrTime - pInst->counter) > ENEMY_IDLE_TIME)
						{
							pInst->innerState = INNER_STATE_ON_ENTER;
							pInst->state = STATE_GOING_LEFT;
						}
						break;
				}
				break;
		}
	}
}

/*
	applies physics to all game obj instances
*/
void applyPhysics(void)
{
	//Update object instances physics and behavior
	for(int i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		GameObjInst *pInst = sGameObjInstList + i;

		// skip non-active object
		if (0 == (pInst->flag & FLAG_ACTIVE) || (pInst->flag & FLAG_NON_GRAVITABLE))
			continue;

		/****************
		Apply gravity
			Velcoty Y = Gravity * Frame Time + Velocity Y

		If object instance is an enemy
			Apply enemy state machine
		****************/
		Vector2DScaleAdd(pInst->velCurr, Gravity, pInst->velCurr, (float) gAEFrameTime);
	}
}

/*
	Checks all game obj instances for grid collision
*/
void gridCollision(void)
{
	int cFlag;
	//Check for grid collision
	for(int i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
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
		if(!(pInst->flag & FLAG_NON_COLLIDABLE))
		{
			pInst->gridCollisionFlag = cFlag = CheckInstanceBinaryMapCollision(pInst->posCurr.x, pInst->posCurr.y, 1, 1);
			if((cFlag & COLLISION_LEFT) > 0)
			{
				SnapToCell(&pInst->posCurr.x);
				pInst->velCurr.x = 0.0f;
			}
			if((cFlag & COLLISION_RIGHT) > 0)
			{
				SnapToCell(&pInst->posCurr.x);
				pInst->velCurr.x = 0.0f;
			}
			if((cFlag & COLLISION_TOP) > 0)
			{
				SnapToCell(&pInst->posCurr.y);
				pInst->velCurr.y = 0.0f;
			}
			if((cFlag & COLLISION_BOTTOM) > 0)
			{
				SnapToCell(&pInst->posCurr.y);
				pInst->velCurr.y = 0.0f;
			}
		}
	}
}

/*
	Draw function
*/
void GameStatePlatformDraw(void)
{
	//Drawing the tile map (the grid)
	int i, j;
	Matrix2D cellTranslation, cellFinalTransformation;

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
	{
		for(j = 0; j < BINARY_MAP_HEIGHT; ++j)
		{
			switch(BinaryCollisionArray[j][i])
			{
				case TYPE_OBJECT_EMPTY:
					Matrix2DTranslate(cellTranslation, i + 0.5f, j + 0.5f);
					Matrix2DConcat(cellFinalTransformation, MapTransform, cellTranslation);
					//
					AEGfxSetTransform((AEMtx33*)&cellFinalTransformation);
					AEGfxTriDraw(pBlackInstance->pObject->pMesh);
					break;
				case TYPE_OBJECT_COLLISION:
					Matrix2DTranslate(cellTranslation, i + 0.5f, j + 0.5f);
					Matrix2DConcat(cellFinalTransformation, MapTransform, cellTranslation);
					//
					AEGfxSetTransform((AEMtx33*)&cellFinalTransformation);
					AEGfxTriDraw(pWhiteInstance->pObject->pMesh);
					break;
			}
		}
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

		Matrix2DConcat(cellFinalTransformation, MapTransform, pInst->transform);
		AEGfxSetTransform((AEMtx33*)&cellFinalTransformation);
		AEGfxTriDraw(pInst->pObject->pMesh);
		//
		//Don't forget to concatenate the MapTransform matrix with the transformation of each game object instance
	}

	char strBuffer[100];
	memset(strBuffer, 0, 100*sizeof(char));
	sprintf_s(strBuffer, "Frame Time:  %.6f", gAEFrameTime);
	AEGfxPrint(40, 20, -1, strBuffer);
	memset(strBuffer, 0, 100*sizeof(char));
	sprintf_s(strBuffer, "Lives:  %i", HeroLives);
	AEGfxPrint(650, 20, -1, strBuffer);	
	sprintf_s(strBuffer, "Score:  %i", iScore);
	AEGfxPrint(650, 50, -1, strBuffer);
	sprintf_s(strBuffer, "Level:  %i", iCurrLevel);
	AEGfxPrint(40, 50, -1, strBuffer);
	//
	if (iGameState == INNER_STATE_FAIL)
	{
		AEGfxPrint(280, 260, 0xFFFF4444, "       GAME OVER       ");
	}
	else if(iGameState == INNER_STATE_WIN)
	{
		AEGfxPrint(280, 260, 0xFF4444FF, "      YOU WIN!       ");
	}
}

/*
	Frees all game obj instances
*/
void GameStatePlatformFree(void)
{
	// kill all object in the list
	for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
		gameObjInstDestroy(sGameObjInstList + i);
}

/*
	Platform game unloader
*/
void GameStatePlatformUnload(void)
{
	// free all CREATED mesh
	for (u32 i = 0; i < sGameObjNum; i++)
		AEGfxTriFree(sGameObjList[i].pMesh);

	/*********
	Free the map data
	*********/
}

/*
	creates a new game obj instance
*/
GameObjInst* gameObjInstCreate(unsigned int type, float scale, Vector2D* pPos, Vector2D* pVel, float dir, enum STATE startState)
{
	Vector2D zero = { 0.0f, 0.0f };

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
			
			return pInst;
		}
	}
	return NULL;
}

/*
	destroys a game obj instance
*/
void gameObjInstDestroy(GameObjInst* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->flag == 0)
		return;

	// zero out the flag
	pInst->flag = 0;
}

/*
	GetCellValue	- function that returns the collision value at a given map location
	X				- X coord value of the position
	Y				- Y coord value of the position

	return			- Collision value at the given position if its exists, 0 if the position doesnt exist
*/
int GetCellValue(int X, int Y)
{
	if(X > -1 && X < BINARY_MAP_WIDTH && Y > -1 && Y < BINARY_MAP_HEIGHT)
	{
		return BinaryCollisionArray[Y][X];
	}
	return 0;
}


/*
	CheckInstanceBinaryMapCollision		- Figuures out which sides are colliding of an object represented by its position and scale value
	PosX								- X position of the object
	PosY								- Y position of the object
	scaleX								- X scale value of the object
	scaleY								- Y scale value of the object

	return								- An integer value whose bits represent which side the object is colliding, if it is colliding.
*/
int CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY)
{
	int top1, top2, bot1, bot2, lft1, lft2, rgt1, rgt2;
	int flag = 0;
	//
	top1 = (int) floor(PosY + (scaleY / 2.0f));
	top2 = (int) floor(PosY + (scaleY / 4.0f));
	//
	bot1 = (int) floor(PosY - (scaleY / 2.0f));
	bot2 = (int) floor(PosY - (scaleY / 4.0f));
	//
	lft1 = (int) floor(PosX - (scaleX / 2.0f));
	lft2 = (int) floor(PosX - (scaleX / 4.0f));
	//
	rgt1 = (int) floor(PosX + (scaleX / 2.0f));
	rgt2 = (int) floor(PosX + (scaleX / 4.0f));
	//
	if(GetCellValue(lft2, top1) || GetCellValue(rgt2, top1))
	{
		flag |= COLLISION_TOP;
	}
	if(GetCellValue(lft2, bot1) || GetCellValue(rgt2, bot1))
	{
		flag |= COLLISION_BOTTOM;
	}
	if(GetCellValue(lft1, top2) || GetCellValue(lft1, bot2))
	{
		flag |=  COLLISION_LEFT;
	}
	if(GetCellValue(rgt1, top2) || GetCellValue(rgt1, bot2))
	{
		flag |= COLLISION_RIGHT;
	}

	return flag;
}


/*
	SnapToCell		- function which snaps an object which is slightly into the area of a colliding object back to a valid position
	Coordinate		- Pointer to the current X or Y position coordinate of the object

	return			- modifies the pointer to point to the correct value
*/
void SnapToCell(float *Coordinate)
{
	*Coordinate = ((float) ((int) (*Coordinate))) + 0.5f;
}

/*
	ImportMapDataFromFile		- imports map data and collision data from an external file and allocates dynamic memory for map data array and binary collision map array
	Filename					- pointer to a string of the local file name which has the map data

	return						- 1 if the operation was successful, 0 otherwise
*/
int ImportMapDataFromFile(char *FileName)
{
	int returnCode = 1;
	FILE *file;
	char line[100];

	if (!(file = fopen(FileName, "r" )))
	{
		printf("[ERROR] BinaryMap.cpp::ImportMapDataFromFile : File \"%s\" could not be opened for reading\n", FileName);
		returnCode = 0;
	} 
	else 
	{
		char *ret;
		int row, col;
		do
		{
			if((ret = fgets(line, 100, file)) == NULL)
			{
				if(ferror(file))
				{
					printf("[ERROR] BinaryMap.cpp::ImportMapDataFromFile: fgets error\n");
					returnCode = 0;
				}
			}
			else
			{
				std::vector<std::string> vec;
				split(line, ' ', vec);
				if(!strcmp(vec[0].c_str(), "Width"))
				{
					BINARY_MAP_WIDTH = atoi(vec[1].c_str());
				}
				else if(!strcmp(vec[0].c_str(), "Height"))
				{
					BINARY_MAP_HEIGHT = atoi(vec[1].c_str());
				}
				//
				if(BINARY_MAP_WIDTH && BINARY_MAP_HEIGHT)
				{
					if(!MapData && !BinaryCollisionArray)
					{
						MapData = allocate2Dint(BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
						if(MapData)
						{
							BinaryCollisionArray = allocate2Dint(BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
							if(BinaryCollisionArray)
							{
								row = BINARY_MAP_HEIGHT;
								col = BINARY_MAP_WIDTH;
							}
						}
					}
					else
					{
						int *p_row1 = *(BinaryCollisionArray + --row);
						int *p_row2 = *(MapData + row);
						int val;
						for(unsigned int i = 0; i < vec.size() && i < BINARY_MAP_WIDTH; ++i)
						{
							val = atoi(vec[i].c_str());
							*(p_row1 + i) = (val > TYPE_OBJECT_COLLISION) ? TYPE_OBJECT_EMPTY : val;
							*(p_row2 + i) = val;
						}
					}
				}
			}
		} while(ret != NULL);
		fclose(file);
	}
	//PrintRetrievedInformation();
	return returnCode;
}

/*
	split		- Splits a token String delimited by a character and appends the split strings into the string vector
	str			- reference to the delimited token string
	delimiter	- the character delimiter
	Result		- the string vector where the split strings will be appended
*/
void split(const std::string& Str, char delimiter, std::vector<std::string>& Result)
{
	using namespace std;
	string::size_type i = 0;
	string::size_type j = Str.find(delimiter);
	while (j != string::npos)
	{
		Result.push_back(Str.substr(i, j-i));
		i = ++j;
		j = Str.find(delimiter, j);
		if (j == string::npos)
		{
			Result.push_back(Str.substr(i, Str.length( )));
		}
	}
}

/*
	FreeMapData		- Frees map data array and binary collision array
*/
void FreeMapData(void)
{
	free2Dint(MapData);
	free2Dint(BinaryCollisionArray);
	MapData = 0;
	BinaryCollisionArray = 0;
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
/*
	allocate2Dint		- Allocates dynamic memory for a 2D int array
	count_x				- width of the array
	count_y				- height of the array

	return				- double int pointer to the allocated memory
*/
int** allocate2Dint(int count_x, int count_y)
{
	using namespace std;
	int *data = (int*) malloc(sizeof(int) * count_x * count_y);
	if(!data)
	{
		cout << "[ERROR] BinaryMap:allocate2Dint: Error allocating memory\n" << endl;
		return 0;
	}
	int **ptr_array = (int**) malloc(sizeof(int*) * count_x);
	if(!ptr_array)
	{
		cout << "[ERROR] BinaryMap:allocate2Dint: Error allocating memory\n" << endl;
		return 0;
	}
	for (int i = 0; i < count_x; ++i)
	{
		ptr_array[i] = data + (i*count_y);
	}
	return ptr_array;
}

/*
	free2Dint		- Deallocates a previously allocated 2D dynamic int array
	ptr_array		- pointer to the previously allocated dynamic 2D array
*/
void free2Dint(int** ptr_array)
{
	if(!ptr_array)
	{
		return;
	}
	if(ptr_array[0])
	{
		free(ptr_array[0]);
		ptr_array[0] = 0;
	}
	free(ptr_array);
	ptr_array = 0;
}

void PrintRetrievedInformation(void)
{
	int i, j;

	printf("Width: %i\n", BINARY_MAP_WIDTH);
	printf("Height: %i\n", BINARY_MAP_HEIGHT);

	printf("Map Data:\n");
	//for(j = 0 ; j < BINARY_MAP_HEIGHT; ++j)
	for(j = BINARY_MAP_HEIGHT - 1; j > -1; --j)
	{
		for(i = 0; i < BINARY_MAP_WIDTH; ++i)
		{
			//printf("%i ", MapData[i][j]);
			printf("%i ", MapData[j][i]);
		}

		printf("\n");
	}

	printf("\n\nBinary Collision Data:\n");
	//for(j = 0 ; j < BINARY_MAP_HEIGHT; ++j)
	for(j = BINARY_MAP_HEIGHT - 1; j > -1; --j)
	{
		for(i = 0; i < BINARY_MAP_WIDTH; ++i)
		{
			//printf("%i ", BinaryCollisionArray[i][j]);
			printf("%i ", BinaryCollisionArray[j][i]);
		}

		printf("\n");
	}
}

/*
	Handles level complete event
*/
void levelWin(void)
{
	if((1+iCurrLevel) <= MAX_LEVELS)
	{
		++iCurrLevel;
		iScore += LEVEL_COMPLETE_POINTS;
		unloadLevel();
		loadLevel();
	}
	else
	{
		iGameState = INNER_STATE_WIN;
	}
}

/*
	destroys hero instance
*/
void destroyHero()
{
	if(pHero)
	{
		gameObjInstDestroy(pHero);
		pHero = 0;
	}
}

/*
	handles hero death
*/
void levelFail(void)
{
	bInputAccept = false;
	addExplosion(pHero);
	destroyHero();
	if(--HeroLives > -1)
	{
		levelStart();
	}
	else
	{
		HeroLives = 0;
		iGameState = INNER_STATE_FAIL;
	}
}

/*
	starts the current level
*/
void levelStart(void)
{
	respawnHero();
	bInputAccept = true;
}

/*
	handles coin collect event
*/
void collectCoin(Coin *coin)
{
	assert(coin && coin->pObj && ((coin->pObj->flag) & FLAG_ACTIVE));
	iScore += COIN_COLLECT_POINTS;
	killCoin(coin);
}

/*
	destroys a coin instance
*/
void destroyCoin(Coin *coin)
{
	gameObjInstDestroy(coin->pObj);
	coin->pObj = 0;
	--TotalCoins;
}

/*
	destroys an enemy instance
*/
void destroyEnemy(Enemy *enemy)
{
	iScore += ENEMY_KILL_POINTS;
	gameObjInstDestroy(enemy->pObj);
	enemy->pObj = 0;
}

// generates a random float number in a range
float randomFloat(float min, float max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return min + r * (max - min);
}

// creates a particle instance
Particle* particleInstCreate(int type1, int type2, int life, Vector2D* pPos, Vector2D* pVel, float dir)
{
	Particle *part = 0;
	//
	for(int i = PARTICLES_INST_MAX - 1; i > -1; --i)
	{
		part = sParticles + i;
		if(part->pObj == 0 || !((part->pObj->flag) & FLAG_ACTIVE))
		{
			part->pObj = gameObjInstCreate(type1, 0.2f, pPos, pVel, dir, STATE_NONE);
			part->pObj->flag |= FLAG_NON_COLLIDABLE | FLAG_NON_GRAVITABLE;
			AE_ASSERT(part->pObj);
			part->life = life;
			part->type = type2;
			i = -1;
		}
	}
	return part;
}

// destroys a particle object
void particleDestroy(Particle &part)
{
	gameObjInstDestroy(part.pObj);
	part.pObj = 0;
}

// adds hero trail particle effect
void addHeroTrail(void)
{
	float dir = randomFloat(-PI, PI), xt;
	Matrix2D mat1, mat2, mrot, mtrans, mscale;
	int life;
	//
	Matrix2DIdentity(mat1);
	Matrix2DIdentity(mat2);
	Matrix2DRotRad(mrot, pHero->dirCurr);
	Matrix2DTranslate(mtrans, pHero->posCurr.x, pHero->posCurr.y);
	Matrix2DScale(mscale, pHero->scale, pHero->scale);

	Matrix2DConcat(mat1, mscale, mat1);
	Matrix2DConcat(mat1, mrot, mat1);
	Matrix2DConcat(mat1, mtrans, mat1);

	Matrix2DConcat(mat2, mscale, mat2);
	Matrix2DConcat(mat2, mrot, mat2);
	//
	for(int i = (int)randomFloat(20, 30); i > -1; --i)
	{
		Vector2D pos, vel;
		//
		pos.y = -0.5;
		pos.x = randomFloat(-0.15f, 0.15f);
		xt = abs(pos.x);
		//
		vel.y = randomFloat(-2.0f, -4.0f);
		vel.x = randomFloat(-3.0f, 3.0f);
		//
		Matrix2DMultVec(pos, mat1, pos);
		Matrix2DMultVec(vel, mat2, vel);
		//
		life = iCurrTime + (int)randomFloat(HERO_TRAIL_LIFE * 0.85f, HERO_TRAIL_LIFE * 1.15f);
		//
		if(xt < 0.07)
		{
			particleInstCreate(TYPE_PARTICLE_0, ACCEL_TRAIL, life, &pos, &vel, dir);
		}
		else if(xt < 0.11)
		{
			particleInstCreate(TYPE_PARTICLE_1, ACCEL_TRAIL, life, &pos, &vel, dir);
		}
		else
		{
			particleInstCreate(TYPE_PARTICLE_2, ACCEL_TRAIL, life, &pos, &vel, dir);
		}
	}
}

/*
	Adds fireball effect around the hero obj
*/
void addHeroFireball(void)
{
	if(!pHero || iFireTime < iCurrTime)
	{
		return;
	}
	float dir = randomFloat(-PI, PI);
	Matrix2D mat1, mat2, mrot, mtrans, mscale;
	int life;
	//
	Matrix2DIdentity(mat1);
	Matrix2DIdentity(mat2);
	Matrix2DRotRad(mrot, pHero->dirCurr);
	Matrix2DTranslate(mtrans, pHero->posCurr.x, pHero->posCurr.y);
	Matrix2DScale(mscale, pHero->scale, pHero->scale);

	Matrix2DConcat(mat1, mscale, mat1);
	Matrix2DConcat(mat1, mrot, mat1);
	Matrix2DConcat(mat1, mtrans, mat1);

	Matrix2DConcat(mat2, mscale, mat2);
	Matrix2DConcat(mat2, mrot, mat2);
	//
	for(int i = 25; i > -1; --i)
	{
		Vector2D pos, vel;
		//
		if(randomFloat(0.0f, 1.0f) < 0.5f)
		{
			pos.x = randomFloat(-0.5f, 0.5f);
			pos.y = (randomFloat(0.0f, 1.0f) < 0.5f) ? 0.5f : - 0.5f;
		}
		else
		{
			pos.y = randomFloat(-0.5f, 0.5f);
			pos.x = (randomFloat(0.0f, 1.0f) < 0.5f) ? 0.5f : -0.5f;
		}
		//
		vel.y = randomFloat(1.0f, 3.0f);
		vel.x = 0.0f;
		//
		Matrix2DMultVec(pos, mat1, pos);
		Matrix2DMultVec(vel, mat2, vel);
		//
		life = iCurrTime + (int)randomFloat(HERO_FIRE_LIFE * 0.85f, HERO_FIRE_LIFE * 1.15f);
		//
		particleInstCreate(TYPE_PARTICLE_2, FIRE_BALL, life, &pos, &vel, dir);
	}
}

/*
	adds explosion particle effect
*/
void addExplosion(GameObjInst *obj)
{
	if(!obj || !(obj->flag & FLAG_ACTIVE))
	{
		return;
	}
	float dir = randomFloat(-PI, PI), rand;
	Matrix2D mat1, mat2, mrot, mtrans, mscale;
	int life;
	//
	Matrix2DIdentity(mat1);
	Matrix2DIdentity(mat2);
	Matrix2DRotRad(mrot, obj->dirCurr);
	Matrix2DTranslate(mtrans, obj->posCurr.x, obj->posCurr.y);
	Matrix2DScale(mscale, obj->scale, obj->scale);

	Matrix2DConcat(mat1, mscale, mat1);
	Matrix2DConcat(mat1, mrot, mat1);
	Matrix2DConcat(mat1, mtrans, mat1);

	Matrix2DConcat(mat2, mscale, mat2);
	Matrix2DConcat(mat2, mrot, mat2);
	//
	for(int i = 50; i > -1; --i)
	{
		Vector2D pos, vel;
		//
		Vector2DZero(pos);
		//
		vel.y = randomFloat(-3.0f, 3.0f);
		vel.x = randomFloat(-3.0f, 3.0f);
		//
		Matrix2DMultVec(pos, mat1, pos);
		Matrix2DMultVec(vel, mat2, vel);
		//
		life = iCurrTime + (int)randomFloat(EXPLOSION_LIFE * 0.75f, EXPLOSION_LIFE * 1.25f);
		//
		rand = randomFloat(0.0f, 1.0f);
		if(rand < 0.33f)
		{
			particleInstCreate(TYPE_PARTICLE_2, EXPLOSION, life, &pos, &vel, dir);
		}
		else if(rand < 0.66f)
		{
			particleInstCreate(TYPE_PARTICLE_1, EXPLOSION, life, &pos, &vel, dir);
		}
		else
		{
			particleInstCreate(TYPE_PARTICLE_0, EXPLOSION, life, &pos, &vel, dir);
		}
	}
}

// updates all particle effects
void updateparticles()
{
	Particle *part;
	float step = PI/18.0f;
	int diff;
	for(unsigned int i = 0; i < PARTICLES_INST_MAX; ++i)
	{
		part = sParticles + i;
		if(!(part->pObj) || ((part->pObj->flag & FLAG_ACTIVE) == 0))
		{
			continue;
		}
		float scale = part->pObj->scale;
		Vector2D *pos = &(part->pObj->posCurr);
		if(pos->x < (gAEWinMinX - scale) || pos->x > (gAEWinMaxX + scale) || 
			pos->y < (gAEWinMinY - scale) || pos->y > (gAEWinMaxY + scale))
		{
			particleDestroy(*part);
		}
		else
		{
			switch(part->type)
			{
				case ACCEL_TRAIL:
					if(part->life - iCurrTime < 0)
					{
						particleDestroy(*part);
					}
					else
					{
						part->pObj->dirCurr += step;
					}
					break;
				case FIRE_BALL:
					if((diff = (part->life) - iCurrTime) < 0)
					{
						particleDestroy(*part);
					}
					else
					{
						if(diff < HERO_FIRE_LIFE * 0.2f)
						{
							part->pObj->scale *= 0.9f;
							
						}
						if(diff < HERO_FIRE_LIFE * 0.4f)
						{
							part->pObj->pObject = sGameObjList + TYPE_PARTICLE_0;
						}
						else if(diff < HERO_FIRE_LIFE * 0.8f)
						{
							part->pObj->pObject = sGameObjList + TYPE_PARTICLE_1;
						}
						part->pObj->posCurr.x += (randomFloat(0.0f, 1.0f) < 0.5f) ? 0.03f : -0.03f;
					}
					break;
				case EXPLOSION:
					if(part->life - iCurrTime < 0)
					{
						particleDestroy(*part);
					}
					else
					{
						part->pObj->dirCurr += 3 * step;
					}
					break;
				case BEACON:
					if((diff = (part->life) - iCurrTime) < 0)
					{
						particleDestroy(*part);
					}
					else
					{
						if(diff < HERO_FIRE_LIFE * 0.2f)
						{
							part->pObj->scale *= 0.9f;
							
						}
						if(pHero)
						{
							if(StaticPointToStaticRect(part->pObj->posCurr, pHero->posCurr, pHero->scale, pHero->scale))
							{
								levelFail();
							}
						}
					}
					break;
			}
		}
		
	}
}

/*
	readies an enemy instance to be destroyed
*/
void killEnemy(Enemy *enemy)
{
	Vector2DZero(enemy->pObj->velCurr);
	enemy->alive = false;
}

/*
	readies a coin instance to be destroyes
*/
void killCoin(Coin *coin)
{
	coin->alive = false;
}

/*
	updates beacon
*/
void updateBeacon(void)
{
	beacon.pObj->dirCurr += 7.0f * PI / 180.0f;
	if(iCurrTime - beacon.iLastBeaconTime > BEACON_INTERVAL)
	{
		beacon.iLastBeaconTime = iCurrTime;
		addBeacon(beacon.pObj);
	}
}

/*
	adds beacon obj
*/
void addBeacon(GameObjInst *obj)
{
	if(!obj || !(obj->flag & FLAG_ACTIVE))
	{
		return;
	}
	float dir = randomFloat(-PI, PI);
	Matrix2D mat1, mat2, mrot, mtrans, mscale;
	int life, count = 20;
	float step = 2 * PI / count, ang = -PI;
	//
	while(ang < PI)
	{
		Vector2D pos, vel;
		//
		Vector2DZero(pos);
		//
		vel.y = 3.0f;
		vel.x = 3.0f;
		//
		Matrix2DIdentity(mat1);
		Matrix2DIdentity(mat2);
		Matrix2DRotRad(mrot, ang);
		Matrix2DTranslate(mtrans, obj->posCurr.x, obj->posCurr.y);
		Matrix2DScale(mscale, obj->scale, obj->scale);

		Matrix2DConcat(mat1, mscale, mat1);
		Matrix2DConcat(mat1, mrot, mat1);
		Matrix2DConcat(mat1, mtrans, mat1);

		Matrix2DConcat(mat2, mscale, mat2);
		Matrix2DConcat(mat2, mrot, mat2);
	//
		Matrix2DMultVec(pos, mat1, pos);
		Matrix2DMultVec(vel, mat2, vel);
		//
		life = iCurrTime + BEACON_LIFE;
		//
		particleInstCreate(TYPE_PARTICLE_2, BEACON, life, &pos, &vel, dir);
		//
		ang += step;
	}
}