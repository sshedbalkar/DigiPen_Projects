/* Start Header -------------------------------------------------------
Copyright (C) 2011 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name		: Level2.cpp
Purpose			: Implements Level 2 including all the game state handlers for level 2
Language		: C++/MS C++ 2010 Express
Platform		: Microsoft Visual Studio 2010/i7 2630QM, 8GB DDR3 1600MHz, 3GB NVIDIA GT 555M/Windows 7 Home Premium
Project			: CS529_SantoshShedbalkar_Project1_2
Author			: Santosh Shedbalkar/santosh.s/60001111
Creation date	: 09/19/2011
- End Header --------------------------------------------------------*/

#include "Level2.h"
#include "Global.h"
#include "Input.h"
#include "GameStateManager.h"
#include "GameStateList.h"
//
#include <stdlib.h>
//
int Level2_Counter = 0, Level2_Lives = 0, Level2_Max_Counter = 0;
//
int load2(void)
{
	output_To_File("Level2: Load\n");
	//
	int returnCode = CODE_SUCCESS;
	FILE *file;
	char line[100];

	// ---------------------------------------------------------------------------
	// Read the value of level 2 counter from external file

	if (!(file = fopen("Level2_Counter.txt", "r" )))
	{
		printf("[ERROR] Level2::load2 : File \"Level2_Counter.txt\" ccould not be opened for reading\n");
		returnCode = CODE_FAILURE;
	} 
	else 
	{
		if(fgets(line, 100, file) == NULL)
		{
			printf("[ERROR] Level2::load2 : fgets error1\n");
			returnCode = CODE_FAILURE;
			fclose(file);
		}
		else
		{
			Level2_Counter = Level2_Max_Counter = atoi(line);
			fclose(file);
			file = 0;
			
			// ---------------------------------------------------------------------------
			// read the value of level 2 lives from external file

			if (!(file = fopen("Level2_Lives.txt", "r" )))
			{
				printf("[ERROR] Level2::load2 : File \"Level2_Lives.txt\" ccould not be opened for reading\n");
				returnCode = CODE_FAILURE;
			} 
			else 
			{
				if(fgets(line, 100, file) == NULL)
				{
					printf("[ERROR] Level2::load2 : fgets error2\n");
					returnCode = CODE_FAILURE;
				}
				else
				{
					Level2_Lives = atoi(line);
					//printf ("The value entered is %d. The double is %d.\n", Level2_Lives, Level2_Lives*2);
				}
				fclose(file);
			}
		}
	}
	return returnCode;
}

int initialize2(void)
{
	output_To_File("Level2: Initialize\n");
	return CODE_SUCCESS;
}

// ---------------------------------------------------------------------------
// update game state handler of level 2 which uses Level2_Counter and Level2_Lives to determine if the state has to be changed or stay unchanged

int update2(void)
{
	if(Level2_Lives > 0 || Level2_Counter > 0)
	{
		//char buffer[65];
		//
		inputHandle();

		output_To_File("Level2: Update\n");

		if(getNext() != LEVEL_QUIT)
		{
			//printf("Lives:Counter = %d:%d\n", Level2_Lives, Level2_Counter);
			/*output_To_File("level2: update\n");
			output_To_File("lives:counter = ");
			output_To_File(itoa(Level2_Lives, buffer, 10));
			output_To_File(":");
			output_To_File(itoa(Level2_Counter, buffer, 10));
			output_To_File("\n");*/

			if(--Level2_Counter < 1)
			{
				if(--Level2_Lives < 1)
				{
					setNext(LEVEL_QUIT);
				}
				else
				{
					Level2_Counter = Level2_Max_Counter;
					setNext(LEVEL_RESTART);
				}
			}
		}
	}
	return CODE_SUCCESS;
}

int draw2(void)
{
	output_To_File("Level2: Draw\n");
	return CODE_SUCCESS;
}

int free2(void)
{
	output_To_File("Level2: Free\n");
	return CODE_SUCCESS;
}

int unload2(void)
{
	output_To_File("Level2: Unload\n");
	return CODE_SUCCESS;
}