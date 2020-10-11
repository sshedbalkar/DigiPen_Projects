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

const double PI = 3.14159;
const double ONE_PI = 0.31830988618;

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
	V m_pos;
	V m_Id;
	//
public:
	const V& GetPos() const { return m_pos; }
	const V& GetDiffuse() const { return m_Id; }
	//
	void SetPos( V v ) { m_pos = v;}
	void SetDiffuse( V v ) { m_Id = v;}
};
//
/*
class Phong
{
private:
	V m_Kd;
	V m_Ks;
	double n;

public:
	const V& GetDiffuse() const { return m_Kd; }
	const V& GetSpecular() const { return m_Ks; }
	const double& GetShininess() const { return n; }
	void SetDiffuse( V v ) {  m_Kd = v; }
	void SetSpecular( V v ) { m_Ks = v;}
	void SetShininess( double f ) { n = f;}
};
*/
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
class Scene
{
public:
	Scene();
	~Scene();
	int width, height;
	typedef std::vector< Shape* > ShapeListType;
	typedef std::vector< Light* > LightListType;
	ShapeListType m_shapeList;
	LightListType m_lightList;
	V ambient;

	// The scene reader-parser will call the Command method with the
	// contents of each line in the scene file.
	void Command( const std::string c, const std::vector<double> f,
					const std::vector<std::string> strings );

	// The main program will call the TraceImage method to generate
	// and return the image.
	void TraceImage( V* image, const int pass );
	const Camera& GetCamera(){ return camera; }
private:
	Camera camera;
};
//
extern Scene* SCENE;
//
#endif