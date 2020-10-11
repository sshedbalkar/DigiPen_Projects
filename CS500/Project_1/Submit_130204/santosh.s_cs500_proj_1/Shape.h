#ifndef SHAPE_H
#define SHAPE_H
//
#include "vector.h"
#include "raytrace.h"
#include "hit.h"
//
class Shape 
{
public:
	virtual ~Shape(){};
	virtual bool Intersect( const Ray& r, double &t, Hit& h ) = 0;
	void SetPhong( const Phong& p ){ ph = p;}
	const Phong& GetPhong() const { return ph; }
	const V& GetPos() const { return pos; }
	void SetPos( V p ){ pos = p; }
	//
protected:
	V pos;
	Phong ph;
};


class Sphere: public Shape
{
public:
	bool Intersect( const Ray& r, double &t, Hit& h );
	void SetRadius( double rad ){ radius = rad; };
	//
private:
	double radius;
};

class Box: public Shape
{
public:
	bool Intersect( const Ray& r, double &t, Hit& h );
	void SetMin( V v ){ min = v;}
	void SetMax( V v ){ max = v;}
	const V& GetMin() const { return min; }
	const V& GetMax() const { return max; }
	//
private:
	V min, max;
};


class Cylinder: public Shape
{
public:	
	bool Intersect( const Ray& r, double &t, Hit& h );
	void SetBase( V v ){ p = v;}
	void SetTop( V v ){ q = v;}
	void SetRadius( const double& f ){ r = f;}
	const V& GetBase() const { return p; }
	const V& GetTop() const { return q; }
	const double& GetRadius() const { return r; }
	//
private:
	V p, q;
	double r;
};

class Triangle: public Shape
{
public:
	bool Intersect( const Ray& r, double &t, Hit& h );
	void SetPoint1( V v ){ p1 = v;}
	void SetPoint2( V v ){ p2 = v;}
	void SetPoint3( V v ){ p3 = v;}
	const V& GetP1() const { return p1; }
	const V& GetP2() const { return p2; }
	const V& GetP3() const { return p3; }
	//
private:
	V p1, p2, p3;
};

#endif