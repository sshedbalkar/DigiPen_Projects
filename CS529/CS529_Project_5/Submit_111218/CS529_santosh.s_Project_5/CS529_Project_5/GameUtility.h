////////////////////////////////////////////////////////////////
//
//	GameUtility.h
//	various game specific utility functions
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef GAME_UTILITY_H
#define GAME_UTILITY_H
//
#include "VMath.h"
//
namespace Wye
{
#pragma message("Including class: GameUtility")
	Vec3 coordToPixel(float x, float y);
	Vec2 pixelToCoord(const Vec3& pixelPos);
	unsigned getNextHighestDepth();
}
//
#endif