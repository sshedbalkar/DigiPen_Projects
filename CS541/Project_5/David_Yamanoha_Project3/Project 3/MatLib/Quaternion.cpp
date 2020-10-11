#include "Quaternion.h"
#include <math.h>

Quaternion::Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w){}

void Quaternion::Set(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

float Quaternion::Dot(const Quaternion& q) const
{
	return x * q.x + y * q.y + z * q.z + w * q.w;
}

void Quaternion::ComputeW()
{
	float t = 1.0f - x*x - y*y - z*z;

	if(t < 0.0f)
		w = 0.0f;
	else
		w = -sqrt(t);
}

float Quaternion::Magnitude()
{
	return sqrt(x*x + y*y + z*z + w*w);
}

void Quaternion::Normalize()
{
	float m = Magnitude();

	if(m > 0.0f)
	{
		x /= m;
		y /= m;
		z /= m;
		w /= m;
    }
}

void Quaternion::operator=(const Quaternion& q)
{
	x = q.x;
	y = q.y;
	z = q.z;
	w = q.w;
}

bool Quaternion::operator==(const Quaternion& q) const
{
	if(x == q.x && y == q.y && z == q.z && w == q.w)
		return true;
	return false;
}

bool Quaternion::operator!=(const Quaternion& q) const
{
	return !((*this) == q);
}

Quaternion Quaternion::operator-(const Quaternion& q) const
{
	Quaternion result;
	result.x = x - q.x;
	result.y = y - q.y;
	result.z = z - q.z;
	result.w = w - q.w;
	return result;
}

Quaternion Quaternion::operator+(const Quaternion& q) const
{
	Quaternion result;
	result.x = x + q.x;
	result.y = y + q.y;
	result.z = z + q.z;
	result.w = w + q.w;
	return result;
}

Quaternion Quaternion::operator-() const
{
	Quaternion temp;
	temp.x = -x;
	temp.y = -y;
	temp.z = -z;
	temp.w = w;
	return temp;
}

Quaternion Quaternion::operator *(const Quaternion& q) const
{
	Quaternion temp;
	temp.x = x*q.w + w*q.x + y*q.z - z*q.y;
	temp.y = y*q.w + w*q.y + z*q.x - x*q.z;
	temp.z = z*q.w + w*q.z + x*q.y - y*q.x;
	temp.w = w*q.w - x*q.x - y*q.y - z*q.z;
	return temp;
}

Quaternion Quaternion::operator *(const Vector4& v) const
{
	Quaternion temp;
	temp.x = w*v.x + y*v.z - z*v.y;
	temp.y = w*v.y + z*v.x - x*v.z;
	temp.z = w*v.z + x*v.y - y*v.x;
	temp.w = -(x*v.x) - (y*v.y) - (z*v.z);
	return temp;
}

Quaternion Quaternion::operator*(const float f) const
{
	Quaternion q;
	q.x = x * f;
	q.y = y * f;
	q.z = z * f;
	q.w = w * f;
	return q;
}

Quaternion Quaternion::operator/(const float f) const
{
	Quaternion q;
	q.x = x / f;
	q.y = y / f;
	q.z = z / f;
	q.w = w / f;
	return q;
}

Vector4 Quaternion::RotatePoint(const Vector4& v) const
{
	Quaternion inverse = -(*this);
	inverse.Normalize();

	Quaternion temp = (*this) * v;
	Quaternion final = temp * inverse;

	Vector4 vFinal(final.x, final.y, final.z, 1.0f);

	return vFinal;
}

Quaternion Quaternion::Slerp(const Quaternion& a, const Quaternion& b, float t)
{
	if( t <= 0.0f )
		return a;
	if( t >= 1.0f )
		return b;
	
	float cosOmega = a.Dot(b);

	Quaternion bCpy = b;
	if(cosOmega < 0.0f)
	{
		bCpy = -bCpy;
		bCpy.w = -bCpy.w;
		cosOmega = -cosOmega;
	}

	float k0, k1;
	if (cosOmega > 0.9999f)
    {
		k0 = 1.0f - t;
		k1 = t;
	}
	else
    {
		float sinOmega = sqrt (1.0f - (cosOmega * cosOmega));
		float omega = atan2 (sinOmega, cosOmega);
		float oneOverSinOmega = 1.0f / sinOmega;
		k0 = sin ((1.0f - t) * omega) * oneOverSinOmega;
		k1 = sin (t * omega) * oneOverSinOmega;
    }
	
	Quaternion result;
	result.Set(		(k0 * a.x) + (k1 * bCpy.x), (k0 * a.y) + (k1 * bCpy.y),
					(k0 * a.z) + (k1 * bCpy.z), (k0 * a.w) + (k1 * bCpy.w)	);
	return result;
}