#ifndef SHAPE_H
#define SHAPE_H
//
#include <vector>
#include "vector.h"
#include "raytrace.h"
#include "hit.h"
//
enum CSG_PART{ NO_CSG, UNION, INTERSECT, DIFF };
//
class Lighting;
class Shape 
{
public:
	Shape():m_isLight( false ){}
	virtual ~Shape(){};
	//
	virtual bool Intersect( const Ray& r, double &t, Hit& h ) = 0;
	void SetLighting( Lighting* lm ){ m_lighting = lm; }
	Lighting* GetLighting() { return m_lighting; }
	const V& GetPos() const { return pos; }
	void SetPos( const V& p ){ pos = p; }
	void SetIsLight( bool b ){ m_isLight = b; }
	bool IsLight(){ return m_isLight; }
	//
	virtual double Estimate( V p ){ return 0.0; }
	void SetOrientation( Q  q ){ m_orientation = q; }
	Q GetOrientation(){ return m_orientation; }
	void SetTranslation ( V v ){ m_translation = v; }
	V GetTranslation(){ return m_translation; }
	void SetScale( V v ){ m_scale = v; }
	V GetScale(){ return m_scale; }
	CSG_PART CSGFlag;
	//
protected:
	//
	V pos;
	Lighting* m_lighting;
	bool m_isLight;
	//
	V m_translation;
	V m_scale;
	Q m_orientation;
};
//
class Sphere: public Shape
{
public:
	bool Intersect( const Ray& r, double &t, Hit& h );
	void SetRadius( double rad ){ m_radius = rad; };
	double GetRadius() { return m_radius; }
	double Estimate( V p );
	//
private:
	double m_radius;
};
//
class Box: public Shape
{
public:
	bool Intersect( const Ray& r, double &t, Hit& h );
	void SetMin( const V& v ){ min = v;}
	void SetMax( const V& v ){ max = v;}
	const V& GetMin() const { return min; }
	const V& GetMax() const { return max; }
	double Estimate( V p );
	//
private:
	V min, max;
};
//
class Cylinder: public Shape
{
public:	
	bool Intersect( const Ray& r, double &t, Hit& h );
	void SetBase( const V& v ){ p = v;}
	void SetTop( const V& v ){ q = v;}
	void SetRadius( const double& f ){ r = f;}
	const V& GetBase() const { return p; }
	const V& GetTop() const { return q; }
	const double& GetRadius() const { return r; }
	double Estimate( V p );
	//
private:
	V p, q;
	double r;
};
//
class Triangle: public Shape
{
public:
	bool Intersect( const Ray& r, double &t, Hit& h );
	void SetPoint1( const V& v ){ p1 = v;}
	void SetPoint2( const V& v ){ p2 = v;}
	void SetPoint3( const V& v ){ p3 = v;}
	const V& GetP1() const { return p1; }
	const V& GetP2() const { return p2; }
	const V& GetP3() const { return p3; }
	double Estimate( V p );
	V E1, E2, Normal;
	//
private:
	V p1, p2, p3;
};
//
class CSGShape: public Shape
{
public:
	double Estimate( V p );
	bool Intersect( const Ray & r, double &t, Hit& h );
	void AddShape( Shape *p ){ m_shapeList.push_back(p); }
	//
private:
	typedef std::vector< Shape* > ShapeListType;
	ShapeListType m_shapeList;
};
//
#endif