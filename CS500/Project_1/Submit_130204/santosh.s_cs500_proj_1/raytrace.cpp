///////////////////////////////////////////////////////////////////////
// Provides the framework a raytracer.
//
// Gary Herron
//
// Copyright © 2012 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include "raytrace.h"
#include "Shape.h"
#include "hit.h"
#include "Utility.h"

#ifdef _WIN32
    // Includes for Windows
    #include <windows.h>
    #include <cstdlib>
    #include <limits>
    #include <crtdbg.h>
    #include <random>
    std::mt19937_64 gen;
    std::uniform_real_distribution<> random(0.0, 1.0);
    double erand48(unsigned short int xsubi[3]) { return random(gen); }
#else
    // Includes for Linux
    #include <stdlib.h>
    double abs(double v) { return v>0 ? v : -v; }
#endif

enum ParserFlag
{
	NOTHING = 0,
	LIGHT,
	PHONG
};
//
ParserFlag state = NOTHING;
Light currentLight;
Phong currentPhong;
//
Scene::Scene()
{
	//
}
//
Scene::~Scene()
{
	for( ShapeListType::iterator it1 = m_shapeList.begin(), it2 = m_shapeList.end(); it1 != it2; it1++ )
	{
		delete *it1;
	}
	for( LightListType::iterator it1 = m_lightList.begin(), it2 = m_lightList.end(); it1 != it2; it1++ )
	{
		delete *it1;
	}
}
//
void Scene::Command(const std::string c, const std::vector<double> f, const std::vector<std::string> strings)
{
    if (c == "screen") {
        width = f[0];
        height = f[1]; }
	else if(c == "camera")
	{
		V v( f[0], f[1], f[2] );
		camera.SetEyePos(v);
		Q q( f[3], f[4], f[5], f[6] );
		camera.SetCameraOrientation( q );
		camera.SetRy( f[7] );

		double f = (double)width*camera.GetRy()/(double)height;
		camera.SetRx(f);
	}
	else if(c == "ambient")
	{
		ambient = V( f[0], f[1], f[2] );
	}
	else if(c == "light")
	{
		state = LIGHT;
		V v( f[0], f[1], f[2] );
		currentLight.SetDiffuse( v );
	}
	else if(c == "sphere")
	{
		if(state == LIGHT)
		{
			Light* l = new Light( currentLight );
			V v( f[0], f[1], f[2] );
			l->SetPos( v );
			m_lightList.push_back( l );
		}
		else if( state == PHONG )
		{
			Sphere* obj = new Sphere();
			V v( f[0], f[1], f[2] );
			obj->SetPos( v );
			obj->SetRadius( f[3] );
			obj->SetPhong( currentPhong );
			m_shapeList.push_back( obj );
		}
	}
	else if(c == "phong")
	{
		state = PHONG;
		V v( f[0], f[1], f[2] );
		currentPhong.SetDiffuse( v );
		v = V( f[3], f[4], f[5] );
		currentPhong.SetSpecular( v );
		double d = f[6];
		currentPhong.SetShininess( d );
	}
	else if(c == "box")
	{
		if (state==PHONG)
		{
			Box* obj = new Box();
			V v( f[0], f[1], f[2] );
			obj->SetMin( v );
			//
			V b = V( f[3], f[4], f[5] );
			obj->SetMax( obj->GetMin()+b );
			//
			obj->SetPos( ((obj->GetMax() + obj->GetMin())*0.5f) );
			//
			obj->SetPhong( currentPhong );
			m_shapeList.push_back( obj );
		}
	}
	else if(c == "cylinder")
	{
		if (state == PHONG)
		{
			Cylinder* obj = new Cylinder();
			V v( f[0], f[1], f[2] );
			obj->SetBase( v );
			V b = V( f[3], f[4], f[5] );
			obj->SetTop( v+b );
			obj->SetRadius( f[6] );
			obj->SetPos( (obj->GetTop()+obj->GetBase())*0.5 );
			obj->SetPhong( currentPhong );
			m_shapeList.push_back( obj );
		}
	}
	else if(c == "triangle")
	{
		if (state == PHONG)
		{
			Triangle* obj = new Triangle();
			V v( f[0], f[1], f[2] );
			obj->SetPoint1( v );
			v = V( f[3], f[4], f[5] );
			obj->SetPoint2( v );
			v = V( f[6], f[7], f[8] );
			obj->SetPoint3( v );
			//
			obj->SetPhong(currentPhong);
			m_shapeList.push_back(obj);
		}

	}
    else {
        fprintf(stderr, "\n*********************************************\n");
        fprintf(stderr, "* Unknown command: %s\n", c.c_str());
        fprintf(stderr, "*********************************************\n\n"); }
}


void Scene::TraceImage(V* image, const int pass)
{
	V X, Y, Z, temp;
	temp = V( camera.GetRx(), 0, 0 );
	Q q = camera.GetOrientation();
	q = q.conj();
	X = q.rotate( temp );
	temp = V( 0,camera.GetRy(), 0 );
	Y = q.rotate( temp );
	temp = V( 0, 0, -1 );
	Z = q.rotate( temp );
	V color;
	//
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			Hit hit;
			double dx, dy;
			Ray currRay;
			double t = 2000.0;
			//
			dx = ( 2.0 *(x+0.5)/width ) - 1.0;
			dy = ( 2.0 *(y+0.5)/height ) - 1.0;
			//
			// Create a ray for the current pixel and set its origin and direction
			V v( X*dx + Y*dy + Z );
			currRay.SetDir( v );
			currRay.SetOrigin( camera.GetEyePos() );
			//
			for( ShapeListType::iterator it1 = m_shapeList.begin(), it2 = m_shapeList.end(); it1 != it2; ++it1 )
			{
				(*it1)->Intersect( currRay, t, hit );
			}
			if( !hit.Obj ) 
			{
				image[y*width + x] = V( 0.0, 0.0, 0.0 );
				continue;
			}
			//
			const Phong& p = hit.Obj->GetPhong();
			color = ambient * p.GetDiffuse();
			V EyeVec = camera.GetEyePos() - hit.Position;
			//
			for( LightListType::iterator it1 = m_lightList.begin(), it2 = m_lightList.end(); it1 != it2; ++it1 )
			{
				V Ldir = (*it1)->GetPos() - hit.Position;
				Ldir=Ldir.unit();
				//
				double nDotL = big( 0.0, hit.Normal.dot(Ldir) );
				V D = ( p.GetDiffuse() * (*it1)->GetDiffuse() ) * nDotL;
				//
				V reflection = ( ((hit.Normal) * 2.0) * nDotL ) - Ldir;
				reflection = reflection.unit();
				//
				double rDotE = max( 0.0, reflection.dot(EyeVec.unit()) );
				double power = pow( rDotE,p.GetShininess() );
				V S = ( p.GetSpecular() * (*it1)->GetDiffuse()) * power;
				//
				color= color + D + S;
			}
			image[y*width + x] = color;
		}
	}
}
