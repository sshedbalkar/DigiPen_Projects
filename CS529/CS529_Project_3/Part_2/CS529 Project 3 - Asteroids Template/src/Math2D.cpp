#include "Math2D.h"
#include "stdio.h"

////////////////////
// From Project 2 //
////////////////////

bool StaticPointToStaticCircle(Vector2D &P, Vector2D &Center, float Radius)
{
	return false;
}

float StaticPointToStaticLineSegment(Vector2D &P, LineSegment2D &LS)
{
	return 0.0f;
}

float AnimatedPointToStaticLineSegment(Vector2D &Ps, Vector2D &Pe, LineSegment2D &LS, Vector2D &Ph)
{
	return 0.0f;
}

float AnimatedCircleToStaticLineSegment(Vector2D &Ps, Vector2D &Pe, float Radius, LineSegment2D &LS, Vector2D &Ph)
{
	return 0.0f;
}

float ReflectAnimatedPointOnStaticLineSegment(Vector2D &Ps, Vector2D &Pe, LineSegment2D &LS, Vector2D &Ph, Vector2D &R)
{
	return 0.0f;
}

float ReflectAnimatedCircleOnStaticLineSegment(Vector2D &Ps, Vector2D &Pe, float Radius, LineSegment2D &LS, Vector2D &Ph, Vector2D &R)
{
	return 0.0f;
}

float AnimatedPointToStaticCircle(Vector2D &Ps, Vector2D &Pe, Vector2D &Center, float Radius, Vector2D &Pi)
{
	return 0.0f;
}

float ReflectAnimatedPointOnStaticCircle(Vector2D &Ps, Vector2D &Pe, Vector2D &Center, float Radius, Vector2D &Pi, Vector2D &R)
{
	return 0.0f;
}

float AnimatedCircleToStaticCircle(Vector2D &Center0s, Vector2D &Center0e, float Radius0, Vector2D &Center1, float Radius1, Vector2D &Pi)
{
	return 0.0f;
}

float ReflectAnimatedCircleOnStaticCircle(Vector2D &Center0s, Vector2D &Center0e, float Radius0, Vector2D &Center1, float Radius1, Vector2D &Pi, Vector2D &R)
{
	return 0.0f;
}


//////////////////////
// New in project 3 //
//////////////////////

/*
This function test if the point Pos is colliding with the circle
whose center is Ctr and radius is "Radius"
*/
bool StaticPointToStaticCircle(Vector2D &Pos, Vector2D &Ctr, float Radius);

/*
This function checks if the point Pos is colliding with the rectangle
whose center is Rect, width is "Width" and height is Height
*/
bool StaticPointToStaticRect(Vector2D &Pos, Vector2D &Rect, float Width, float Height);

/*
This function checks for collision between 2 circles.
Circle0: Center is Center0, radius is "Radius0"
Circle1: Center is Center1, radius is "Radius1"
*/
bool StaticCircleToStaticCircle(Vector2D &Center0, float Radius0, Vector2D &Center1, float Radius1);

/*
This functions checks if 2 rectangles are colliding
Rectangle0: Center is pRect0, width is "Width0" and height is "Height0"
Rectangle1: Center is pRect1, width is "Width1" and height is "Height1"
*/
bool StaticRectToStaticRect(Vector2D &Rect0, float Width0, float Height0, Vector2D &Rect1, float Width1, float Height1);
