// --------------------------------------------------------------------------------
// Project Name		:	Platformer - Part 1
// File Name		:	BinaryMap.cpp
// Author			:	Antoine Abi Chakra
// Creation Date	:	2008/03/05
// Purpose			:	Implementation of the binary map functions
// History			:
//	- 
//© Copyright 1996-2010, DigiPen Institute of Technology (USA). All rights reserved.
// ---------------------------------------------------------------------------------


#include "BinaryMap.h"



/*The number of horizontal elements*/
static int BINARY_MAP_WIDTH;

/*The number of vertical elements*/
static int BINARY_MAP_HEIGHT;

/*This will contain all the data of the map, which will be retireved from a file
when the "ImportMapDataFromFile" function is called*/
static int **MapData;

/*This will contain the collision dataof the binary map. It will be filled in the 
"ImportMapDataFromFile" after filling "MapData". Basically, if an array element 
in MapData is 1, it represents a collision cell, any other value is a non-collision
cell*/
static int **BinaryCollisionArray;



int GetCellValue(int X, int Y)
{
	return 0;
}


int CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY)
{
	return 0
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

void PrintRetrievedInformation(void)
{
	int i, j;

	printf("Width: %i\n", BINARY_MAP_WIDTH);
	printf("Height: %i\n", BINARY_MAP_HEIGHT);

	printf("Map Data:\n");
	for(j = BINARY_MAP_HEIGHT - 1; j >= 0; --j)
	{
		for(i = 0; i < BINARY_MAP_WIDTH; ++i)
		{
			printf("%i ", MapData[i][j]);
		}

		printf("\n");
	}

	printf("\n\nBinary Collision Data:\n");
	for(j = BINARY_MAP_HEIGHT - 1; j >= 0; --j)
	{
		for(i = 0; i < BINARY_MAP_WIDTH; ++i)
		{
			printf("%i ", BinaryCollisionArray[i][j]);
		}

		printf("\n");
	}
}