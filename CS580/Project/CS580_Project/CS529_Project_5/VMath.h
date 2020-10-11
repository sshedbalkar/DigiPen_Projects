///////////////////////////////////////////////////////////////////////////////////////
///
///	\file VMath.h
///	Typedefs the DirectX Extension math library and provides some utility functions.
///
///	Authors: Chris Peters
///	Copyright 2010, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////////////
#ifndef VMATH_H
#define VMATH_H
//#pragma message("Compiling: "__FILE__)
//#pragma once //Makes sure this header is only included once

//Include our math headers
#include "Precompiled.h"
#include <cmath>

//#define EPSILON 0.0001
namespace Wye
{
//#pragma message("Including Class: VMath")
#define EPSILON 0.0001
	typedef D3DXVECTOR2 Vec2;
	typedef D3DXVECTOR3 Vec3;
	typedef D3DXVECTOR4 Vec4;
	typedef D3DXMATRIXA16 Mat4;
	typedef unsigned int uint;
	//
	inline float dot(const Vec2& a, const Vec2& b)
	{
		return a.x * b.x + a.y * b.y;
	}
	inline float dot(const Vec3& a, const Vec3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
	inline float dot(const Vec4& a, const Vec4& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}
	//
	inline float lengthSquared(const Vec2& a)
	{
		return a.x * a.x + a.y * a.y;
	}
	inline float lengthSquared(const Vec3& a)
	{
		return a.x * a.x + a.y * a.y + a.z * a.z;
	}
	inline float lengthSquared(const Vec4& a)
	{
		return a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w;
	}
	//
	inline float normalize(Vec2& a)
	{
		float len = sqrt(lengthSquared(a));
		if(abs(len) > EPSILON)
		{
			a /= len;
		}
		return len;
	}
	inline float normalize(Vec3& a)
	{
		float len = sqrt(lengthSquared(a));
		if(abs(len) > EPSILON)
		{
			a /= len;
		}
		return len;
	}
	inline float normalize(Vec4& a)
	{
		float len = sqrt(lengthSquared(a));
		if(abs(len) > EPSILON)
		{
			a /= len;
		}
		return len;
	}
}
//
#endif
