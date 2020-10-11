/* Start Header -------------------------------------------------------
Copyright (C) 2011 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name		: Global.cpp
Purpose			: Place to keep global data and global functions
Language		: C++/MS C++ 2010 Express
Platform		: Microsoft Visual Studio 2010/i7 2630QM, 8GB DDR3 1600MHz, 3GB NVIDIA GT 555M/Windows 7 Home Premium
Project			: CS529_SantoshShedbalkar_Project1_2
Author			: Santosh Shedbalkar/santosh.s/60001111
Creation date	: 09/19/2011
- End Header --------------------------------------------------------*/

#include "Global.h"
#include "Level1.h"
#include "Level2.h"

// ---------------------------------------------------------------------------
// defines output stream where the output will be written

FILE *outputStream;

// ---------------------------------------------------------------------------
// Array that defines all the game state handlers of all levels

struct LevelHandlers levels[] = 
{
	{
		load1,
		initialize1,
		update1,
		draw1,
		free1,
		unload1
	},
	{
		load2,
		initialize2,
		update2,
		draw2,
		free2,
		unload2
	}
};

int Global_initialize(void)
{
	return CODE_SUCCESS;
}

int Global_destroy(void)
{
	return CODE_SUCCESS;
}

// ---------------------------------------------------------------------------
// function that writes a string to an output stream

int output_To_File(const char* string)
{
	if(fputs(string, outputStream) == EOF)
	{
		printf("[ERROR] Global::output_To_File : Error writing to output stream\n");
		return CODE_FAILURE;
	}
	else
	{
		fflush(outputStream);
		printf(string);
		return CODE_SUCCESS;
	}
}