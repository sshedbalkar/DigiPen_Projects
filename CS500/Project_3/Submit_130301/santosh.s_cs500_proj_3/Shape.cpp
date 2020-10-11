#include "Shape.h"
#include "raytrace.h"
#include "Utility.h"
#include "Lighting.h"
//
#define EPSILON 0.00001
//
bool Sphere::Intersect( const Ray& ray, double& tmin, Hit& h )
{
	V sphereToRay = ray.GetOrigin() - GetPos();
	double a = ray.GetDir().dot( ray.GetDir());
	double b = 2 * sphereToRay.dot( ray.GetDir());
	double c = sphereToRay.dot( sphereToRay) - radius * radius;

	double delta = b * b - 4.0 * a * c;
	if( fabs(a) < DBL_EPSILON || delta < 0.0 ) return false;

	double t1 = (-b + sqrt(delta)) / (2.0 * a);
	double t2 = (-b - sqrt(delta)) / (2.0 * a);
	double t;
	if(t1 > 0.0 && t2 <= 0.0)
	{
		t= t1;
	}
	else if(t2 > 0.0 && t1 <= 0.0)
	{
		t = t2;
	}
	else if( t1 <= 0.0 && t2 <= 0.0 )
	{
		return false;
	}
	else
	{
		t = (t1<t2)? t1: t2;
	}
	// check if this is the closest intersecting shape currently, if yes, then populate the hit obj with relevent data
	if( t < tmin )
	{
		tmin = t;
		h.Position = ray.GetOrigin() + ray.GetDir() * tmin; 
		h.Normal = (h.Position-GetPos()).unit();
		//
		h.Obj = this;
	}
	//
	return true;
}
//

enum Axes { AXIS_X, AXIS_Y, AXIS_Z };
bool Box::Intersect( const Ray& r, double &t, Hit& h )
{
	double tmin = 0.0;
	double tmax = FLT_MAX;
	//
	V p = r.GetOrigin();
	V d = r.GetDir();
	//d = d.unit();
	bool sign[3] = {false, false, false};
	Axes a = AXIS_X;
	for( int i = 0; i < 3; ++i )
	{
		if( abs(d[i]) < EPSILON )
		{
			// Ray is parallel to slab. No hit if origin not within slab
			if (p[i] < min[i] || p[i] > max[i]) 
				return false;
		}
		else
		{
			// Compute intersection t value of ray with near and far plane of slab
			double ood = 1.0 / d[i];
			double t1 = (min[i] - p[i]) * ood;
			double t2 = (max[i] - p[i]) * ood;
			//
			// Make t1 be intersection with near plane, t2 with far plane
			if( t1 > t2 )
			{
				std::swap( t1, t2 );
				if( a == AXIS_Z )
					sign[0] = !sign[0];
				else if( a == AXIS_Y )
					sign[1] = !sign[1];
				else if( a == AXIS_Z )
					sign[2]=!sign[2];
			}
			// Compute the intersection of slab intersection intervals
			if( t1 > tmin )
			{
				tmin = t1;
				if( i == 0 )
					a = AXIS_X;
				else if( i == 1 )
					a = AXIS_Y;
				else if( i == 2 )
					a = AXIS_Z;
			}
			if( t2 < tmax )
				tmax = t2;
			// Exit with no collision as soon as slab intersection becomes empty
			if( tmin > tmax )
				return false;
		}
	}
	//
	//V point = p + ( d*(tmin + EPSILON) );
	//
	//check if this is the closest intersecting shape currently, if yes, then populate the hit obj with relevent data
	if(tmin < t)
	{
		//if(a != AXIS_Y) return false;
		t = tmin;
		if(a == AXIS_X)
		{
			if(sign[0]) { h.Normal = V(-1,0,0); }
			else { h.Normal = V(1,0,0); }
		}
		//
		if(a == AXIS_Y)
		{
			if(sign[1]) { h.Normal = V(0,1,0); }
			else { h.Normal = V(0,-1,0); }
		}
		//
		if(a == AXIS_Z)
		{
			if(sign[2]) { h.Normal = V(0,0,-1); }
			else {h.Normal = V(0,0,1);  }
		}
		h.Position = p + d*tmin;
		h.Normal = h.Normal.unit();
		h.Obj = this;
	}
	return true;
}
//
void cylinderHelperF( double &tmin, double t, Hit& h, const Ray& r, Shape& p, int side, V endPoint )
{
	if( t < DBL_EPSILON ) return;
	V w = r.GetOrigin() + endPoint * t; 
	V v = w - r.GetOrigin();
	t = v.length();

	//check if this is the closest intersecting shape currently, if yes, then populate the hit obj with relevent data
	if( t < tmin )
	{
		V N;
		tmin = t;
		h.Position = w; 
	
		if( side == 0 )	//mid section
		{
			N = h.Position - V( p.GetPos().x, p.GetPos().y, h.Position.z );
		}
		else if( side == 1 ) //top plane
		{
			N = V( p.GetPos().x, p.GetPos().y, h.Position.z ) - p.GetPos(); 
		}
		else if( side == 2 ) //bottom plane
		{
			N = V( p.GetPos().x, p.GetPos().y, h.Position.z ) - p.GetPos(); 
		}
		//
		h.Normal = N.unit();
		h.Obj = &p;
	}
}
//
bool Cylinder::Intersect( const Ray &ray, double &tmin, Hit& h )
{
	double t;
	V p = GetBase();
	V q = GetTop();
	double r = GetRadius();
	//
	V d = q - p;
	V m = ray.GetOrigin() - p;
	V n = (ray.GetOrigin() + ray.GetDir() * 100.0) - ray.GetOrigin();
	//
	double md = m.dot(d);
	double nd = n.dot(d);
	double dd = d.dot(d);
	//
	double nn = n.dot(n);
	double mn = m.dot(n);
	double a = (dd * nn) - (nd * nd);
	double k = m.dot(m) - (r * r);
	double c = (dd * k) - (md * md);
	//
	if( abs(a) < EPSILON )
	{
		if (c > 0.0) return 0;
		//
		if (md < 0.0) t = -mn / nn;
		else if (md > dd) t = (nd - mn) / nn;
		else t = 0.0;
		cylinderHelperF( tmin, t, h, ray, *this, 0, n );
		return true;
	}
	//
	double b = (dd * mn) - (nd * md);
	double discr = (b * b) - (a * c);
	//
	if (discr < 0.0) 
		return 0;
	//
	t = (-b - sqrt(discr)) / a;
	//
	if (md + t * nd < 0.0) 
	{
		// Intersection outside cylinder on bottom side
		if (nd <= 0.0) 
			return 0; // Segment pointing away from end
		t = -md / nd;
		//
		if( k + 2 * t * (mn + t * nn) <= 0.0)
		{
			cylinderHelperF( tmin, t, h, ray, *this, 2, n );
			return true;
		}
		else
		{
			return false;
		}
	}
	else if( md + t * nd > dd )
	{
		// Intersection outside cylinder on top side
		if (nd >= 0.0) 
			return 0; // Segment pointing away from end
		t = (dd - md) / nd;
		//
		if( k + dd - 2.0 * md + t * (2 * (mn - nd) + t * nn) <= 0.0)
		{
			cylinderHelperF( tmin, t, h, ray, *this, 1, n );
			return true;
		}
		else
			return false;
	}
	//
	// Segment intersects cylinder between the end planes; t is correct
	cylinderHelperF( tmin, t, h, ray, *this, 0, n );
	return true;
}


bool Triangle::Intersect( const Ray & ray,  double &tmin, Hit& h)
{
	V p = ray.GetDir().cross( E2 );
	double d = p.dot( E1 );
	if( fabs(d) < DBL_EPSILON ) return false;
	V S = ray.GetOrigin() - p1;
	double u = p.dot( S )/d;
	if( u<0.0 || u>1 ) return false;
	V q = S.cross( E1 );
	double v = ray.GetDir().dot( q )/d;
	if( v<0.0 || u+v>1 ) return false;
	double t = E2.dot( q )/d;
	if( t < 0.0 ) return false;
	//
	//check if this is the closest intersecting shape currently, if yes, then populate the hit obj with relevent data
	if( t < tmin )
	{
		tmin = t;
		h.Position = ray.GetOrigin() + ray.GetDir() * t; 
		h.Normal = Normal;
		//
		h.Obj = this;
	}
	return true;
}