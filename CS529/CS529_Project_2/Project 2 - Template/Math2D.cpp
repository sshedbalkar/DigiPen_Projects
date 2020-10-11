#include "Math2D.h"
#include "stdio.h"


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