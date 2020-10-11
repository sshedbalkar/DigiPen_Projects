/**
 *	\file	CoordinateSystem.h
 *  \author David Yamanoha
 *  \date   09.08.08
 */
#pragma once

#include "Matrix4.h"
#include "Quaternion.h"

/** 
*	\enum COORDINATE_SPACE
*	\brief	For conversions between spaces, we can specify wether these are brought into local reference frames or the
*			world's reference frame.
*/
enum COORDINATE_SPACE{CS_LOCAL, CS_WORLD};

/**
 *	\class	CoordinateSystem
 *  \brief	Matrix4 extended functionality.
 *
 *			This class extends the matrix4 class to provide additional functionality related to coordinate systems.
 */
class CoordinateSystem : public Matrix4
{
public:
	CoordinateSystem();
	CoordinateSystem(const Matrix4& m);

	void	Set(const Vector4& xAxis, const Vector4& yAxis, const Vector4& zAxis, const Vector4& translation);
	void	SetTranslation(const Vector4& translation);
	void	SetLocalTranslation(const Vector4& translation);
	void	SetPosition(const Vector4& translation);
	void	SetOrientation(Vector4 orientation, Vector4 worldUp = Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	

	Vector4 GetTranslation() const;
	Vector4 GetXAxis() const;
	Vector4 GetYAxis() const;
	Vector4 GetZAxis() const;

	Matrix4 GetBasis() const;
	Matrix4 GetIBasis() const;

	Vector4	ToWorld(const Vector4& v) const;
	Vector4	ToLocal(const Vector4& v) const;
};