// ----------------------------------------------------------------------------
// Project Name		:	Cage
// File Name		:	Project1_Win32.cpp
// Author			:	Dan Weiss, Antoine Abi Chakra
// Creation Date	:	September 7, 2010
// Purpose			:	Initial point for creating a window and intializing
//						all related systems for the game state manager project.
// History			:
// - 2010/09/07		:	Initial Implementation
// ----------------------------------------------------------------------------

#include <windows.h>
#include <GdiPlus.h> //used for basic drawing calls
#include <Gdiplusgraphics.h>

#include "System.h"
#include "GameStateList.h"
#include "GameStateManager.h"
#include "Input.h"
#include "FrameRateController.h"
#include "Graphics.h"


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
	"Project 2 - Cage",								/*The window caption*/
	WS_OVERLAPPEDWINDOW,							/*The style of the window, which determines if the window will have a minimize/maximize buttons, if its frame is thick or not.. */
	200,											/*The X position of the top left corner of the window. Remember that (0,0) is the top left of the monitor*/
	100,											/*The Y position of the top left corner of the window. Remember that (0,0) is the top left of the monitor*/
	WindowRect.right - WindowRect.left,										/*The width of the window*/
	WindowRect.bottom - WindowRect.top + GetSystemMetrics( SM_CYCAPTION ),										/*The height of the window*/
	NULL,											/*Handle to the parent window (in case this was a child window)*/
	NULL,											/*Handle to a menu (In case there is a menu for this window)*/
	instance,										/*Instance of the module associated with the window. This is the 1st paramter passed to the WinMain function*/
	NULL);											/*Pointer to a value sent to the window in the WM_CREATE message*/
	ShowWindow(hWnd, show);
	UpdateWindow(hWnd);

	//--------------------------------------
	//Game State Manager Code Initialization
	//--------------------------------------
	Sys_Initialize();
	GSM_Initialize(GS_LEVEL_CAGE);
	FRC_Initialize();
	GFX_Initialize(hWnd);

	//---------------------------------------------------------------------------
	// Main game loop.  This handles the standard game loop behavior of
	//    initializing new game states, updating the current game states, then
	//    freeing the past game states as required.
	//
	// There is also a small section in the update loop where we do a call to
	//    the Windows API function PeekMessage.  This is a nonblocking function
	//    used to process any system messages that Windows is attempting to send
	//    to the process.
	//---------------------------------------------------------------------------
	while (gGameStateCurr != GS_QUIT)
	{
		//-------------------------------------------------------------------------
		// Initialization block.  This will load and intialize whatever the current
		//   state is that is being loaded.  This uses the function pointers for
		//   GameStateLoad and GameStateInit, allowing for different behavior for
		//   each state type.
		//-------------------------------------------------------------------------
		if (gGameStateCurr != GS_RESTART)
		{
			GSM_Update();
			GameStateLoad();
		}
		else
			gGameStateCurr = gGameStateNext = gGameStatePrev;

		GameStateInit();

		//-------------------------------------------------------------------------
		// Update block.  This will handle all behavior associated with updating
		//   the components of the game engine.  At this point, it includes the
		//   updates of the game state manager, as well as input, followed by
		//   the main call to draw all objects.  This uses the function pointers
		//   for GameStateUpdate and GameStateDraw, allowing for state-independent
		//   behavior.
		//
		// This section also contains a while loop for PeekMessage.  This function
		//   is a nonblocking function to process any messages in the queue.  This
		//   function will return a non-zero number if any messages remain,
		//   allowing us to only process messages when they exist.
		//-------------------------------------------------------------------------
		while (gGameStateCurr == gGameStateNext)
		{
			FRC_StartFrame();
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				if (msg.message == WM_QUIT)
					gGameStateNext = GS_QUIT;
			}

			IN_Handle();
			GameStateUpdate();
			GameStateDraw();
			FRC_EndFrame();
		}

		//-------------------------------------------------------------------------
		// Unload block.  This section handles all behavior associated with freeing
		//   objects in current states, as well as the unloading of the current
		//   state, before switching to the next state.
		//-------------------------------------------------------------------------
		GameStateFree();

		if (gGameStateNext != GS_RESTART)
			GameStateUnload();

		gGameStatePrev = gGameStateCurr;
		gGameStateCurr = gGameStateNext;
	}

	//---------------------------------------------------------------------------
	// Cleanup block.  In particular, the reason I added keep track of hWnd in
	//   the project was to call DestroyWindow here.  This safely deletes the
	//   window in the event that it has not been deleted elsewhere.
	//---------------------------------------------------------------------------
	if (hWnd)
		DestroyWindow(hWnd);

	GFX_Exit();
	Sys_Exit();

	UnregisterClass(wc.lpszClassName, instance);

	return (int)msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	switch (msg)
	{
		/* when the window is created */
		case WM_CREATE:
			break;

		/* when the rectangle is drawn */
		case WM_LBUTTONDOWN:
			break;
		
		case WM_MOUSEMOVE:
			break;

		/*case WM_PAINT:
			dc = BeginPaint(hWnd, &ps);
      
      BitBlt(dc, 0, 0, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top,
        drawDevice, WindowRect.left, WindowRect.top, SRCCOPY);

			EndPaint(hWnd, &ps);
			break;*/

		/* When it's time for the window to be closed and removed */
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
				gGameStateNext = GS_QUIT;
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