/* Start Header -------------------------------------------------------
Copyright (C) 2011 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name		: Level1.cpp
Purpose			: Implements Level 1 including all the game state handlers for level 1
Language		: C++/MS C++ 2010 Express
Platform		: Microsoft Visual Studio 2010/i7 2630QM, 8GB DDR3 1600MHz, 3GB NVIDIA GT 555M/Windows 7 Home Premium
Project			: CS529_SantoshShedbalkar_Project1_2
Author			: Santosh Shedbalkar/santosh.s/60001111
Creation date	: 09/19/2011
- End Header --------------------------------------------------------*/

#include "Level1.h"
#include "Global.h"
#include "Input.h"
#include "GameStateManager.h"
#include "GameStateList.h"
//
#include <stdlib.h>
//
int Level1_Counter = 0;
//
int load1(void)
{
	output_To_File("Level1: Load\n");
	//
	int returnCode = CODE_SUCCESS;
	FILE *file;
	char line[100];
	
	// ---------------------------------------------------------------------------
	// Read level 1 counter value from an extern file

	if (!(file = fopen("Level1_Counter.txt", "r" )))
	{
		printf("[ERROR] Level1::load1 : File \"Level1_Counter.txt\" could not be opened for reading\n");
		returnCode = CODE_FAILURE;
	} 
	else 
	{
		if(fgets(line, 100, file) == NULL)
		{
			printf("[ERROR] Level1::load1 : fgets error\n");
			returnCode = CODE_FAILURE;
		}
		else
		{
			//printf("Line: %s", line);
			//output_To_File(line);
			Level1_Counter = atoi(line);
			//printf ("The value entered is %d. The double is %d.\n", Level1_Counter, Level1_Counter*2);
		}
		fclose(file);
	}
	return returnCode;
}

int initialize1(void)
{
	output_To_File("Level1: Initialize\n");
	return CODE_SUCCESS;
}

// ---------------------------------------------------------------------------
// Update game state handler of level 1 which decrements the counter every iteration and sets the 'next' variable to level 2 when the counter becomes 0

int update1(void)
{
	if(Level1_Counter > 0)
	{
		inputHandle();

		output_To_File("Level1: Update\n");

		if(getNext() != LEVEL_QUIT)
		{
			if(--Level1_Counter < 1)
			{
				setNext(LEVEL_2);
			}
		}
		//printf("Level1 counter: %d\n", Level1_Counter);
	}
	return CODE_SUCCESS;
}

int draw1(void)
{
	output_To_File("Level1: Draw\n");
	return CODE_SUCCESS;
}

int free1(void)
{
	output_To_File("Level1: Free\n");
	return CODE_SUCCESS;
}

int unload1(void)
{
	output_To_File("Level1: Unload\n");
	return CODE_SUCCESS;
}