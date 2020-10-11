///////////////////////////////////////////////////////////////////////
// $Id$
//
// Geometric objects (Points, Vectors, Planes, ...) and operations.
//
// Gary Herron
//
// Copyright © 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#if !defined(_GEOMLIB_H)
#define _GEOMLIB_H

#include <algorithm> // transform
#include <cmath> // sqrt
#include <functional> // bind2nd, plus, minus, etc.
#include <iostream>
#include <numeric> // inner_product
#include <sstream> // stringstream
#include <utility> // pair

#define PI (3.14159f)

#ifndef _WIN32
#include <values.h>
#define abs fabs
#endif

struct ZeroDivide {};          // Used to signal divide-by-zero errors

struct Unimplemented {};       // Marks code to be implemented by students.

// This may be a bad idea.  Previously, Point?D and Vector?D were
// separate types, each with their own methods.  Due to student
// requests, the distinction has been done away with.  Point?D is now
// a synonym for Vector?D, and the methods of the Point?D classes have
// been moved to the corresponding Vector?D class.
#define Point2D Vector2D
#define Point3D Vector3D

// Forward declarations:
class Vector2D;
class Vector3D;
class Vector4D;

class Line3D;
class Box3D;
class Sphere3D;
class Plane3D;
class Triangle3D;

////////////////////////////////////////////////////////////////////////
// Class Color represents a color with a 4-vector of floats for red,
// green, blue, and alpha.  It supports indexing operations, a Set
// method, and several component wise operations. 
//
// There is also a sub-class called HSVColor which provides a
// constructor for a Color to be specified via hue, saturation and
// value parameters.
////////////////////////////////////////////////////////////////////////
class Color
{
public:
    Color(const float r=0.0, const float g=0.0,
          const float b=0.0, const float a=1.0);

    void Set(float r=0.0, float g=0.0, float b=0.0, float a=1.0);
    
    // Indexing operators.
    float& operator[](const unsigned int i) {return rgba[i];};
    inline float operator[](const unsigned int i) const {return rgba[i];}

    // Sequential-access methods.
    typedef float *iterator;
    typedef const float *const_iterator;
    iterator begin() { return rgba + 0; }
    const_iterator begin() const { return rgba + 0; }
    iterator end() { return rgba + DIM; }
    const_iterator end() const { return rgba + DIM; }

    unsigned char R() const;
    
    unsigned char G() const;

    unsigned char B() const;

    const Color& operator*=(float rhs);



    enum {DIM=4};
    float rgba[DIM];
};

Color operator+(const Color& C, const Color& D);
Color operator*(const Color& C, const float t);
Color operator*(const float t, const Color& C);
Color operator*(const Color& C, const Color& D);

class HSVColor : public Color
{
public:
    HSVColor(float h=0.0, float s=0.0, float v=0.0, float a=1.0);
};

////////////////////////////////////////////////////////////////////////
// Vector2D
////////////////////////////////////////////////////////////////////////
class Vector2D
{
public:
    // Constructor
    Vector2D(const float X=0, const float Y=0) { crds[0]=X; crds[1]=Y; };

    // In-place operators.
    Vector2D& operator+=(const Vector2D& rhs); 
    Vector2D& operator-=(const Vector2D& rhs); 
    Vector2D& operator*=(const float& rhs); 
    Vector2D& operator/=(const float& rhs); 
    void normalize();

    // Indexing operators.
    float& operator[](const unsigned int i) { return crds[i]; }
    float operator[](const unsigned int i) const { return crds[i]; }

    // Sequential-access methods.
    typedef float *iterator;
    typedef const float *const_iterator;
    iterator begin() { return crds + 0; }; 
    const_iterator begin() const { return crds + 0; }
    iterator end() { return crds + DIM; }
    const_iterator end() const { return crds + DIM; }

    // Utility methods.
    float length() const; 
    Vector2D normalized() const; 

private:
    enum {DIM=2};
    float crds[DIM];
};

// Operators
bool operator==(const Vector2D& lhs, const Vector2D& rhs); 
bool operator!=(const Vector2D& lhs, const Vector2D& rhs); 
Vector2D operator+(const Vector2D& lhs, const Vector2D& rhs); 
Vector2D operator-(const Vector2D& lhs, const Vector2D& rhs); 
Vector2D operator*(const Vector2D& lhs, const float& rhs); 
Vector2D operator*(const float& lhs, const Vector2D& rhs); 
Vector2D operator/(const Vector2D& lhs, const float& rhs); 
float Dot(const Vector2D& vec1, const Vector2D& vec2);

// Point2D/Vector2D operators
Point2D operator+(const Point2D& lhs, const Vector2D& rhs);
Point2D operator+(const Vector2D& lhs, const Point2D& rhs);
Point2D operator-(const Point2D& lhs, const Vector2D& rhs);
Vector2D operator-(const Point2D& lhs, const Point2D& rhs);

// Stream operator
template<class charT, class traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& lhs, const Vector2D& rhs)
{
    std::basic_ostringstream<charT, traits> s;
    s.flags(lhs.flags());
    s.imbue(lhs.getloc());
    s.precision(lhs.precision());

    typename Vector2D::const_iterator i = rhs.begin();
    s <<'(' <<*i;

    for(++i; i != rhs.end(); ++i)
        s <<',' <<*i;
    s <<')';

    return lhs <<s.str();
}

////////////////////////////////////////////////////////////////////////
// Line2D
////////////////////////////////////////////////////////////////////////
class Line2D
{
public:
    // Constructors
    Line2D() : point(), vector() {return;}
    Line2D(const Point2D& p, const Vector2D& v): point(p), vector(v) {return;}

    Point2D point;
    Vector2D vector;
};

// Functions
bool Intersects(const Line2D& line1, const Line2D& line2,
                Point2D *rpoint=0);


////////////////////////////////////////////////////////////////////////
// Segment2D
////////////////////////////////////////////////////////////////////////
class Segment2D
{
public:
    // Constructor
    Segment2D() : point1(), point2() {return;}

    Segment2D(const Point2D& p1, const Point2D& p2)
        : point1(p1), point2(p2) {return;}


    Point2D point1;
    Point2D point2;
};

// Functions
bool Intersects(const Segment2D& seg1, const Segment2D& seg2,
                Point2D *rpoint=0);


////////////////////////////////////////////////////////////////////////
// Vector3D
////////////////////////////////////////////////////////////////////////
class Vector3D
{
public:
    // Constructor
    Vector3D(const float X=0, const float Y=0, const float Z=0)
        { crds[0]=X; crds[1]=Y; crds[2]=Z; };

    // In-place operators.
    Vector3D& operator+=(const Vector3D& rhs); 
    Vector3D& operator-=(const Vector3D& rhs); 
    Vector3D& operator*=(const float& rhs); 
    Vector3D& operator/=(const float& rhs);
    void normalize();

    // Indexing operators.
    float& operator[](const unsigned int i) { return crds[i]; }
	float operator[](const unsigned int i) const { return crds[i]; }

    // Utility methods.
    float length() const; 
    float lengthSquared() const;
    Vector3D normalized() const;

    // Sequential-access methods.
    typedef float *iterator;
    typedef const float *const_iterator;
    iterator begin() { return crds + 0; }
    const_iterator begin() const { return crds + 0; }
    iterator end() { return crds + DIM; }
    const_iterator end() const { return crds + DIM; }

private:
    enum {DIM=3};
    float crds[DIM];
};

// Utility functions:
float Distance(const Point3D& point, const Line3D& line); 
float Distance(const Point3D& point, const Plane3D& plane);
bool Coplanar(const Point3D& A,const Point3D& B,
              const Point3D& C, const Point3D& D); 

// Operators
bool operator==(const Vector3D& lhs, const Vector3D& rhs);
bool operator!=(const Vector3D& lhs, const Vector3D& rhs);
Vector3D operator+(const Vector3D& lhs, const Vector3D& rhs);
Vector3D operator-(const Vector3D& lhs, const Vector3D& rhs);
Vector3D operator*(const Vector3D& lhs, const float& rhs);
Vector3D operator*(const float& lhs, const Vector3D& rhs);
Vector3D operator/(const Vector3D& lhs, const float& rhs);

// Functions
float Dot(const Vector3D& vec1, const Vector3D& vec2); 
Vector3D Cross(const Vector3D& vec1, const Vector3D& vec2);

// Stream operator
template<class charT, class traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& lhs, const Vector3D& rhs)
{
    std::basic_ostringstream<charT, traits> s;
    s.flags(lhs.flags());
    s.imbue(lhs.getloc());
    s.precision(lhs.precision());

    typename Vector3D::const_iterator i = rhs.begin();
    s <<'(' <<*i;

    for(++i; i != rhs.end(); ++i) {
        s <<',' <<*i; }
    s <<')';

    return lhs <<s.str();
}


////////////////////////////////////////////////////////////////////////
// Line3D
////////////////////////////////////////////////////////////////////////
class Line3D
{
public:
    // Constructors
    Line3D() : point(), vector() {return;}
    Line3D(const Point3D& p, const Vector3D& v) : point(p),vector(v) {return;}

public:
    Point3D point;
    Vector3D vector;
};

// Functions
float sign(float f);
float AngleBetween(const Line3D& line1, const Line3D& line2); 
bool Coplanar(const Line3D& line1, const Line3D& line2); 
bool Parallel(const Line3D& line1, const Line3D& line2); 
bool Perpendicular(const Line3D& line1, const Line3D& line2);
 
float AngleBetween(const Line3D& line, const Plane3D& plane); 
bool Parallel(const Line3D& line, const Plane3D& plane); 
bool Perpendicular(const Line3D& line, const Plane3D& plane); 
bool Intersects(const Line3D& line, const Plane3D& plane, Point3D *rpoint=0); 

////////////////////////////////////////////////////////////////////////
// Segment3D
////////////////////////////////////////////////////////////////////////
class Segment3D
{
public:
    // Constructors
    Segment3D() : point1(), point2() {return;}
    Segment3D(const Point3D& p1, const Point3D& p2)
        : point1(p1), point2(p2) {return;}

    // Utility methods
    bool contains(const Point3D& point) const;

    Point3D point1;
    Point3D point2;
};

// Functions
bool Intersects(const Segment3D& seg, const Triangle3D& tri, Point3D *rpoint=0);


////////////////////////////////////////////////////////////////////////
// Ray3D
////////////////////////////////////////////////////////////////////////
class Ray3D
{
public:
    // Constructor
    Ray3D() : origin(), direction() {return;}
    Ray3D(const Point3D& o, const Vector3D& d)
        : origin(o), direction(d) {return;} 


    // Containment method
    bool contains(const Point3D& point, float *t=NULL) const;
    // Returns paramter of intersection if containment is true and t != NULL

    Point3D origin;
    Vector3D direction;
};

struct AABB;

// Utility functions:
int Intersects(const Ray3D& ray, const Sphere3D& sphere,
               std::pair<Point3D, Point3D> *rpoints=0); 
bool Intersects(const Ray3D& ray, const Triangle3D& tri, Point3D *rpoint=0);
int Intersects(const Ray3D& ray, const AABB& box,
               std::pair<Point3D, Point3D> *rpoints=0);
bool Intersects(const Triangle3D& tri, const AABB& box);
int Intersects(const Ray3D& ray, const AABB& box,
               std::pair<float, float> *rpoints);


////////////////////////////////////////////////////////////////////////
// Box3D
////////////////////////////////////////////////////////////////////////
class Box3D
{
public:
    // Constructor
    Box3D() {return;}
    Box3D(const Point3D& c, const Vector3D& e) : center(c), extents(e) {return;}

    // Utility method
    bool contains(const Point3D& point) const;

    Point3D  center;    // Center point
    Vector3D extents;   // Center to corner half extents.
};

struct AABB
{
  AABB();
  AABB(Vector3D minPt, Vector3D maxPt);
  Vector3D m_min, m_max;
  float GetSurfaceArea() const;
};


////////////////////////////////////////////////////////////////////////
// Sphere3D
////////////////////////////////////////////////////////////////////////
class Sphere3D
{
public:
    // Constructors
    Sphere3D() : center(), radius(0) {return;}
    Sphere3D(const Point3D& c, const float r) : center(c), radius(r) {return;}

    Point3D center;
    float radius;
};

enum SplitAxis
{
  AXIS_X,
  AXIS_Y,
  AXIS_Z
};

struct AAPlane
{
  SplitAxis m_axis; // Which axis to split along.Z
  float m_point;    // how far along axis this split is.
  float m_cost;     // The cost of this split plane.
};


////////////////////////////////////////////////////////////////////////
// Plane3D
////////////////////////////////////////////////////////////////////////
class Plane3D
{
public:
    // Constructor
    Plane3D(const float A=0, const float B=0, const float C=0, const float D=0)
        { crds[0]=A; crds[1]=B; crds[2]=C; crds[3]=D; }

    // Indexing operators.
    float& operator[](const unsigned int i) { return crds[i]; }
    float operator[](const unsigned int i) const { return crds[i]; } 

    // Utility methods.
    Vector3D normal() const { return Vector3D(crds[0], crds[1], crds[2]); }
    float Evaluate(Point3D p) const { return crds[0]*p[0] + crds[1]*p[1] + crds[2]*p[2] + crds[3]; }
    float D() const;

    // Sequential-access methods.
    typedef float *iterator;
    typedef const float *const_iterator;
    iterator begin() { return crds + 0; }
    const_iterator begin() const { return crds + 0; }
    iterator end() { return crds + DIM; }
    const_iterator end() const { return crds + DIM; }

private:
    enum {DIM=4} ;
    float crds[DIM];

};

// Utility functions:
float AngleBetween(const Plane3D& plane1, const Plane3D& plane2);
bool Parallel(const Plane3D& plane1, const Plane3D& plane2);
bool Perpendicular(const Plane3D& plane1, const Plane3D& plane2);
bool Intersects(const Segment3D& seg, const Plane3D& plane, Point3D *rpoint=0);


////////////////////////////////////////////////////////////////////////
// Triangle3D
////////////////////////////////////////////////////////////////////////
class Triangle3D
{
public:
    // Constructor
    Triangle3D() {return;}
    Triangle3D(const Point3D& p1, const Point3D& p2, const Point3D& p3)
        { points[0]=p1; points[1]=p2; points[2]=p3; }

        Point3D& operator[](unsigned int i) { return points[i]; }
        const Point3D& operator[](unsigned int i) const { return points[i]; } 


    bool contains(const Point3D& point) const;

    Point3D* GetArray();

    void MakeAABB();

    Color m_diffuse;
    Color m_specular;

    Vector3D m_normal;
    
    AABB m_aabb;

    // Sequential-access methods.
    typedef Point3D *iterator;
    typedef const Point3D *const_iterator;
    iterator begin() { return points + 0; }
    const_iterator begin() const { return points + 0; }
    iterator end() { return points + DIM; }
    const_iterator end() const { return points + DIM; }

private:
    enum{ DIM=3 };
    Point3D points[DIM];
};

// Utility function:
int Intersects(const Triangle3D& tri1, const Triangle3D& tri2,
           std::pair<Point3D, Point3D> *rpoints=0);


////////////////////////////////////////////////////////////////////////
// Vector4D
////////////////////////////////////////////////////////////////////////
class Vector4D
{
public:
    // Constructor
    Vector4D(const float X=0, const float Y=0, const float Z=0, const float W=1)
        { crds[0]=X; crds[1]=Y; crds[2]=Z; crds[3]=W; }
    explicit Vector4D(const Vector3D V, float hcoord)
        { crds[0]=V[0]; crds[1]=V[1]; crds[2]=V[2]; crds[3]=hcoord; }

    // In-place operators.
    Vector4D& operator+=(const Vector4D& rhs); 
    Vector4D& operator-=(const Vector4D& rhs); 
    Vector4D& operator*=(const float& rhs);
    Vector4D& operator/=(const float& rhs);
    void normalize();
    Point3D Hdiv();

    Vector3D ToVec3() const;

    // Indexing operators.
    float& operator[](const unsigned int i) { return crds[i]; } 
    float operator[](const unsigned int i) const { return crds[i]; } 

    // Utility methods.
    float length() const;
    Vector4D normalized() const;

    // Sequential-access methods.
    typedef float *iterator;
    typedef const float *const_iterator;
    iterator begin() { return crds + 0; }
    const_iterator begin() const { return crds + 0; }
    iterator end() { return crds + DIM; }
    const_iterator end() const { return crds + DIM; }

private:
    enum {DIM=4};
    float crds[DIM];
};

// (Global) operators.
bool operator==(const Vector4D& lhs, const Vector4D& rhs); 
bool operator!=(const Vector4D& lhs, const Vector4D& rhs); 
Vector4D operator+(const Vector4D& lhs, const Vector4D& rhs); 
Vector4D operator-(const Vector4D& lhs, const Vector4D& rhs); 
Vector4D operator*(const Vector4D& lhs, const float& rhs); 
Vector4D operator*(const float& lhs, const Vector4D& rhs); 
Vector4D operator/(const Vector4D& lhs, const float& rhs); 

// Utility function:
float Dot(const Vector4D& hvec1, const Vector4D& hvec2);
float ToRadians(const float& Degrees);

// Stream operator
template<class charT, class traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& lhs, const Vector4D& rhs)
{
    std::basic_ostringstream<charT, traits> s;
    s.flags(lhs.flags());
    s.imbue(lhs.getloc());
    s.precision(lhs.precision());

    typename Vector4D::const_iterator i = rhs.begin();
    s <<'(' <<*i;

    for(++i; i != rhs.end(); ++i)
    {
        s <<',' <<*i;
    }
    s <<')';

    return lhs <<s.str();
}


////////////////////////////////////////////////////////////////////////
// Matrix4x4
////////////////////////////////////////////////////////////////////////
class Matrix4x4
{
public:
    // Constructor
    Matrix4x4(const Vector4D& row0=Vector4D(1, 0, 0, 0),
              const Vector4D& row1=Vector4D(0, 1, 0, 0),
              const Vector4D& row2=Vector4D(0, 0, 1, 0),
              const Vector4D& row3=Vector4D(0, 0, 0, 1))
        { rows[0] = row0;  rows[1] = row1;  rows[2] = row2;  rows[3] = row3; }

    // In-place operators.
    Matrix4x4& operator*=(const Matrix4x4& rhs);
    void reset();

    // Indexing operators. 
    Vector4D& operator[](const unsigned int i) { return rows[i]; }
    const Vector4D& operator[](const unsigned int i) const { return rows[i]; }

    // Sequential-access methods. 
    typedef Vector4D *iterator;
    typedef const Vector4D *const_iterator;
    iterator begin() { return rows + 0; }
    const_iterator begin() const { return rows + 0; }
    iterator end() { return rows + DIM; }
    const_iterator end() const { return rows + DIM; }

private:
    enum {DIM=4};
    Vector4D rows[DIM];
};

// (Global) operators.

Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs); 
Vector4D operator*(const Matrix4x4& lhs, const Vector3D& rhs); 
Vector4D operator*(const Matrix4x4& lhs, const Point3D& rhs); 
Vector4D operator*(const Matrix4x4& lhs, const Vector4D& rhs); 

// Utility functions
Matrix4x4 LookAt(const Point3D& center, const Point3D& direction,
                 const Vector3D& up);

Matrix4x4 Perspective(const float w, const float h,
                      const float d, const float f);

// Stream operator
template<class charT, class traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& lhs, const Matrix4x4& rhs)
{
    std::basic_ostringstream<charT, traits> s;
    s.flags(lhs.flags());
    s.imbue(lhs.getloc());
    s.precision(lhs.precision());

    for(typename Matrix4x4::const_iterator i=rhs.begin(); i != rhs.end(); ++i)
        s << *i << '\n';

    return lhs << s.str();
}

#endif // _GEOMLIB_H
