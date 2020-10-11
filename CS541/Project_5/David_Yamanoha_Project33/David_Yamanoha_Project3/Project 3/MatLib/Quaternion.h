#pragma once
#include "Vector4.h"

class Quaternion
{
public:
	float x;
	float y;
	float z;
	float w;

				Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);
	void		Set(float x, float y, float z, float w);
	float		Dot(const Quaternion& q) const;
	void		ComputeW();
	void		Normalize();
	float		Magnitude();
	Vector4		RotatePoint(const Vector4& v) const;
	Quaternion	Slerp(const Quaternion& a, const Quaternion& b, float t);

	void		operator=(const Quaternion& q);
	bool		operator==(const Quaternion& q) const;
	bool		operator!=(const Quaternion& q) const;
	Quaternion	operator-(const Quaternion& q) const;
	Quaternion	operator+(const Quaternion& q) const;
	Quaternion	operator-() const;
	Quaternion	operator*(const Quaternion& q) const;
	Quaternion	operator*(const Vector4& v) const;
	Quaternion	operator*(const float f) const;
	Quaternion	operator/(const float f) const;
};