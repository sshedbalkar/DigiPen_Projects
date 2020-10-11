#include "Shape.h"
#include "raytrace.h"
#include "Utility.h"
#include "Lighting.h"
//
#define EPSILON 0.001
//
bool Sphere::Intersect( const Ray &r, double &tmin, Hit& h )
{
	double t;
	V m = r.GetOrigin () - pos;
	V dir = r.GetDir().unit();
	double b = m.dot(dir);
	double c = m.dot(m) - radius * radius;
	//
	if( c > 0.0 && b > 0.0 ) return false;
	double discr = b*b - c;
	// ray is missing sphere
	if(discr < 0.0) return false;
	// we have an intersection
	t = -b - sqrt( discr );
	//
	if( t < 0.0 ) 
	{
		t = 0.0;
		return false;
	}
	// check if this is the closest intersecting shape currently, if yes, then populate the hit obj with relevent data
	if( t < tmin )
	{
		tmin = t;
		h.Position = r.GetOrigin() +  r.GetDir() * tmin; 
		V N=h.Position-pos;
		h.Normal=N.unit();
		//
		h.Obj = this;
	}
	//
	return true;
}
//
enum Axes { AXIS_X, AXIS_Y, AXIS_Z };
bool Box::Intersect( const Ray & r, double &t, Hit& h )
{
	double tmin = 0;
	double tmax = FLT_MAX;
	//
	V p = r.GetOrigin();
	V d = r.GetDir();
	d = d.unit();
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
				if( a == AXIS_X )
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
	V point = p + ( d*(tmin + EPSILON) );
	//
	//check if this is the closest intersecting shape currently, if yes, then populate the hit obj with relevent data
	if(tmin < t)
	{
		//if(a != AXIS_Y) return false;
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
			if(sign[2])  { h.Normal = V(0,0,-1); }
			else {h.Normal = V(0,0,1);  }
		}
		h.Position = point;
		h.Obj = this;
	}
	return true;
}
//
void cylinderHelperF( double &tmin, double t, Hit& h, const Ray& r, Shape& p, int side )
{
	//check if this is the closest intersecting shape currently, if yes, then populate the hit obj with relevent data
	if( t < tmin )
	{
		V N;
		tmin = t;
		h.Position = r.GetOrigin() + r.GetDir() * t; 
	
		if( side == 0 )	//mid section
		{
			V q;
			q = V(p.GetPos().x,p.GetPos().y,h.Position.z); 
			N = h.Position-q;
		}
		else if( side == 1 ) //top plane
		{
			N = V( p.GetPos().x, p.GetPos().y, p.GetPos().z ); 
		}
		else if( side == 2 ) //bottom plane
		{
			N = V( p.GetPos().x, p.GetPos().y,-p.GetPos().z ); 
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
	V n = ray.GetDir();
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
		cylinderHelperF( tmin, t, h, ray, *this, 0 );
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
			cylinderHelperF( tmin, t, h, ray, *this, 2 );
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
		if( k + dd - 2 * md + t * (2 * (mn - nd) + t * nn) <= 0.0)
		{
			cylinderHelperF( tmin, t, h, ray, *this, 1 );
			return true;
		}
		else
			return false;
	}
	//
	// Segment intersects cylinder between the end planes; t is correct
	cylinderHelperF( tmin, t, h, ray, *this, 0 );
	return true;
}

bool Triangle::Intersect( const Ray & r,  double &tmin, Hit& h)
{
	double v, u, w;
	double t;
	V ab = p2 - p1;
	V ac = p3 - p1;
	V qp = r.GetDir();
	qp = qp.unit();
	//
	// Compute triangle normal
	V n = ab.cross(ac);
	//
	double d = qp.dot(n);
	if (d <= 0.0) 
		return false;
	//
	V ap = r.GetOrigin()-p1;
	t = -ap.dot(n);
	if (t < 0.0) return 0;
	//
	V e = qp.cross(ap);
	v = ac.dot(e);
	if (v < 0.0 || v > d) return 0;
	w = -ab.dot(e);
	if (w < 0.0 || v + w > d) return 0;
	//
	double ood = 1.0 / d;
	t *= ood;
	v *= ood;
	w *= ood;
	u = 1.0 - v - w;
	//
	//check if this is the closest intersecting shape currently, if yes, then populate the hit obj with relevent data
	if( t < tmin )
	{
		tmin = t;
		h.Position = r.GetOrigin() + r.GetDir() * t; 
		//
		V ab = p1 - p2;
		V ac = p3 - p1;
		//
		V N = ab.cross(ac);
		h.Normal = N.unit();
		//
		h.Obj = this;
	}
	return 1;
}