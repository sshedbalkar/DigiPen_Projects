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
	virtual const V& GetColor( const Hit& hit ) const;
	virtual ~Lighting(){};
};
//
class Phong: public Lighting
{
public:
	const V& GetColor( const Hit& hit ) const;
	void SetDiffuse( const V& v ){ m_diffuse = v; }
	void SetSpecular( const V& v ){ m_specular = v; }
	void SetShininess( double v ){ m_shininess = v; }
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
	const V& GetColor( const Hit& hit ) const;
	void SetDiffuse( const V& v ){ m_diffuse = v; }
	void SetSpecular( const V& v ){ m_specular = v; }
	void SetShininess( double v ){ m_shininess = v; }
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
	const V& GetColor( const Hit& hit ) const;
	void SetDiffuse( const V& v ){ m_diffuse = v; }
	void SetSpecular( const V& v ){ m_specular = v; }
	void SetShininess1( double v ){ m_shininess1 = v; }
	void SetShininess2( double v ){ m_shininess2 = v; }
	//
private:
	V m_diffuse;
	V m_specular;
	double m_shininess1;
	double m_shininess2;
};
//
#endif