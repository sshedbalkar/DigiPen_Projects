///////////////////////////////////////////////////////////////////////
// $Id$
//
// Geometric objects (Points, Vectors, Planes, ...) and operations.
//
// Gary Herron
//
// Copyright © 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include "geomlib.h"
#include <vector>

float ToRadians(const float& Degrees)
{
    const float ret = (PI*Degrees)/180.0f;
    return ret;
}

// Test any STL sequence for all elements zero
template<class Sequence> bool IsZero(const Sequence& s)
{
    for(typename Sequence::const_iterator i = s.begin(); i != s.end(); ++i)
        if(0 != *i)
            return false;
    return true;
}


////////////////////////////////////////////////////////////////////////
// Color methods and functions
////////////////////////////////////////////////////////////////////////
Color::Color(const float r, const float g,
			 const float b, const float a)
{
	Set(r,g,b,a);
}

void Color::Set(float r, float g, float b, float a)
{
	rgba[0] = r;
	rgba[1] = g;
	rgba[2] = b;
	rgba[3] = a;
}
	
Color operator+(const Color& C, const Color& D)
{
	return Color(C[0] + D[0], C[1] + D[1], C[2] + D[2], C[3] + D[3]);
}

Color operator*(const Color& C, const float t)
{
	return Color(C[0]*t, C[1]*t, C[2]*t, C[3]*t);
}

Color operator*(const float t, const Color& C)
{
	return Color(C[0]*t, C[1]*t, C[2]*t, C[3]*t);
}

Color operator*(const Color& C, const Color& D)
{
	return Color(C[0]*D[0], C[1]*D[1], C[2]*D[2], C[3]*D[3]);
}

HSVColor::HSVColor(float h, float s, float v, float a)
{
	if (s == 0.0)
		Set(v,v,v,a);
	else {
		int i = (int)(h*6.0);
		float f = (h*6.0) - i;
		float p = v*(1.0 - s);
		float q = v*(1.0 - s*f);
		float t = v*(1.0 - s*(1.0-f));
		if (i%6 == 0)
			Set(v,t,p,a);
		else if (i == 1)
			Set(q,v,p,a);
		else if (i == 2)
			Set(p,v,t,a);
		else if (i == 3)
			Set(p,q,v,a);
		else if (i == 4)
			Set(t,p,v,a);
		else if (i == 5)
			Set(v,p,q,a); }
}

////////////////////////////////////////////////////////////////////////
// In-place operations
////////////////////////////////////////////////////////////////////////

// Vector2D In-place operations
Vector2D& Vector2D::operator+=(const Vector2D& rhs)
{
    std::transform(begin(), end(), rhs.begin(), begin(),
        std::plus<float>());
    return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& rhs)
{
    std::transform(begin(), end(), rhs.begin(), begin(),
        std::minus<float>());
    return *this;
}

Vector2D& Vector2D::operator*=(const float& rhs)
{
    std::transform(begin(), end(), begin(),
        std::bind2nd(std::multiplies<float>(), rhs));
    return *this;
}

Vector2D& Vector2D::operator/=(const float& rhs)
{
    std::transform(begin(), end(), begin(),
        std::bind2nd(std::divides<float>(), rhs));
    return *this;
}

void Vector2D::normalize() throw(ZeroDivide)
{
    const float len = length();
    if(!len) throw ZeroDivide();
    *this /= len;
}


// Vector3D In-place operations
Vector3D& Vector3D::operator+=(const Vector3D& rhs)
{
    std::transform(begin(), end(), rhs.begin(), begin(),
				   std::plus<float>());
    return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D& rhs)
{
    std::transform(begin(), end(), rhs.begin(), begin(),
				   std::minus<float>());
    return *this;
}

Vector3D& Vector3D::operator*=(const float& rhs)
{
    std::transform(begin(), end(), begin(),
        std::bind2nd(std::multiplies<float>(), rhs));
    return *this;
}

Vector3D& Vector3D::operator/=(const float& rhs)
{
    std::transform(begin(), end(), begin(),
        std::bind2nd(std::divides<float>(), rhs));
    return *this;
}

void Vector3D::normalize() throw(ZeroDivide)
{
    const float len = length();
    if(!len) throw ZeroDivide();
    *this /= len;
}


// Vector4D In-place operations
Vector4D& Vector4D::operator+=(const Vector4D& rhs)
{
    std::transform(begin(), end(), rhs.begin(), begin(),
        std::plus<float>());
    return *this;
}

Vector4D& Vector4D::operator-=(const Vector4D& rhs)
{
    std::transform(begin(), end(), rhs.begin(), begin(),
				   std::minus<float>());
    return *this;
}

Vector4D& Vector4D::operator*=(const float& rhs)
{
    std::transform(begin(), end(), begin(),
        std::bind2nd(std::multiplies<float>(), rhs));
    return *this;
}

Vector4D& Vector4D::operator/=(const float& rhs)
{
    std::transform(begin(), end(), begin(),
        std::bind2nd(std::divides<float>(), rhs));
    return *this;
}

void Vector4D::normalize() throw(ZeroDivide)
{
    const float len = length();
    if(!len) throw ZeroDivide();
    *this /= len;
}


////////////////////////////////////////////////////////////////////////
// Various operations
////////////////////////////////////////////////////////////////////////

// Vector* length
float Vector2D::length() const
{
    const float ret = sqrt(Dot(*this, *this));
    return ret;
}

float Vector3D::length() const
{
    const float ret = sqrt(Dot(*this, *this));
    return ret;
}

float Vector4D::length() const
{
    const float ret = sqrt(Dot(*this, *this));
    return ret;
}

// Vector* normalized()
Vector2D Vector2D::normalized() const throw(ZeroDivide)
{
    const float len = length();
    if(!len) throw ZeroDivide();
    Vector2D ret(*this);
    return ret /= len;
}

Vector3D Vector3D::normalized() const throw(ZeroDivide)
{
    const float len = length();
    if(!len) throw ZeroDivide();
    Vector3D ret(*this);
    return ret /= len;
}

Vector4D Vector4D::normalized() const throw(ZeroDivide)
{
    const float len = length();
    if(!len) throw ZeroDivide();
    Vector4D ret(*this);
    return ret /= len;
}

// Cross product (Vector3D only)
Vector3D Cross(const Vector3D& v1, const Vector3D& v2)
{
    Vector3D ret;

        ret[0] = v1[1]*v2[2] - v1[2]*v2[1];
        ret[1] = v1[2]*v2[0] - v1[0]*v2[2];
        ret[2] = v1[0]*v2[1] - v1[1]*v2[0];

    return ret;
}

// Homogeneous division of a Vector4D to a Point3D
Point3D Vector4D::Hdiv() throw(ZeroDivide)
{
	if (!crds[3]) throw ZeroDivide();
	return Point3D(crds[0]/crds[3],
				   crds[1]/crds[3],
				   crds[2]/crds[3]);
}


////////////////////////////////////////////////////////////////////////
// Arithmetic operations
////////////////////////////////////////////////////////////////////////

// Vector2D operations
bool operator==(const Vector2D& lhs, const Vector2D& rhs)
{
    const bool ret = std::equal(lhs.begin(), lhs.end(), rhs.begin());
    return ret;
}

bool operator!=(const Vector2D& lhs, const Vector2D& rhs)
{
    const bool ret = !(lhs == rhs);
    return ret;
}

Vector2D operator+(const Vector2D& lhs, const Vector2D& rhs)
{
    Vector2D ret(lhs);
    ret += rhs;
    return ret;
}

Vector2D operator-(const Vector2D& lhs, const Vector2D& rhs)
{
    Vector2D ret(lhs);
    ret -= rhs;
    return ret;
}

Vector2D operator*(const Vector2D& lhs, const float& rhs)
{
    Vector2D ret(lhs);
    ret *= rhs;
    return ret;
}

Vector2D operator*(const float& lhs, const Vector2D& rhs)
{
    return rhs*lhs;
}

Vector2D operator/(const Vector2D& lhs, const float& rhs)
{
    Vector2D ret(lhs);
    ret /= rhs;
    return ret;
}

float Dot(const Vector2D& lhs, const Vector2D& rhs)
{
    const float ret = std::inner_product(lhs.begin(), lhs.end(), rhs.begin(),
        float(0));
    return ret;
}


// Vector3D operations
bool operator==(const Vector3D& lhs, const Vector3D& rhs)
{
    const bool ret = std::equal(lhs.begin(), lhs.end(), rhs.begin());
    return ret;
}

bool operator!=(const Vector3D& lhs, const Vector3D& rhs)
{
    const bool ret = !(lhs == rhs);
    return ret;
}

Vector3D operator+(const Vector3D& lhs, const Vector3D& rhs)
{
    Vector3D ret(lhs);
    ret += rhs;
    return ret;
}

Vector3D operator-(const Vector3D& lhs, const Vector3D& rhs)
{
    Vector3D ret(lhs);
    ret -= rhs;
    return ret;
}

Vector3D operator*(const Vector3D& lhs, const float& rhs)
{
    Vector3D ret(lhs);
    ret *= rhs;
    return ret;
}

Vector3D operator*(const float& lhs, const Vector3D& rhs)
{
    return rhs*lhs;
}

Vector3D operator/(const Vector3D& lhs, const float& rhs)
{
    Vector3D ret(lhs);
    ret /= rhs;
    return ret;
}

float Dot(const Vector3D& lhs, const Vector3D& rhs)
{
    return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
}


// Vector4D operations
bool operator==(const Vector4D& lhs, const Vector4D& rhs)
{
    const bool ret = std::equal(lhs.begin(), lhs.end(), rhs.begin());
    return ret;
}

bool operator!=(const Vector4D& lhs, const Vector4D& rhs)
{
    const bool ret = !(lhs == rhs);

    return ret;
}

Vector4D operator+(const Vector4D& lhs, const Vector4D& rhs)
{
    Vector4D ret(lhs);
    ret += rhs;
    return ret;
}

Vector4D operator-(const Vector4D& lhs, const Vector4D& rhs)
{
    Vector4D ret(lhs);
    ret -= rhs;
    return ret;
}

Vector4D operator*(const Vector4D& lhs, const float& rhs)
{
    Vector4D ret(lhs);
    ret *= rhs;
    return ret;
}

Vector4D operator/(const Vector4D& lhs, const float& rhs)
{
    Vector4D ret(lhs);
    ret /= rhs;
    return ret;
}

Vector4D operator*(const float& lhs, const Vector4D& rhs)
{
    return rhs*lhs;
}

float Dot(const Vector4D& lhs, const Vector4D& rhs)
{
    const float ret = std::inner_product(lhs.begin(), lhs.end(), rhs.begin(),
        float(0));
    return ret;
}


bool Coplanar(const Point3D& A,const Point3D& B,
			  const Point3D& C, const Point3D& D)
{
	// Generated with maxima as the determinant of the 4x4:
	// | A[0] A[1] A[2] 1 |
    // |                  |
	// | B[0] B[1] B[2] 1 |
    // |                  |
	// | C[0] C[1] C[2] 1 |
    // |                  |
	// | D[0] D[1] D[2] 1 |
	float det = 
		- B[0]*C[1]*D[2] + A[0]*C[1]*D[2] + B[1]*C[0]*D[2] - A[1]*C[0]*D[2] 
		- A[0]*B[1]*D[2] + A[1]*B[0]*D[2] + B[0]*C[2]*D[1] - A[0]*C[2]*D[1] 
		- B[2]*C[0]*D[1] + A[2]*C[0]*D[1] + A[0]*B[2]*D[1] - A[2]*B[0]*D[1] 
		- B[1]*C[2]*D[0] + A[1]*C[2]*D[0] + B[2]*C[1]*D[0] - A[2]*C[1]*D[0] 
		- A[1]*B[2]*D[0] + A[2]*B[1]*D[0] + A[0]*B[1]*C[2] - A[1]*B[0]*C[2] 
		- A[0]*B[2]*C[1] + A[2]*B[0]*C[1] + A[1]*B[2]*C[0] - A[2]*B[1]*C[0];

	return fabs(det) < 1.0e-3;
}
