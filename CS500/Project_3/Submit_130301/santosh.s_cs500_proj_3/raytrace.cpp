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
#include "Lighting.h"
//
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
	PHONG,
	PHONG_BRDF,
	ASHIRLEY,
	DIELECTRIC
};
//
Scene* SCENE = 0;
//
ParserFlag state	= NOTHING;
Light				currentLight;
Lighting*			currLighting;
//
Scene::Scene()
{
	SCENE = this;
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
	else if(c == "dielectric") 
	{
		state = DIELECTRIC;
		Dielectric* p1 = new Dielectric();
		p1->SetRefractivity( f[0] );
		p1->SetReflectivity( f[1] );
		currLighting = p1;
	}
	else if(c == "ambient")
	{
		ambient = V( f[0], f[1], f[2] );
	}
	else if(c == "light")
	{
		state = LIGHT;
		currentLight.SetDiffuse( V(f[0], f[1], f[2]) );
	}
	else if(c == "sphere")
	{
		if( state == LIGHT )
		{
			
			Light* l = new Light( currentLight );
			V v( f[0], f[1], f[2] );
			l->SetPos( v );
			m_lightList.push_back( l );
			//
			Sphere* obj = new Sphere();
			obj->SetPos( v );
			obj->SetRadius( f[3] );
			obj->SetLighting( new Lighting() );
			obj->SetIsLight( true );
			m_shapeList.push_back( obj );
		}
		else if( state == PHONG || state == PHONG_BRDF || state == ASHIRLEY || state == DIELECTRIC )
		{
			Sphere* obj = new Sphere();
			obj->SetPos( V(f[0], f[1], f[2]) );
			obj->SetRadius( f[3] );
			obj->SetLighting( currLighting );
			m_shapeList.push_back( obj );
		}
	}
	else if(c == "phong")
	{
		state = PHONG;
		Phong* lm = new Phong();
		//
		lm->SetDiffuse( V(f[0], f[1], f[2]) );
		lm->SetSpecular( V(f[3],f[4],f[5]) );
		lm->SetShininess( f[6] );
		//
		currLighting = lm;
	}
	else if(c == "phongbrdf")
	{
		state = PHONG_BRDF;
		PhongBRDF* lm = new PhongBRDF();
		//
		lm->SetDiffuse( V( f[0], f[1], f[2]) );
		lm->SetSpecular( V(f[3], f[4], f[5]) );
		lm->SetShininess( f[6] );
		//
		currLighting = lm;

	}
	else if(c == "shirley")
	{
		state = ASHIRLEY;
		AShirley* lm = new AShirley;
		//
		lm->SetDiffuse( V( f[0], f[1], f[2]) );
		lm->SetSpecular( V(f[3], f[4], f[5]) );
		lm->SetShininess1( f[6] );
		lm->SetShininess2( f[7] );
		//
		currLighting = lm;
	}
	else if(c == "box")
	{
		if( state == PHONG || state == PHONG_BRDF || state == ASHIRLEY )
		{
			Box* obj = new Box();
			V v( f[0], f[1], f[2] );
			obj->SetMin( v );
			//
			V b = V( f[3], f[4], f[5] );
			obj->SetMax( obj->GetMin()+b );
			//
			obj->SetPos( ((obj->GetMax() + obj->GetMin())*0.5) );
			//
			obj->SetLighting( currLighting );
			m_shapeList.push_back( obj );
		}
	}
	else if(c == "cylinder")
	{
		if( state == PHONG || state == PHONG_BRDF || state == ASHIRLEY )
		{
			Cylinder* obj = new Cylinder();
			V v( f[0], f[1], f[2] );
			obj->SetBase( v );
			V b = V( f[3], f[4], f[5] );
			obj->SetTop( v+b );
			obj->SetRadius( f[6] );
			obj->SetPos( (obj->GetTop()+obj->GetBase())*0.5 );
			obj->SetLighting( currLighting );
			m_shapeList.push_back( obj );
		}
	}
	else if(c == "triangle")
	{
		if( state == PHONG || state == PHONG_BRDF || state == ASHIRLEY )
		{
			Triangle* obj = new Triangle();
			obj->SetPoint1( V(f[0], f[1], f[2]) );
			obj->SetPoint2( V(f[3], f[4], f[5]) );
			obj->SetPoint3( V(f[6], f[7], f[8]) );
			obj->SetE1( obj->GetP2() - obj->GetP1() );
			obj->SetE2( obj->GetP3() - obj->GetP1() );
			obj->SetNormal( obj->GetE2().cross(obj->GetE1()).unit() );
			//
			obj->SetLighting(currLighting);
			m_shapeList.push_back(obj);
		}

	}
    else {
        fprintf(stderr, "\n*********************************************\n");
        fprintf(stderr, "* Unknown command: %s\n", c.c_str());
        fprintf(stderr, "*********************************************\n\n"); }
}
//
const int AA_Factor = 4;
const double AA_step = 1.0 / ((double)AA_Factor);
//
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
	//
	unsigned short Xi[3];
#pragma omp parallel for schedule(dynamic, 1) private(Xi)
	for (int y = 0; y < height; ++y)
	{
		Xi[0] = pass;
		Xi[1] = y*pass*pass;
		Xi[2] = y*y*pass;
		for( int x = 0; x < width; ++x)
		{
			V color(0.0, 0.0, 0.0);
			for( int yy = 0; yy<AA_Factor; ++yy )
			{
				for( int xx = 0; xx<AA_Factor; ++xx )
				{
					double stepX = 0.5, stepY = 0.5;
					if( AA_Factor > 1 )
					{
						stepX = ((double) xx)*AA_step;
						stepY = ((double) yy)*AA_step;
					}
					Hit hit;
					Ray currRay;
					double t = 2000.0;
					//
					double dx = ( 2.0 *(x+stepX)/width ) - 1.0;
					double dy = ( 2.0 *(y+stepY)/height ) - 1.0;
					//
					// Create a ray for the current pixel and set its origin and direction
					V v( X*dx + Y*dy + Z );
					currRay.SetDir( v.unit() );
					currRay.SetOrigin( camera.GetEyePos() );
					//
					for( ShapeListType::iterator it1 = m_shapeList.begin(), it2 = m_shapeList.end(); it1 != it2; ++it1 )
					{
						(*it1)->Intersect( currRay, t, hit );
					}
					if( !hit.Obj ) 
					{
						continue;
					}
					color += hit.Obj->GetLighting()->GetColor( hit );
				}
			}
			//
			image[y*width + x] = color / (AA_Factor*AA_Factor) ;
		}
	}
}
//
bool Scene::TraceRay( Ray& ray, Hit& hit )
{
	double t = 2000.0;
	for( ShapeListType::iterator i = m_shapeList.begin(), j = m_shapeList.end(); i != j; ++i )
	{
		(*i)->Intersect( ray, t, hit );
	}
	if(hit.Obj)
	{
		return true;
	}
	return false;
}
