/**
*	\file	Vector4.h
*	\author David Yamanoha
*	\date   09.08.08
*/
#pragma once

#define GL

/**
*	\class	Vector4
*	\brief	4-dimensional vector class.
*/
class Vector4
{
public:
	/**
	*	3-dimensional vector component: x.
	*/
	float x;
	/**
	*	3-dimensional vector component: y.
	*/
	float y;
	/**
	*	3-dimensional vector component: z.
	*/
	float z;
	/**
	*	3-dimensional vector component (homogneous): w.
	*/
	float w;
	
	Vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);
	void	Set(float x_, float y_, float z_, float w_);
	void	Set(const Vector4& v);
	float	Magnitude() const;
	void	Normalize();
	void	Homogenize();
	void	Zero();

	Vector4	operator+(const Vector4& v) const;
	Vector4	operator*(const float s) const;
	Vector4 operator/(const float s) const;
	Vector4	operator-(const Vector4 v) const;
	Vector4	operator-() const;
	float	operator*(const Vector4 v) const;
	bool	operator==(const Vector4 v) const;
	bool	operator!=(const Vector4& v) const;
	Vector4	operator%(const Vector4& v) const;
	void	operator+=(const Vector4& v);
	void	operator*=(const float scalar);
	void	operator/=(const float scalar);
	void	operator-=(const Vector4& v);
	void	operator=(const Vector4& v);
};