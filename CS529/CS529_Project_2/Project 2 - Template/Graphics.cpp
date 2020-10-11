// ----------------------------------------------------------------------------
// Project Name			:	Game State Manager
// File Name			:	Graphics.c
// Author			    :	Dan Weiss
// Creation Date		:	September 22, 2010
// Purpose				:	Contains the implementation of the graphics wrapper.  This
//							includes the base functionality of setting up the second
//							draw buffer, as well as the implementation of simple shape
//							drawing, line drawing, and text drawing.  Also contains the
//							function that swaps buffers after drawing is complete.
// History				:
// - 2010/09/22			:	Initial Implementation
// ----------------------------------------------------------------------------

#include "Graphics.h"
#include "FrameRateController.h"

//-----------------------------------------------------------------------------
// Buffer objects and associated memory.
//-----------------------------------------------------------------------------
HDC drawDevice, mainDC;
HBITMAP hMemBitmap;
HGDIOBJ hOldBitmap;

//-----------------------------------------------------------------------------
// Window boundaries
//-----------------------------------------------------------------------------
const RECT WindowRect = { 0, 0, 800, 600 };
const RECT BackgroundRect = { 0, 0, WindowRect.right, WindowRect.bottom}; //slightly larger to allow for
                                                   //overdraw of the back buffer

//-----------------------------------------------------------------------------
// Function: GFX_Initialize
//
// Purpose: Initializes the data associated with the graphics wrapper.  In this
//          case, we are creating a second device context to use as a back
//          buffer for our GDI draw calls.
//
// Params:
//    HWND hWnd: Handle to the Win32 window created at the beginning of the
//               program execution.
//-----------------------------------------------------------------------------
void GFX_Initialize(HWND hWnd)
{
  mainDC = GetDC(hWnd);                    //Get the window's device context
  drawDevice = CreateCompatibleDC(mainDC); //Create a back buffer compatible with
                                           //the main device context

  //Creates the memory block necessary for the back buffer to draw to
  hMemBitmap = CreateCompatibleBitmap( 
      mainDC, 
      WindowRect.right - WindowRect.left, 
      WindowRect.bottom - WindowRect.top );
  hOldBitmap = SelectObject( drawDevice, hMemBitmap );
}

//-----------------------------------------------------------------------------
// Function: GFX_Exit
//
// Purpose: Safely frees any memory created for our graphics wrapper.
//-----------------------------------------------------------------------------
void GFX_Exit()
{
  DeleteDC(drawDevice);
  SelectObject( drawDevice, hOldBitmap );
  DeleteObject( hMemBitmap );
}

//-----------------------------------------------------------------------------
// Function: GFX_BeginDraw
//
// Purpose: Initializes the back buffer to a white screen to allow for the
//          start of the current draw loop.
//-----------------------------------------------------------------------------
void GFX_BeginDraw()
{
  FillRect( drawDevice, &BackgroundRect, (HBRUSH)GetStockObject( WHITE_BRUSH ) );
}

//-----------------------------------------------------------------------------
// Function: GFX_EndDraw
//
// Purpose: Performs the bit block transfer of the second device context
//          (representing the back buffer) into the device context of the main
//          window.
//-----------------------------------------------------------------------------
void GFX_EndDraw()
{
	BitBlt(mainDC, 0, 0, WindowRect.right, WindowRect.bottom,
         drawDevice, WindowRect.left, WindowRect.top, SRCCOPY);
}

//-----------------------------------------------------------------------------
// Function: GFX_DrawRectangle
//
// Purpose: Draws a rectangle to the screen.
//
// Params:
//    int centerX: X position of the center of the rectangle
//    int centerY: Y position of the center of the rectangle
//    int height: Height (in pixels) of the rectangle
//    int width: Width (in pixels) of the rectangle
//    unsigned color: Color to use in drawing the rectangle.  This is in the
//                    format 0x00bbggrr.
//-----------------------------------------------------------------------------
void GFX_DrawRectangle(int centerX, int centerY, int height, int width, unsigned color)
{
  HBRUSH hBrush = CreateSolidBrush(color);
  HPEN hPen = CreatePen(PS_SOLID, 1, color);
  SelectObject(drawDevice, hPen);
  SelectObject(drawDevice, hBrush);

  Rectangle(drawDevice, centerX - width/2, centerY - height/2, centerX + width/2, centerY + height/2);

  DeleteObject(hPen);
  DeleteObject(hBrush);
}

//-----------------------------------------------------------------------------
// Function: GFX_DrawEllipse
//
// Purpose: Draws an ellipse to the screen.
//
// Params:
//    int centerX:	X position of the center of the ellipse
//    int centerY:	Y position of the center of the ellipse
//    int raiusA:	Radius A  (in pixels) of the ellipse
//    int radiusB:	Radius B (in pixels) of the ellipse
//    unsigned color: Color to use in drawing the ellipse.  This is in the
//                    format 0x00bbggrr.
//-----------------------------------------------------------------------------
void GFX_DrawEllipse(int centerX, int centerY, int radiusA, int radiusB, unsigned color)
{
  HBRUSH hBrush = CreateSolidBrush(color);
  HPEN hPen = CreatePen(PS_SOLID, 1, color);
  SelectObject(drawDevice, hPen);
  SelectObject(drawDevice, hBrush);

  Ellipse(drawDevice, centerX - radiusA, centerY - radiusB, centerX + radiusA, centerY + radiusB);

  DeleteObject(hPen);
  DeleteObject(hBrush);
}

//-----------------------------------------------------------------------------
// Function: GFX_DrawLine
//
// Purpose: Draws a line to the screen.
//
// Params:
//    int startX: X position of the start of the line
//    int startY: Y position of the start of the line
//    int endX: X position of the end of the line
//    int endY: Y position of the end of the line
//    unsigned color: Color to use in drawing the line.  This is in the
//                    format 0x00bbggrr.
//-----------------------------------------------------------------------------
void GFX_DrawLine(int startX, int startY, int endX, int endY, unsigned color)
{
  HPEN hPen = CreatePen(PS_SOLID, 1, color);
  SelectObject(drawDevice, hPen);

  MoveToEx(drawDevice, startX, startY, NULL);
  LineTo(drawDevice, endX, endY);

  DeleteObject(hPen);
}

//-----------------------------------------------------------------------------
// Function: GFX_DrawText
// 
// Purpose: Draws the specified text to the screen.
//
// Params:
//    int startX: X position of the top left of the first character in the 
//                string being drawn.
//    int startY: Y position of the top left of the first character in the 
//                string being drawn.
//    char *buffer: String being drawn to the screen
//    int length: Number of characters in the buffer
//    unsigned color: Color to use in drawing the line.  This is in the
//                    format 0x00bbggrr.
//-----------------------------------------------------------------------------
void GFX_DrawText(int startX, int startY, char *buffer, int length, unsigned color)
{
  SetTextColor(drawDevice, color);
  SetBkColor(drawDevice, RGB(255,255,255));
  TextOut(drawDevice, startX, startY, buffer, length);
}

//-----------------------------------------------------------------------------
// Function: GFX_DrawFrameRate
// 
// Purpose: Draws the current frame rate to the screen.
//
// Params:
//    int startX: X position of the top left of the first character in the 
//                frame rate string.
//    int startY: Y position of the top left of the first character in the 
//                frame rate string.
//    unsigned color: Color to use in drawing the line.  This is in the
//                    format 0x00bbggrr.
//-----------------------------------------------------------------------------
void GFX_DrawFrameRate(int startX, int startY, unsigned color)
{
  char buffer[256];
  _itoa_s(int(gFrameRate + 0.5),buffer,10);
  GFX_DrawText(startX, startY, buffer, 2, color);
}
