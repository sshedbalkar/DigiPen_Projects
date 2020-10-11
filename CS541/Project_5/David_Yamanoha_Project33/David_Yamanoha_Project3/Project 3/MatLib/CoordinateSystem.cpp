/**
 *	\file	CoordinateSystem.cpp
 *  \author David Yamanoha
 *  \date   09.08.08
 */
#include "CoordinateSystem.h"
#include <math.h>

/** 
*	Constructor.  Intializes it's matrix to the identity.
*	\returns n/a.
*	\callergraph 
*/
CoordinateSystem::CoordinateSystem()
{
	LoadIdentity();
}

CoordinateSystem::CoordinateSystem(const Matrix4& m)
{
	*(Matrix4*)this = m;
}

/** 
*	Sets the coordinate system's axes.  If GL is specified, axes default to row major.  Otherwise rows are used.
*	\param	xAxis Orientation of the coordinate system's x-axis.
*	\param	yAxis Orientation of the coordinate system's y-axis.
*	\param	zAxis Orientation of the coordinate system's z-axis.
*	\param	translation The position of the coordinate system.
*	\returns	void.
*	\callergraph 
*	\callgraph
*/
void CoordinateSystem::Set(const Vector4& xAxis, const Vector4& yAxis, const Vector4& zAxis, const Vector4& translation)
{
#ifdef GL
		SetColumns(xAxis, yAxis, zAxis, translation);
#else
		SetRows(xAxis, yAxis, zAxis, translation);
#endif
}

/** 
*	Gets the coordinate system's x-axis.  If GL is specified, a column will be retrieved.
*	\returns	Coordinate system's x-axis.
*	\callergraph 
*	\callgraph
*/
Vector4 CoordinateSystem::GetXAxis() const
{
#ifdef GL
		return GetColumn(1);	
#else
		return GetRow(1);
#endif
}

/** 
*	Gets the coordinate system's y-axis.  If GL is specified, a column will be retrieved.
*	\returns	Coordinate system's y-axis.
*	\callergraph 
*	\callgraph
*/
Vector4 CoordinateSystem::GetYAxis() const
{
#ifdef GL
		return GetColumn(2);
#else
		return GetRow(2);
#endif
}

/** 
*	Gets the coordinate system's z-axis.  If GL is specified, a column will be retrieved.
*	\returns	Coordinate system's z-axis.
*	\callergraph 
*	\callgraph
*/
Vector4 CoordinateSystem::GetZAxis()const
{
#ifdef GL
		return GetColumn(3);
#else
		return GetRow(3);
#endif
}

/** 
*	Constructs an ortho-normal basis making use of orientation and a worldUp vector.
*	\param	orientation Direction the basis will be oriented in.
*	\param	worldUp	Direction of the world's up vector. (Used to construct the basis via cross product).
*	\note	Unexepected behavior will incur if <i>orienation == worldUp</i> OR if <i>orientation</i> is parallel to
*			<i>worldUp</i>.
*	\returns	void.
*	\callergraph 
*	\callgraph
*/
void CoordinateSystem::SetOrientation(Vector4 orientation, Vector4 worldUp)
{
#ifdef GL
	worldUp.Set(0.0f, 0.0f, 1.0f, 0.0f);
#endif
	orientation.Normalize();
	Vector4 orientation_perp(worldUp % orientation);
	orientation_perp.Normalize();
	Vector4 orientationUp(orientation % orientation_perp);
	orientationUp.Normalize();
#ifdef GL
		SetColumns(orientation_perp, orientation, orientationUp, GetColumn(4));
#else
		SetRows(orientation_perp, orientationUp, orientation, GetRow(4));
#endif	
}

/** 
*	Translates the the coordinate system from it's current world-defined position.
*	\param	translation	Vector by which to translate by.
*	\returns	void.
*	\callergraph 
*	\callgraph
*/
void CoordinateSystem::SetTranslation(const Vector4& translation)
{
#ifdef GL
		SetColumn(4, GetColumn(4) + translation);
#else
		SetRow(4, GetRow(4) + translation);
#endif
}

/** 
*	Translates the the coordinate system from it's current world-defined position where <i>translation</i> is defined
*	within it's own local coordinate system.
*	\param	translation Vector by which to translate by.
*	\returns	void.
*	\callergraph 
*	\callgraph
*/
void CoordinateSystem::SetLocalTranslation(const Vector4& translation)
{
	SetTranslation(ToWorld(translation));
}

/** 
*	Sets the position of the coordinate system in world space.
*	\param	position New position of the coordinate system.
*	\returns	void.
*	\callergraph 
*	\callgraph
*/
void CoordinateSystem::SetPosition(const Vector4& position)
{
#ifdef GL
		SetColumn(4, position);
#else
		SetRow(4, position);
#endif
}

/** 
*	Retrieves the matrix representation of the coordinate system.
*	\returns	Coordinate system's matrix representation.
*	\callergraph 
*	\callgraph
*/
Matrix4 CoordinateSystem::GetBasis() const
{
	return *this;
}

/** 
*	Retrieves the inverse basis of the coordinate system.
*	\note	IMPORTANT:  The inverse basis here is <i>not</i> the inverse of the actual matrix.  It is, however, the 
*			the coordinate system un-translated, and un-rotated.  This function should be used as a helper for 
*			coordinate system conversions, and does not reflect actual matrix inversion such as Guass-Jordan.
*	\returns	Coordinate system's un-translated and un-rotated matrix representation.
*	\callergraph
*	\callgraph
*/
Matrix4 CoordinateSystem::GetIBasis() const
{
	return ~(*this);
}

/** 
*	Transforms a point from the local coordiante system into the world's.
*	\param v	A pointer to the vector to convert into the world's coordinate system. 
*	\returns	Coordinate system's un-translated and un-rotated matrix representation.
*	\callergraph 
*	\callgraph
*/
Vector4 CoordinateSystem::ToWorld(const Vector4& v) const
{
	return (*this) * v;
}

/** 
*	Transforms a point from the world's coordiante system into the local one.
*	\param v	A to the vector to convert into the coordinate system's local reference frame.
*	\note	IMPORTANT:  The inverse basis here is <i>not</i> the inverse of the actual matrix.  It is, however, the 
*			the coordinate system un-translated, and un-rotated.  This function should be used as a helper for 
*			coordinate system conversions, and does not reflect actual matrix inversion such as Guass-Jordan.
*	\returns	Coordinate system's un-translated and un-rotated matrix representation.
*	\callergraph 
*	\callgraph
*/
Vector4 CoordinateSystem::ToLocal(const Vector4& v) const
{
	return ~(*this) * v;
}

/** 
*	Returns the translation portion of the coordinate system.
*	\returns	Coordinate system's translation.
*	\callergraph 
*	\callgraph
*/
Vector4 CoordinateSystem::GetTranslation() const
{
#ifdef GL
		return GetColumn(4);
#else
		return GetRow(4);
#endif
}