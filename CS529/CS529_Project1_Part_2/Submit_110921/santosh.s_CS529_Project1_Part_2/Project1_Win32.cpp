/* Start Header -------------------------------------------------------
Copyright (C) 2011 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name		: Project1_Win32.cpp
Purpose			: Initial point for creating a window and intializing all related systems for the game state manager project.
Language		: C++/MS C++ 2010 Express
Platform		: Microsoft Visual Studio 2010/i7 2630QM, 8GB DDR3 1600MHz, 3GB NVIDIA GT 555M/Windows 7 Home Premium
Project			: CS529_SantoshShedbalkar_Project1_2
Author			: Santosh Shedbalkar/santosh.s/60001111
Creation date	: 09/20/2011
- End Header --------------------------------------------------------*/

#include "System.h"
#include "GameStateManager.h"
#include "GameStateList.h"
#include "Global.h"
//
#include <windows.h>

// ----------------------------------------------------
// Function declarations

void initializeApp(void);
void gameLoop(void);
void exitApp(void);

LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

//
//  FUNCTION: WinMain()
//
//  PURPOSE: Main used by the Windows API to run Win32 programs.
//
//  COMMENTS:
//
//    This is the replacement for main seen in traditional C/C++ programs.
//    The Windows API will always be looking for a WinMain function, so it
//    can be treated as such.  The function params are a bit different, so if
//    your program requires their usage, the information can be found on MSDN.
//
int initDone = 0;
int WINAPI WinMain(HINSTANCE instance, HINSTANCE hPreviousInstance, LPSTR command_line, int show)
{
	WNDCLASS wc;
	HWND hWnd;
	MSG msg;

	wc.style = CS_HREDRAW | CS_VREDRAW;							/*Class style*/
	wc.lpfnWndProc = WinProc;									/*A function pointer which should point to the procedure function. Remember that the procedure function handles the window messages*/
	wc.cbClsExtra = 0;											/*The number of extra bytes you want to allocate for this window class structure. The default value is 0*/
	wc.cbWndExtra = 0;											/*The number of extra bytes you want to allocate for the window instance*/
	wc.hInstance = instance;									/*Instance of the module associated with the window. This is the 1st paramter passed to the WinMain function*/
	wc.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);					/*Handle to the icon class which will be displayed on the top left part of the window*/
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);					/*Handle to the cursor class which will be used in this window class*/
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		/*Handle to the class background brush. It can be a color value*/
	wc.lpszMenuName = NULL;										/*Pointer to a null terminated string which specifies the name of the menu resources used by this window class (if any)*/
	wc.lpszClassName = "Window Class Name";						/*String that specifies thw window class name. This is needed to create any window based on this window class*/

	RegisterClass(&wc);


	hWnd = CreateWindow(wc.lpszClassName,			/*The class name we chose for the window class*/
		"Win32 Sample",								/*The window caption*/
		WS_OVERLAPPEDWINDOW,						/*The style of the window, which determines if the window will have a minimize/maximize buttons, if its frame is thick or not.. */
		200,										/*The X position of the top left corner of the window. Remember that (0,0) is the top left of the monitor*/
		100,										/*The Y position of the top left corner of the window. Remember that (0,0) is the top left of the monitor*/
		640,										/*The width of the window*/
		480,										/*The heiht of the window*/
		NULL,										/*Handle to the parent window (in case this was a child window)*/
		NULL,										/*Handle to a menu (In case there is a menu for this window)*/
		instance,									/*Instance of the module associated with the window. This is the 1st paramter passed to the WinMain function*/
		NULL);										/*Pointer to a value sent to the window in the WM_CREATE message*/
	ShowWindow(hWnd, show);
	UpdateWindow(hWnd);

	//while (GetMessage(&msg, NULL, 0, 0))
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// ---------------------------------------------------------------------------
	// Main application execution

	initializeApp();
	gameLoop();
	exitApp();

	//---------------------------------------------------------------------------
	// Cleanup block.  In particular, the reason I added keep track of hWnd in
	//   the project was to call DestroyWindow here.  This safely deletes the
	//   window in the event that it has not been deleted elsewhere.
	//---------------------------------------------------------------------------
	if (hWnd)
		DestroyWindow(hWnd);

	return (int) msg.wParam;

	UnregisterClass(wc.lpszClassName, instance);

	return (int)msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	HDC dc;           /* device context */
	PAINTSTRUCT ps;   /* the paint struct */

	switch (msg)
	{
		/* when the window ibvs created */
		case WM_CREATE:
			break;

		/* when the rectangle is drawn */
		case WM_LBUTTONDOWN:
			break;
		
		case WM_MOUSEMOVE:
			break;

		case WM_PAINT:
			dc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		/* When it's time for the window to be closed and removed */
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				setNext(LEVEL_QUIT);// Quit the application by setting the next game state indicator to the QUIT "state"
			}
			break;

		/* called any time the window is moved */
		case WM_MOVE:
			/* Invalidate the rect to force a redraw */
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

void gameLoop(void)
{
	// ----------------------------------------------------
	// implementation of the game loop in the simplest form

	while(getCurrent() != LEVEL_QUIT)
	{
		if(getCurrent() == LEVEL_RESTART)
		{
			setCurrent(setNext(getPrev()));
		}
		else
		{
			GSM_Update();
			levelLoad();
		}
		levelInitialize();
		
		while(getCurrent() == getNext())
		{
			levelUpdate();
			levelDraw();
		}

		levelFree();

		if(getNext() != LEVEL_RESTART)
		{
			levelUnload();
		}

		setPrev(getCurrent());
		setCurrent(getNext());
	}
}


void exitApp(void)
{
	// ----------------------------------------------------
	// Application exit

	Global_destroy();
	System_Exit();
}

void initializeApp(void)
{
	// ---------------------------------------------------
	// Application initialization

	System_Initialize();
	Global_initialize();
	GSM_Initialize(LEVEL_1);
}