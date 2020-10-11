/**
 *	\file	Vector4.cpp
 *  \author David Yamanoha
 *  \date   09.08.08
 */
#include "Vector4.h"
#include <math.h>

/**
*	Constructor.
*	\param x	x component value.
*	\param y	y component value.
*	\param z	z component value.
*	\param w	w component value.
*	\return n/a.
*	\callergraph
*	\callgraph
*/
Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

/**
*	Allows you to set all components in a concise manner.
*	\param x	x component value.
*	\param y	y component value.
*	\param z	z component value.
*	\param w	w component value.
*	\return void.
*	\callergraph
*	\callgraph
*/
void Vector4::Set(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

/**
*	Allows you to set all components in a concise manner via a vector.
*	\param 	v component value.
*	\return void.
*	\callergraph
*	\callgraph
*/
void Vector4::Set(const Vector4& v)
{
	Set(v.x, v.y, v.z, v.w);
}

/**
*	Computes the length of the vector.  Note: The homogenous coordinate is <i>not</i> included in the
*	the calculation of magnitude.
*	<p><i>sqrt(x^2 + y^2 + z^2)</i></p>.
*	\return		Length of the vector.
*	\callergraph
*	\callgraph
*/
float Vector4::Magnitude() const
{
	return sqrt(x*x + y*y + z*z);	
}

/**
*	Turns vector into a unit vector (Magnitude == 1).
*		<p><i>	x/sqrt(x^2 + y^2 + z^2)<br>
*				y/sqrt(x^2 + y^2 + z^2)<br>
*				z/sqrt(x^2 + y^2 + z^2)	</i></p>
*	\return		void.
*	\callergraph
*	\callgraph
*/
void Vector4::Normalize()
{
	float m = Magnitude();
	if(m > 0.0f)
	{
		x /= m;
		y /= m;
		z /= m;
	}
}

/**
*	Projects the vector from the fourth dimension into the third.
*	\return	void.
*	\callergraph
*	\callgraph
*/
void Vector4::Homogenize()
{
	if(w == 0.0f)
	{
		Zero();
		return;
	}

	x /= w;
	y /= w;
	z /= w;
	w = 1.0f;
}

/**
*	Sets all components of the vector to 0.
*	\return		void.
*	\callergraph
*	\callgraph
*/
void Vector4::Zero()
{
	x = y = z = w = 0.0f;
}	

/**
*	Vector addition.
*	\param	v Operand to add.
*	\note	Point addition has no meaning.  If your vector represents a point, your homogenous
*			coordinates will turn to something meaningless under this operation, and take you out of 3D space.  
*	\return Sum of of the vector operands.
*	\callergraph
*	\callgraph
*/
Vector4	Vector4::operator+(const Vector4& v) const
{
	
	Vector4 temp;
	temp.x = x + v.x;
	temp.y = y + v.y;
	temp.z = z + v.z;
	temp.w = w + v.w;
	return temp;
}

/**
*	Vector scalar multiplication.
*	\param s	Operand to scale by.
*	\note	Homogenous coordinate is not scaled.
*	\return	Scaled vector.
*	\callergraph
*	\callgraph
*/
Vector4	Vector4::operator*(const float s) const

{
	Vector4 temp;
	temp.x = x * s;
	temp.y = y * s;
	temp.z = z * s;
	temp.w = w;
	return temp;
}

/**
*	Vector scalar division.  Enables mulitiplication by 1/scalar in a more logical manner.
*	\param s	Operand to scale by.
*	\note		Homogenous coordinate is not scaled.
*	\return		Scaled vector.
*	\callergraph
*	\callgraph
*/
Vector4 Vector4::operator/(const float s) const
{
	Vector4 temp;
	temp.x = x / s;
	temp.y = y / s;
	temp.z = z / s;
	temp.w = w;
	return temp;
}

/**
*	Vector subtraction.
*	\param v	Operand to be subtracted.
*	\return		Vector pointing from <i>(*this)</i> to operand v.
*	\callergraph
*	\callgraph
*/
Vector4	Vector4::operator-(const Vector4 v) const
{
	Vector4 temp;
	temp.x = x - v.x;
	temp.y = y - v.y;
	temp.z = z - v.z;
	temp.w = w - v.w;
	return temp;
}

/**
*	Vector unary negation.
*	\return		<i>(*this)</i> negated.
*	\callergraph
*	\callgraph
*/
Vector4	Vector4::operator-() const
{
	Vector4 temp;
	temp.x = -x;
	temp.y = -y;
	temp.z = -z;
	temp.w = w;
	return temp;
}

/**
*	Dot product.
*	\param	v Operand to dot with.
*	\note	You can use the dot product to project vector v onto <i>(*this)</i>.  To do this, simply perform:
*			<i>projection = Normalized((*this)) * [(*this) * v]</i>  Where the first operation 
*			[Vector4 * Vector4] returns a scalar which is used to scale normalzied this.
*	\return	Scalar dot product.
*	\callergraph
*	\callgraph
*/
float Vector4::operator*(const Vector4 v) const
{
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

/**
*	Vector equality.  True if component-wise equality is true.
*	\param	v Operand to check equality against.
*	\return	Equality test result.
*	\callergraph
*	\callgraph
*/
bool Vector4::operator==(const Vector4 v) const
{
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

/**
*	Vector in-equality.  True if component-wise equality is <i>not</i> true.
*	\param	v Operand to check in-equality against.
*	\return	In-equality test result.
*	\callergraph
*	\callgraph
*/
bool Vector4::operator!=(const Vector4& v) const
{
	return !(*this == v);
}

/**
*	Vector cross product.  Assumes handedness of \#define GL.
*	\param	v Operand to cross against.
*	\note	The cross product is typically used to produce a normal to a surface(plane) which is spanned by
*			(*this) and v.  The input vectors need not be normalized, and the resulting vector is also not
*			normalized.  The direction the vector points (up from the surface or down from the surface)
*			depends upon the handedness of the coordinate system.
*	\return	Vector result of cross product operation.
*	\callergraph
*	\callgraph
*/
Vector4	Vector4::operator%(const Vector4& v) const
{
	Vector4 temp;
	temp.x = y*v.z - z*v.y;
	temp.y = z*v.x - x*v.z;
	temp.z = x*v.y - y*v.x;
	return temp;
}

/**
*	Vector addition and assignment.
*	\param	v Operand to add.
*	\return	void.
*	\callergraph
*	\callgraph
*/
void Vector4::operator+=(const Vector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}

/**
*	Vector scalar multiplication and assignment.
*	\param	s Operand to scale <i>(*this)</i> by.
*	\return	void.
*	\callergraph
*	\callgraph
*/
void Vector4::operator*=(const float s)
{
	x *= s;
	y *= s;
	z *= s;
}

/**
*	Vector scalar division and assignment.
*	\param	s Operand to divide <i>(*this)</i> by.
*	\return	void.
*	\callergraph
*	\callgraph
*/
void Vector4::operator/=(const float s)
{
	x /= s;
	y /= s;
	z /= s;
}

/**
*	Vector subtraction and assignment.
*	\param	v Operand to subtract by.
*	\return	void.
*	\callergraph
*	\callgraph
*/
void Vector4::operator-=(const Vector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

/**
*	Vector assignment.
*	\param	v Operand to be set equal to.
*	\return	void.
*	\callergraph
*	\callgraph
*/
void Vector4::operator=(const Vector4& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}
