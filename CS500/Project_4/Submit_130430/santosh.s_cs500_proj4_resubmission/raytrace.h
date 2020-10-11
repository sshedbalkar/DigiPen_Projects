///////////////////////////////////////////////////////////////////////
// Provides the framework a raytracer.
//
// Gary Herron
//
// Copyright © 2012 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////
#ifndef RAYTRACE_H
#define RAYTRACE_H
////////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include <string>
#include <fstream>
#include <vector>
#include "vector.h"
#include "hit.h"
#include <random>
//
const double PI = 3.14159;
const double ONE_PI = 0.31830988618;
//
extern std::mt19937_64 gen;
extern std::uniform_real_distribution<> random;
extern double erand48( unsigned short int xsubi[3] );
//
class Shape;

class Ray 
{
private:
	V m_Origin;
	V m_Dir;
	//
public:
	void SetDir( V v ) { m_Dir = v; }
	void SetOrigin( V v ) { m_Origin = v; }
	//
	const V& GetOrigin() const { return m_Origin; }
	const V& GetDir() const { return m_Dir; }
};
//
class Light
{
private:
	V		m_pos;
	V		m_Id;
	Shape*	m_shape;
	//
public:
	Light(){};
	Light( const Light* l )
	{
		m_pos	= l->m_pos;
		m_Id	= l->m_Id;
	}
	//
	Shape* GetShape(){ return m_shape; }
	void SetShape( Shape* sh ){ m_shape = sh; }
	const V& GetPos() const { return m_pos; }
	const V& GetDiffuse() const { return m_Id; }
	//
	void SetPos( V v ) { m_pos = v;}
	void SetDiffuse( V v ) { m_Id = v;}
};
//
class Camera
{
private:
	V m_eyePos;
	Q m_orientation;
	double ry;
	double rx;
public:
	void SetEyePos( V v ) { m_eyePos=v; }
	const V& GetEyePos() const { return m_eyePos; }
	void SetCameraOrientation( Q q ) { m_orientation=q; }
	const Q& GetOrientation() const { return m_orientation; }
	void SetRy( double t ){ry = t;}
	const double& GetRy() const { return ry; }
	void SetRx( double t ){ rx = t;}
	const double& GetRx() const { return rx; }
};
//
typedef std::vector< Shape* > ShapeListType;
typedef std::vector< Light* > LightListType;
//
class Scene
{
public:
	Scene();
	~Scene();
	int width, height;
	ShapeListType m_shapeList;
	LightListType m_lightList;
	V ambient;
	static unsigned short Xi[3];
	// The scene reader-parser will call the Command method with the
	// contents of each line in the scene file.
	void Command( const std::string c, const std::vector<double> f,
					const std::vector<std::string> strings );

	// The main program will call the TraceImage method to generate
	// and return the image.
	void TraceImage( V* image, const int pass );
	const Camera& GetCamera(){ return camera; }
	bool TraceRay( Ray& ray, Hit& hit);
	V Sample( V N, double  cTheta, double Phi );
	//
private:
	Camera camera;
};
//
extern Scene* SCENE;
//
#endif