/* Start Header -------------------------------------------------------
Copyright (C) 2011 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name		: BinaryMap.cpp
Purpose			: Implementation of binary collision map
Language		: C++/MS VS 2010 
Platform		: Microsoft Visual Studio 2010/i7 2630QM, 8GB DDR3 1600MHz, 3GB NVIDIA GT 555M/Windows 7 Home Premium
Project			: CS529_SantoshShedbalkar_Project4_1
Author			: Santosh Shedbalkar/santosh.s/60001111
Creation date	: 10/19/2011
- End Header --------------------------------------------------------*/


#include "BinaryMap.h"
#include <string.h>
#include <vector>
#include <functional>
#include <iostream>

/*The number of horizontal elements*/
static int BINARY_MAP_WIDTH;

/*The number of vertical elements*/
static int BINARY_MAP_HEIGHT;

/*This will contain all the data of the map, which will be retireved from a file
when the "ImportMapDataFromFile" function is called*/
static int **MapData = 0;

/*This will contain the collision dataof the binary map. It will be filled in the 
"ImportMapDataFromFile" after filling "MapData". Basically, if an array element 
in MapData is 1, it represents a collision cell, any other value is a non-collision
cell*/
static int **BinaryCollisionArray = 0;

using namespace std;

// --------------------------------------------------------------------------
void split(const string& Str, char delimiter, vector<string>& Result);	// Splits a Strings into tokens with a delimiter and stores the substrings into the vector
int** allocate2Dint(int count_x, int count_y);							// Allocates dynamic memory for a 2D int array
void free2Dint(int** ptr_array);										// Deallocates the previously allocated 2D int array
// --------------------------------------------------------------------------

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
	top1 = floor(PosY + (scaleY / 2.0f));
	top2 = floor(PosY + (scaleY / 4.0f));

	bot1 = floor(PosY - (scaleY / 2.0f));
	bot2 = floor(PosY - (scaleY / 4.0f));

	lft1 = floor(PosX - (scaleX / 2.0f));
	lft2 = floor(PosX - (scaleX / 4.0f));

	rgt1 = floor(PosX + (scaleX / 2.0f));
	rgt2 = floor(PosX + (scaleX / 4.0f));
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
	*Coordinate = (float) ((int) (*Coordinate)) + 0.5f;
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
				vector<string> vec;
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
						for(int i = 0; i < vec.size(); ++i)
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
	return returnCode;
}

/*
	split		- Splits a token String delimited by a character and appends the split strings into the string vector
	str			- reference to the delimited token string
	delimiter	- the character delimiter
	Result		- the string vector where the split strings will be appended
*/
void split(const string& Str, char delimiter, vector<string>& Result)
{
	string::size_type i = 0;
	string::size_type j = Str.find(delimiter);
	while (j != string::npos)
	{
		Result.push_back(Str.substr(i, j-i));
		i = ++j;
		j = Str.find(delimiter, j);
		if (j == string::npos)
		Result.push_back(Str.substr(i, Str.length( )));
	}
}

/*
	FreeMapData		- Frees map data array and binary collision array
*/
void FreeMapData(void)
{
	free2Dint(MapData);
	free2Dint(BinaryCollisionArray);
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
			//printf("%i ", MapData[i][j]);
			printf("%i ", MapData[j][i]);
		}

		printf("\n");
	}

	printf("\n\nBinary Collision Data:\n");
	for(j = BINARY_MAP_HEIGHT - 1; j >= 0; --j)
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
	allocate2Dint		- Allocates dynamic memory for a 2D int array
	count_x				- width of the array
	count_y				- height of the array

	return				- double int pointer to the allocated memory
*/
int** allocate2Dint(int count_x, int count_y)
{
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
	}
	free(ptr_array);
}

