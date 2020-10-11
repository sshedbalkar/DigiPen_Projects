///////////////////////////////////////////////////////////////////////////////////////
///
///	\file VMath.h
///	Typedefs the DirectX Extension math library and provides some utility functions.
///
///	Authors: Chris Peters
///	Copyright 2010, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////////////
#pragma once //Makes sure this header is only included once

//Include our math headers
#include <d3dx10.h>
#include <cmath>
#include <vector>
//#include "Serialization.h"



namespace Framework
{
#define S_DELETE(x)			{if(x) delete(x);x=NULL;}
#define S_RELEASE(x)		{if(x) x->Release();x=NULL;}
#define EPSILON 0.0001
	typedef D3DXVECTOR2 Vec2;
	typedef D3DXVECTOR3 Vec3;
	typedef D3DXVECTOR4 Vec4;
	typedef D3DXMATRIXA16 Mat4;
	typedef D3DXQUATERNION	Quat;

	struct Line
	{
		Vec2 start;
		Vec2 end;
	};
	typedef std::vector<Line> LineListType;

	inline float dot(const Vec2& a, const Vec2& b)
	{
		return a.x * b.x + a.y * b.y;
	}

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

	inline float Rand( float lower, float higher )
	{
		float ret = (float)(rand()%10000) / 9999.0f;
		ret *= ( higher - lower );
		ret += lower;
		return ret;
	}

	inline float GaussianRand( float expectation, float deviation )
	{
		float r0 = (float)(rand()%10000) / 5000.0f;
		float r1 = (float)(rand()%10000) / 5000.0f;
		float r2 = (float)(rand()%10000) / 5000.0f;

		float result = ( r0 + r1 + r2 ) / 3.0f;
		result -= 1.0f;
		result *= deviation;
		result += expectation;
		return result;
	}

	inline void QuatMultiply( Vec4& out, Vec4& a, Vec4& b )
	{
		//r.w = (qa.w * qb.w) - (qa.x * qb.x) - (qa.y * qb.y) - (qa.z * qb.z);
		//r.x = (qa.x * qb.w) + (qa.w * qb.x) + (qa.y * qb.z) - (qa.z * qb.y);
		//r.y = (qa.y * qb.w) + (qa.w * qb.y) + (qa.z * qb.x) - (qa.x * qb.z);
		//r.z = (qa.z * qb.w) + (qa.w * qb.z) + (qa.x * qb.y) - (qa.y * qb.x);
		Vec4 temp;
		temp.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
		temp.x = a.x * b.w + a.w * b.x + a.y * b.z - a.z * b.y;
		temp.y = a.y * b.w + a.w * b.y + a.z * b.x - a.x * b.z;
		temp.z = a.z * b.w + a.w * b.z + a.x * b.y - a.y * b.x;
		out = temp;
	}

	inline void QuatRotatePoint( Vec4& quat, Vec3& in, Vec3& out )
	{
		Quat temp;
		temp.w = -(quat.x * in.x) - quat.y * in.y - quat.z * in.z;
		temp.x = quat.w * in.x + quat.y * in.z - quat.z * in.y;
		temp.y = quat.w * in.y + quat.z * in.x - quat.x * in.z;
		temp.z = quat.w * in.z + quat.x * in.y - quat.y * in.x;
		Quat inv;
		inv.w = quat.w; inv.x = -quat.x; inv.y = -quat.y; inv.z = -quat.z;
		out.x = temp.x * inv.w + temp.w * inv.x + temp.y * inv.z - temp.z * inv.y;
		out.y = temp.y * inv.w + temp.w * inv.y + temp.z * inv.x - temp.x * inv.z;
		out.z = temp.z * inv.w + temp.w * inv.z + temp.x * inv.y - temp.y * inv.x;
	}

	inline void QuatNormalize( Vec4& quat )
	{
		float c = quat.w;
		float s = 1.0f - c * c;
		if ( s < 0.0f ) s = 0.0f;
		else s = sqrt(s);
		float dis = quat.x * quat.x + quat.y * quat.y + quat.z * quat.z;
		if ( dis < 0.0001f )
		{
			quat.x = 0.0f;
			quat.y = 0.0f;
			quat.z = 0.0f;
		}
		else 
		{
			dis = sqrt(dis);
			quat.x /= dis;
			quat.x *= s;
			quat.y /= dis;
			quat.y *= s;
			quat.z /= dis;
			quat.z *= s;
		}
	}

	inline void QuatInverse( Vec4& quat )
	{
		quat.x = -quat.x;
		quat.y = -quat.y;
		quat.z = -quat.z;
	}

	typedef unsigned int uint;
}
