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
#include <omp.h>
#include <ctime>
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
enum CSGFlag
{
	NO_CSG2 = 0,
	CSG
};
//
Scene* SCENE = 0;
//
ParserFlag state	= NOTHING;
Light				currentLight;
Lighting*			currLighting;
CSG_PART state2		= UNION;
CSGFlag state3		= NO_CSG2;
CSGShape* obj1;
Shape* CSGPart;
int CSGCount;
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
			l->SetShape( obj );
		}
		else if( state == PHONG || state == PHONG_BRDF || state == ASHIRLEY || state == DIELECTRIC )
		{
			Sphere* obj = new Sphere();
			V v(f[0], f[1], f[2]);
			obj->SetPos( v );
			obj->SetRadius( f[3] );
			//
			if( state3 == CSG )
			{
				CSGPart = obj;
				if( CSGCount == 0 )
				{
					obj1 = new CSGShape();
				}
				obj->CSGFlag = state2;
				obj1->AddShape( obj );
				if( CSGCount == 0 )
				{
					obj1->SetPos( v );
					m_shapeList.push_back( obj1 );
					obj1->SetLighting( currLighting );
				}
			}
			else
			{
				obj->SetLighting( currLighting );
				m_shapeList.push_back( obj );
			}
		}
	}
	else if(c == "phongbrdf")
	{
		state = PHONG_BRDF;
		PhongBRDF* lm = new PhongBRDF();
		//
		V v(f[0],f[1],f[2]);
		lm->SetDiffuse( v );
		lm->SetKd( lm->RGBtoYUV(v) );
		//
		v = V( f[3], f[4], f[5] );
		lm->SetSpecular( v );
		lm->SetKs( lm->RGBtoYUV(v) );
		//
		lm->SetShininess( f[6] );
		//
		v = V( f[7], f[8], f[9] );
		lm->SetKt(v);
		lm->SetKt_Y( lm->RGBtoYUV(v) );
		lm->SetIor( f[10] );
		lm->SetKsum();
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
			if( state3 == CSG )
			{
				CSGPart = obj;
				if( CSGCount == 0 )
				{
					obj1 = new CSGShape();
				}
				obj->CSGFlag = state2;
				obj1->AddShape( obj );
				if( CSGCount == 0 )
				{
					obj->SetPos( ((obj->GetMax() + obj->GetMin())*0.5) );
					m_shapeList.push_back( obj1 );
					obj1->SetLighting( currLighting );
				}
			}
			else
			{
				obj->SetLighting( currLighting );
				m_shapeList.push_back( obj );
			}
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
			//
			if( state3 == CSG )
			{
				CSGPart = obj;
				if( CSGCount == 0 )
				{
					obj1 = new CSGShape();
				}
				obj->CSGFlag = state2;
				obj1->AddShape( obj );
				if( CSGCount == 0 )
				{
					obj1->SetPos( obj->GetPos() );
					m_shapeList.push_back( obj1 );
					obj1->SetLighting( currLighting );
				}
			}
			else
			{
				obj->SetLighting( currLighting );
				m_shapeList.push_back( obj );
			}
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
			obj->E1 = obj->GetP2() - obj->GetP1();
			obj->E2 = obj->GetP3() - obj->GetP1();
			obj->Normal = obj->E2.cross( obj->E1 ).unit();
			//
			obj->SetLighting(currLighting);
			m_shapeList.push_back(obj);
		}

	}
	else if( c == "CSG_START" )
	{
		state3 = CSG;
		CSGCount = 0;
		state2 = UNION;
	}
	else if( c == "CSG_END" )
	{
		state3 = NO_CSG2;
	}
	else if( c == "INTERSECT" )
	{
		state2 = INTERSECT;
		++CSGCount;
	}
	else if( c == "UNION" )
	{
		state2 = UNION;
		++CSGCount;
	}
	else if( c == "DIFF" )
	{
		state2 = DIFF;
		++CSGCount;
	}
	else if( c == "translate" )
	{
		V v( f[0], f[1], f[2] );
		CSGPart->SetTranslation( v );
	}
	else if( c == "scale" )
	{
		V v( f[0], f[1], f[2] );
		CSGPart->SetScale( v );
	}
	else if( c == "rotate" )
	{
		Q q( f[0], f[1], f[2], f[3] );
		CSGPart->SetOrientation( q );
	}
    else {
        fprintf(stderr, "\n*********************************************\n");
        fprintf(stderr, "* Unknown command: %s\n", c.c_str());
        fprintf(stderr, "*********************************************\n\n"); }
}
//
unsigned short Scene::Xi[3];
const int AA_Factor = 4;
const double AA_step = 1.0 / ((double)AA_Factor);
//
V Scene::Sample( V N, double  cTheta, double Phi )
{
	double sTheta = sqrt(1 - cTheta*cTheta);
	V k = V(sTheta*cos(Phi), sTheta*sin(Phi), cTheta);
	Q q = QuaternionV2Z(N).conj();
	return q.rotate(k);
}
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
	double dx, dy;
	//
	//unsigned short Xi[3];
//#pragma omp parallel for schedule(dynamic, 1) private(Xi)
	for (int y = 0; y < height; ++y)
	{
		for( int x = 0; x < width; ++x)
		{
			Xi[0] = pass;
			Xi[1] = y*pass*pass;
			Xi[2] = y*y*pass;
			V color(0.0, 0.0, 0.0);

			dx = (2.0*(x+erand48(Xi))/width)-1.0;
			dy = (2.0*(y+erand48(Xi))/height)-1.0;
			Hit hit;
			Ray viewRay;
			V v(X*dx+Y*dy+Z);
			viewRay.SetDir(v.unit());
			viewRay.SetOrigin(camera.GetEyePos());
			//
			double t=2000.0;
			for( ShapeListType::iterator it1 = m_shapeList.begin(), it2 = m_shapeList.end(); it1 != it2; ++it1 )
			{
				(*it1)->Intersect( viewRay, t, hit );
			}
			if( !hit.Obj ) 
			{
				continue;
			}
			//
			V viewdir = SCENE->camera.GetEyePos()-hit.Position;
			viewdir = viewdir.unit();
			color += hit.Obj->GetLighting()->GetColor( hit, viewdir );
			image[y*width + x] += color ;
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
