/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name		: System.cpp
Purpose			: File that implements all the system handlers
Language		: C++/MS C++ 2010 Express
Platform		: Microsoft Visual C++ 2010/i7 2630QM, 8GB DDR3 1600MHz, 3GB NVIDIA GT 555M/Windows 7 Home Premium
Project			: CS529_SantoshShedbalkar_Project1_1
Author			: Santosh Shedbalkar/santosh.s/60001111
Creation date	: 09/19/2011
- End Header --------------------------------------------------------*/

#include "System.h"
#include "Global.h"

// ---------------------------------------------------------------------------
// Initializes the system for the application. Opens a file stream for outputting game messages

int System_Initialize(void)
{
	FILE *file;
	int returnCode = CODE_SUCCESS;
	//
	if (!(file = fopen("Output.txt", "w+" )))
	{
		printf("[ERROR] System::System_Initialize : File \"Output.txt\" could not be opened for reading/writing \n");
		returnCode = CODE_FAILURE;
	} 
	else 
	{
		outputStream = file;
	}
	output_To_File("System: Initialize\n");
	return returnCode;
}

// ---------------------------------------------------------------------------
// Closes the system after the application execution. Closes the file stream

int System_Exit(void)
{
	output_To_File("System: Exit");
	if(fclose(outputStream) == EOF)
	{
		printf("[ERROR] System::System_Exit : File \"Output.txt\" could not be closed\n");
		return CODE_FAILURE;
	}
	else
	{
		return CODE_SUCCESS;
	}
}