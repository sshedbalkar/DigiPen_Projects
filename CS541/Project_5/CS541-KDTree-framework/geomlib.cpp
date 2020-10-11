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
#include <cfloat>

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

unsigned char Color::R() const
{
  return static_cast<char>(rgba[0] * 255.f);
}

unsigned char Color::G() const
{
  return static_cast<char>(rgba[1] * 255.f);
}

unsigned char Color::B() const
{
  return static_cast<char>(rgba[2] * 255.f);
}

const Color& Color::operator*=( float rhs )
{
  rgba[0] = rgba[0] * rhs;
  rgba[1] = rgba[1] * rhs;
  rgba[2] = rgba[2] * rhs;
  rgba[3] = rgba[3] * rhs;
  return *this;
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
		float f = (h*6.0f) - i;
		float p = v*(1.0f - s);
		float q = v*(1.0f - s*f);
		float t = v*(1.0f - s*(1.0f-f));
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

void Vector2D::normalize()
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

void Vector3D::normalize()
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

void Vector4D::normalize()
{
    const float len = length();
    if(!len) throw ZeroDivide();
    *this /= len;
}


// Matrix4x4 In-place operations
Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& rhs)
{
	Matrix4x4 temp;

	const Vector4D& A = (*this)[0];
	const Vector4D& B = (*this)[1];
	const Vector4D& C = (*this)[2];
	const Vector4D& D = (*this)[3];

	temp[0][0] = rhs[0][0]*A[0]+rhs[1][0]*A[1]+rhs[2][0]*A[2]+rhs[3][0]*A[3];
	temp[1][0] = rhs[0][0]*B[0]+rhs[1][0]*B[1]+rhs[2][0]*B[2]+rhs[3][0]*B[3];
	temp[2][0] = rhs[0][0]*C[0]+rhs[1][0]*C[1]+rhs[2][0]*C[2]+rhs[3][0]*C[3];
	temp[3][0] = rhs[0][0]*D[0]+rhs[1][0]*D[1]+rhs[2][0]*D[2]+rhs[3][0]*D[3];

	temp[0][1] = rhs[0][1]*A[0]+rhs[1][1]*A[1]+rhs[2][1]*A[2]+rhs[3][1]*A[3];
	temp[1][1] = rhs[0][1]*B[0]+rhs[1][1]*B[1]+rhs[2][1]*B[2]+rhs[3][1]*B[3];
	temp[2][1] = rhs[0][1]*C[0]+rhs[1][1]*C[1]+rhs[2][1]*C[2]+rhs[3][1]*C[3];
	temp[3][1] = rhs[0][1]*D[0]+rhs[1][1]*D[1]+rhs[2][1]*D[2]+rhs[3][1]*D[3];

	temp[0][2] = rhs[0][2]*A[0]+rhs[1][2]*A[1]+rhs[2][2]*A[2]+rhs[3][2]*A[3];
	temp[1][2] = rhs[0][2]*B[0]+rhs[1][2]*B[1]+rhs[2][2]*B[2]+rhs[3][2]*B[3];
	temp[2][2] = rhs[0][2]*C[0]+rhs[1][2]*C[1]+rhs[2][2]*C[2]+rhs[3][2]*C[3];
	temp[3][2] = rhs[0][2]*D[0]+rhs[1][2]*D[1]+rhs[2][2]*D[2]+rhs[3][2]*D[3];

	temp[0][3] = rhs[0][3]*A[0]+rhs[1][3]*A[1]+rhs[2][3]*A[2]+rhs[3][3]*A[3];
	temp[1][3] = rhs[0][3]*B[0]+rhs[1][3]*B[1]+rhs[2][3]*B[2]+rhs[3][3]*B[3];
	temp[2][3] = rhs[0][3]*C[0]+rhs[1][3]*C[1]+rhs[2][3]*C[2]+rhs[3][3]*C[3];
	temp[3][3] = rhs[0][3]*D[0]+rhs[1][3]*D[1]+rhs[2][3]*D[2]+rhs[3][3]*D[3];
	
	return *this = temp;
}

void Matrix4x4::reset()
{
	*this = Matrix4x4();
    return;
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
Vector2D Vector2D::normalized() const
{
    const float len = length();
    if(!len) throw ZeroDivide();
    Vector2D ret(*this);
    return ret /= len;
}

Vector3D Vector3D::normalized() const
{
    const float len = length();
    if(!len) throw ZeroDivide();
    Vector3D ret(*this);
    return ret /= len;
}

float Vector3D::lengthSquared() const
{
  return Dot(*this, *this);
}

Vector4D Vector4D::normalized() const
{
    const float len = length();
    if(!len) throw ZeroDivide();
    Vector4D ret(*this);
    return ret /= len;
}

// Cross product (Vector3D only)
Vector3D Cross(const Vector3D& v1, const Vector3D& v2)
{
    return Vector3D(v1[1]*v2[2] - v1[2]*v2[1],
                    v1[2]*v2[0] - v1[0]*v2[2],
                    v1[0]*v2[1] - v1[1]*v2[0]);
}

// Homogeneous division of a Vector4D to a Point3D
Point3D Vector4D::Hdiv()
{
	if (!crds[3]) throw ZeroDivide();
	return Point3D(crds[0]/crds[3],
				   crds[1]/crds[3],
				   crds[2]/crds[3]);
}

Vector3D Vector4D::ToVec3() const
{
  return Vector3D(crds[0], crds[1], crds[2]);
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
  return Vector2D(lhs[0] + rhs[0], lhs[1] + rhs[1]);
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
    return (lhs[0] * rhs[0] + lhs[1] * rhs[1]);
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
  return Vector3D(lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2]);
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
  return Vector4D(lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2], lhs[3] + lhs[3]);
}

Vector4D operator-(const Vector4D& lhs, const Vector4D& rhs)
{
    Vector4D ret(lhs);
    ret -= rhs;
    return ret;
}

Vector4D operator*(const Vector4D& lhs, const float& rhs)
{
  return Vector4D(lhs[0] * rhs, lhs[1] * rhs, lhs[2] * rhs, lhs[3] * rhs);
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


// Matrix4x4 operations
Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs)
{
    Matrix4x4 ret(lhs);
    ret *= rhs;
    return ret;
}

Vector4D operator*(const Matrix4x4& lhs, const Vector3D& rhs)
{
    Vector4D temp(rhs[0], rhs[1], rhs[2], 0);
    temp = lhs*temp;
    return temp;
}

Vector4D operator*(const Matrix4x4& lhs, const Vector4D& rhs)
{
    Vector4D ret;
	ret[0] = Dot(lhs[0], rhs);
	ret[1] = Dot(lhs[1], rhs);
	ret[2] = Dot(lhs[2], rhs);
	ret[3] = Dot(lhs[3], rhs);

    return ret;
}

Matrix4x4 LookAt(const Point3D& C, const Point3D& D, const Vector3D& U)
{
	//look z, y is up, x to the right

	Vector3D target = (D - C).normalized();
	Vector3D X = Cross(target, U).normalized();
	Vector3D newU = Cross(X, target);
	Vector3D CV = C - Point3D(0,0,0);

	Matrix4x4 ret;

	ret[0] = Vector4D(X[0], X[1], X[2], -Dot(X, CV));
	ret[1] = Vector4D(newU[0], newU[1], newU[2], -Dot(newU, CV));
	ret[2] = Vector4D(-target[0], -target[1], -target[2], Dot(target, CV));
	ret[3] = Vector4D(0        , 0       , 0   , 1       );

	return ret;
}

Matrix4x4 Perspective(const float w, const float h,
					  const float d, const float f)
{
    Matrix4x4 ret;

	ret[0] = Vector4D(d/w,   0,       0,          0);
	ret[1] = Vector4D(  0, d/h,       0,          0);
	ret[2] = Vector4D(  0,   0, f/(f-d), -d*f/(f-d));
	ret[3] = Vector4D(  0,   0,       1,          0);

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

float sign( float f )
{
  if (f < 0.f) return -1.f;
  else return 1.f;
}

float Plane3D::D() const
{
  return crds[3];
}

Point3D* Triangle3D::GetArray()
{
  return points;
}

void Triangle3D::MakeAABB()
{
  float lx = -FLT_MAX;
  float ly = -FLT_MAX;
  float lz = -FLT_MAX;

  float sx = FLT_MAX;
  float sy = FLT_MAX;
  float sz = FLT_MAX;

  for (unsigned i = 0; i < 3; ++i)
  {
    if (points[i][0] < sx) sx = points[i][0];
    if (points[i][1] < sy) sy = points[i][1];
    if (points[i][2] < sz) sz = points[i][2];

    if (points[i][0] > lx) lx = points[i][0];
    if (points[i][1] > ly) ly = points[i][1];
    if (points[i][2] > lz) lz = points[i][2];
  }

  m_aabb.m_min = Vector3D(sx, sy, sz);
  m_aabb.m_max = Vector3D(lx, ly, lz);
}

AABB::AABB( Vector3D minPt, Vector3D maxPt ) : m_min(minPt), m_max(maxPt)
{

}

AABB::AABB()
{

}

float AABB::GetSurfaceArea() const
{
  float height = m_max[0] - m_min[0];
  float width = m_max[1] - m_min[1];
  float len = m_max[2] - m_max[2];

  return 2.f * height * width + 2.f * height * len + 2.f * width * len;
}
