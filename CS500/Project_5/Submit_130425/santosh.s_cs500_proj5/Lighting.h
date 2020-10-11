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
	virtual V GetColor( const Hit& hit, const V& startPos );
	virtual ~Lighting(){};
	//virtual V GetColorRecursive( const Hit& hit, const V& eyePos, int recursionlevel ) const;
	virtual const V& Diffuse() { return m_diffuse; }
	//
protected:
	std::vector< Light* > GetVisibleLights( const LightListType& lights, const ShapeListType& shapes, const V& pos ) const;
	V m_diffuse;
};
//
class PhongBRDF: public Lighting
{
private:
	enum choice
	{
		DIFFUSE,
		SPECULAR,
		TRANSMISSION
	};
	//
public:
	void SetDiffuse( const V& v ){ m_diffuse = v; }
	void SetSpecular( const V& v ){ m_specular = v; }
	void SetShininess( double v ){ m_shininess = v; }
	V GetColorRecursive( const Hit& hit, const V& eyePos, int recursionlevel ) const;
	void SetKt( const V& v ){ Kt = v; }
	void SetIor( double v ){ ior = v; }
	V CalcDiffuse( Light* light, const Hit &hit );
	V CalcSpecular( Light* light, const Hit &hit, const V& viewDir );
	V GetColor( const Hit & hit, const V& viewdirection );
	V GetDirectColor( const Hit & hit, const V& viewDir );
	V GetInDirectColor( const Hit & hit, const V& viewDir );
	choice ChooseRandomProperty();
	double RGBtoYUV( V& v );
	void SetKd( double v ){ Kd = v; }
	void SetKs( double v ){ Ks = v; }
	void SetKt_Y( double v ){ Kt_Y = v; }
	void SetKsum(){ K_Sum = Kd + Ks + Kt_Y; }
	bool Snell( double n, const V& I, const V& normal, V& T ); 
	//
private:
	V m_specular;
	double m_shininess;
	double K_Sum;
	double Kd;
	double Ks;
	double Kt_Y;
	double ior;
	V Kt;
};
//
#endif