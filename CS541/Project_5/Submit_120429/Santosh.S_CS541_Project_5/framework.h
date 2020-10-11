///////////////////////////////////////////////////////////////////////
// $Id$
//
// Provides the framework for graphics projects. Opens a window and
// hooks up all GUI callbacks.
//
// Gary Herron
//
// Copyright © 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#if !defined(_FRAMEWORK_H)
#define _FRAMEWORK_H

#ifdef _WIN32

// Included for Windows
#include <windows.h>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <crtdbg.h>

#else

// Includes for Linux
#include <string>
#include <fstream>
#include <values.h>
#define sprintf_s sprintf
#endif


#include <GL/gl.h>
#include <GL/glu.h>
#include <freeglut.h>

#include <vector>
#include "math.h"

#include "geomlib.h"

#endif // _FRAMEWORK_H
