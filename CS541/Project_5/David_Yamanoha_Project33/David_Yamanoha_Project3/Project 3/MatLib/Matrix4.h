/**
 *	\file	Matrix4.h
 *  \author David Yamanoha
 *  \date   09.08.08
 */
#pragma once

#include "Vector4.h"
#include "Quaternion.h"

/**
*	\struct Matrix_t
*	\brief	4x4 set of float values representing a matrix.
*/
struct Matrix_t   { float _11; float _12; float _13; float _14;
					float _21; float _22; float _23; float _24;
					float _31; float _32; float _33; float _34;
					float _41; float _42; float _43; float _44;	};

/**
*	\def M_DIMENSION
*	\brief The number width and height of the square matrix represented by this class.
*/
#define M_DIMENSION 4

/**
 *	\class	Matrix4
 *  \brief	4x4 matrix class.
 *			This class is used for any coordinate transforms, and basic matrix operations.
 */
class Matrix4
{
public:
	
	Matrix4();

	void		LoadIdentity();

	void		SetRow(int row, const Vector4& v);
	void		SetRows(const Vector4& r1, const Vector4& r2, const Vector4& r3, const Vector4& r4);
	void		SetColumn(int column, const Vector4& v);
	void		SetColumns(const Vector4& c1, const Vector4& c2, const Vector4& c3, const Vector4& c4);
	Vector4		GetRow(int row) const;
	Vector4		GetColumn(int col) const;
	void		Build(const Quaternion& q);
	
	Vector4		operator*(const Vector4 v) const;
	Matrix4		operator*(const Matrix4 m) const;
	Matrix4		operator~() const;
	bool		operator==(const Matrix4 m)const;
	bool		operator!=(const Matrix4 m)const;
	void		operator=(const Matrix4 m);
	float&		operator()(int i, int j)const;

	Matrix_t	GetTransform() const;

	Matrix4		ElementVariance(const Matrix4& m) const;
	float		Variance() const;

private:
	bool		CheckMatrixBounds(int dimension) const;
	/**
	*	Struct containing the actual values of the matrix.
	*/
	Matrix_t	m_matrix;
};