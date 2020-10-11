// ----------------------------------------------------------------------------
// Project Name		:	Game State Manager
// File Name		  :	Graphics.h
// Author			    :	Dan Weiss
// Creation Date	:	September 22, 2010
// Purpose			  :	Declares graphics wrapper functions.  Currently, we are
//                  wrapping GDI calls to make drawing simple shapes easier.
// History			  :
// - 2010/09/22		:	Initial Implementation
// ----------------------------------------------------------------------------

#ifndef GRAPHICS__H__
#define GRAPHICS__H__

#include <Windows.h>
#include <GdiPlus.h> //used for basic drawing calls
#include <Gdiplusgraphics.h>

extern HDC drawDevice;
extern const RECT WindowRect;
extern const RECT BackgroundRect;

void GFX_Initialize(HWND hWnd);
void GFX_Exit();

void GFX_BeginDraw();
void GFX_EndDraw();

void GFX_DrawRectangle(int centerX, int centerY, int height, int width, unsigned color);
void GFX_DrawEllipse(int centerX, int centerY, int height, int width, unsigned color);
void GFX_DrawLine(int startX, int startY, int endX, int endY, unsigned color);
void GFX_DrawText(int startX, int startY, char *buffer, int length, unsigned color);
void GFX_DrawFrameRate(int startX, int startY, unsigned color);

#endif