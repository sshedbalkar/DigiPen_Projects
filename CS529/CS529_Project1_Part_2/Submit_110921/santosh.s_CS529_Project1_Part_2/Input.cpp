/* Start Header -------------------------------------------------------
Copyright (C) 2011 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name		: Input.cpp
Purpose			: Implementation of all the input handlers
Language		: C++/MS C++ 2010 Express
Platform		: Microsoft Visual Studio 2010/i7 2630QM, 8GB DDR3 1600MHz, 3GB NVIDIA GT 555M/Windows 7 Home Premium
Project			: CS529_SantoshShedbalkar_Project1_2
Author			: Santosh Shedbalkar/santosh.s/60001111
Creation date	: 09/19/2011
- End Header --------------------------------------------------------*/

#include "Input.h"
#include "Global.h"
//
#include <Windows.h>
//
void inputHandle(void)
{
	// ---------------------------------------------------------------------------
	// Copy event messages from the system queue to the application local queue

	MSG msg;
	//
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	//
	output_To_File("Input: Handle\n");
}