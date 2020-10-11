#ifndef LIGHTING_H
#define LIGHTING_H
//
#include "vector.h"
#include "raytrace.h"
#include "hit.h"
//
class Lighting
{
public:
	virtual V GetColor( const Hit& hit ) const;
	virtual ~Lighting(){};
	virtual V GetColorRecursive( const Hit& hit, const V& eyePos, int recursionlevel ) const;
	//
protected:
	std::vector< Light* > GetVisibleLights( const LightListType& lights, const ShapeListType& shapes, const V& pos ) const;
};
//
class Phong: public Lighting
{
public:
	void SetDiffuse( const V& v ){ m_diffuse = v; }
	void SetSpecular( const V& v ){ m_specular = v; }
	void SetShininess( double v ){ m_shininess = v; }
	V GetColorRecursive( const Hit& hit, const V& eyePos, int recursionlevel ) const;
	//
private:
	V m_diffuse;
	V m_specular;
	double m_shininess;
};
//
class PhongBRDF: public Lighting
{
public:
	void SetDiffuse( const V& v ){ m_diffuse = v; }
	void SetSpecular( const V& v ){ m_specular = v; }
	void SetShininess( double v ){ m_shininess = v; }
	V GetColorRecursive( const Hit& hit, const V& eyePos, int recursionlevel ) const;
	//
private:
	V m_diffuse;
	V m_specular;
	double m_shininess;
};
//
class AShirley: public Lighting
{
public:
	void SetDiffuse( const V& v ){ m_diffuse = v; }
	void SetSpecular( const V& v ){ m_specular = v; }
	void SetShininess1( double v ){ m_shininess1 = v; }
	void SetShininess2( double v ){ m_shininess2 = v; }
	V GetColorRecursive( const Hit& hit, const V& eyePos, int recursionlevel ) const;
	//
private:
	V m_diffuse;
	V m_specular;
	double m_shininess1;
	double m_shininess2;
};
//
class Dielectric: public Lighting
{
public:
	bool Snell( double n, const V& I, const V& normal, V& T ) const;
	void SetRefractivity( double v ){ m_refractivity = v; }
	void SetReflectivity( double v ){ m_reflectivity = v; }
	V GetColorRecursive( const Hit& hit, const V& eyePos, int recursionlevel ) const;
	//
private:
	double m_refractivity;
	double m_reflectivity;
};
//
#endif