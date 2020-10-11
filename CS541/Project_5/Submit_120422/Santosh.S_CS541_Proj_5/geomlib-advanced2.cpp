///////////////////////////////////////////////////////////////////////
// $Id$
//
// Geometric objects (Points, Vectors, Planes, ...) and operations.
//
// Gary Herron
//
// Copyright © 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////
#include "geomlib.h"
#include <vector>

////////////////////////////////////////////////////////////////////////
// Distance methods
////////////////////////////////////////////////////////////////////////

// Return the distance from a point to a line.
float Distance(const Point3D& point, const Line3D& line)
{
	Vector3D span = point - line.point;
	return abs(Cross(line.vector, span).length() / line.vector.length());

}

// Return the distance from a point to a plane.
float Distance(const Point3D& point, const Plane3D& plane)
{
	return ((Dot(point, plane.normal()) + plane[3]) / plane.normal().length());
}

////////////////////////////////////////////////////////////////////////
// Containment methods
////////////////////////////////////////////////////////////////////////

// Determines if point (known to be on a line) is contained within a segment.
bool Segment3D::contains(const Point3D& point) const
{
	Vector3D span = point - point1;
	Vector3D v = point2 - point1;
	float l = v.length();
	v.normalize();
	float d = Dot(span, v);
	if(d < 0.0f || d > l)
		return false;
	return true;
//
//	// Separating axis.
//	for(unsigned a = 0; a < 3; a++)
//	{
//		if(point[a] < point1[a] || point[a] > point2[a])
//			return false;
//	}
//	return true;
}

// Determines if point (known to be on a line) is contained within a ray.
bool Ray3D::contains(const Point3D& point, float *t) const
{
	if(!t)
		return false;

	Vector3D nRay = direction;
	nRay.normalize();

	Vector3D span = point - origin;
	*t = Dot(span, nRay)/direction.length();
	if(*t >= 0.0f)
		return true;
	return false;
}

// Determines if point is contained within a box.
bool Box3D::contains(const Point3D& point) const
{
	// Separating axis.
	for(unsigned a = 0; a < 3; a++)
	{
		if(point[a] < origin[a] || point[a] > extent[a])
			return false;
	}
	return true;
}

// Determines if point (known to be on a plane) is contained within a triangle.
bool Triangle3D::contains(const Point3D& point) const
{
	Point3D ccwPts[3];
	ccwPts[0] = *begin();
	ccwPts[1] = *(begin() + 1);
	ccwPts[2] = *(begin() + 2);

	Vector3D normal = Cross(ccwPts[1] - ccwPts[0], ccwPts[2] - ccwPts[0]);
	// Check to se if the point exists in the plane of the triangle.
	if(Dot(point - ccwPts[0], normal) > EPSILON)
		return false;

	Vector3D edge, span;
	for(int p = 0; p < 3; p++)
	{
		int next = (p + 1) % 3;
		edge = ccwPts[next] - ccwPts[p];
		span = point - ccwPts[p];
		
		if(Dot(Cross(edge, span), normal) < 0.0f)
			return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////
// Intersects functions
// In the following Intersects function these rules apply:
//
// * Most test are to determine if a *unique* solution exists. (Or in
//   some cases up to two intersection points exist.)  Parallel
//   objects have either zero or infinitely many solutions and so
//   return false.
//
// * If a unique solution exists, a function value of true is
//   returned.  (Or in the cases where several solutions can exist,
//   the number of intersection points is returned.)
//
// * If a unique solution does exist, the calling program may provide
//   a memory location into which the intersection point can be
//   returned.  Such pointer may be NULL to indicate that the
//   intersection point is not to be returned.
//
////////////////////////////////////////////////////////////////////////

// Determines if 2D segments have a unique intersection.
// If true and rpoint is not NULL, returns intersection point.
// May not have been discussed in class.
bool Intersects(const Segment2D& seg1, const Segment2D& seg2,
				Point2D *rpoint)
{
	if(!rpoint)
		return false;

	Vector2D u = seg1.point2 - seg1.point1;
	Vector2D v = seg2.point2 - seg2.point1;
	Vector2D w = seg1.point1 - seg2.point1;
	
	float a = Dot(u, u);
	float b = Dot(u, v);
	float c = Dot(v, v);
	float d = Dot(u, w);
	float e = Dot(v, w);
	
	if(a*c - b*b == 0)
		return false;

	float sc = (b*e - c*d)/(a*c - b*b);
	float tc = (a*e - b*d)/(a*c - b*b);

	if(sc < 0.0f || sc > 1.0f)
		return false;
	if(tc < 0.0f || tc > 1.0f)
		return false;

	Vector2D pSc = seg1.point1 + sc * u;
	Vector2D pTc = seg2.point1 + tc * v;

	if((pSc - pTc).length() > EPSILON)
		return false;
	
	*rpoint = pSc;
	return true;
}

// Determines if 2D lines have a unique intersection.
// If true and rpoint is not NULL, returns intersection point.
// May not have been discussed in class.
bool Intersects(const Line2D& line1, const Line2D& line2,
				Point2D *rpoint)
{
	if(!rpoint)
		return false;

	Vector2D u = line1.vector;
	Vector2D v = line2.vector;
	Vector2D w = line1.point - line2.point;
	
	float a = Dot(u, u);
	float b = Dot(u, v);
	float c = Dot(v, v);
	float d = Dot(u, w);
	float e = Dot(v, w);
	
	if(a*c - b*b == 0)
		return false;

	float sc = (b*e - c*d)/(a*c - b*b);
	float tc = (a*e - b*d)/(a*c - b*b);

	Vector2D pSc = line1.point + sc * u;
	Vector2D pTc = line2.point + tc * v;

	if((pSc - pTc).length() > EPSILON)
		return false;

	*rpoint = pSc;
	return true;
}

// Determines if 3D line and plane have a unique intersection.  
// If true and rpoint is not NULL, returns intersection point.
bool Intersects(const Line3D& line, const Plane3D& plane, Point3D *rpoint)
{
	if(!rpoint)
		return false;

	if(abs(Dot(line.vector, plane.normal())) <= EPSILON)
		return false;

	//Vector3D o(	plane[0]/plane.normal()[0], 
	//			plane[1]/plane.normal()[1],
	//			plane[2]/plane.normal()[2] );

	//float t = (Dot(o, plane.normal()) - Dot(line.point, plane.normal()) - plane[3]) / Dot(line.vector, plane.normal());
	//*rpoint = line.point + line.vector * t;

	float t = (-Dot(plane.normal(), line.point) - plane[3]) / Dot(plane.normal(), line.vector);
	*rpoint = line.point + line.vector * t;
	return true;
}

// Determines if 3D segment and plane have a unique intersection.  
// If true and rpoint is not NULL, returns intersection point.
bool Intersects(const Segment3D& seg, const Plane3D& plane, Point3D *rpoint)
{
	if(!rpoint)
		return false;
	
	Vector3D v = seg.point2 - seg.point1;
	if(abs(Dot(v, plane.normal())) <= EPSILON)
		return false;

	float t = (-Dot(plane.normal(), seg.point1) - plane[3]) / Dot(plane.normal(), v);
	if(t < 0.0f || t > 1.0f)
		return false;

	*rpoint = seg.point1 + v * t;
	return true;

}

// Determines if 3D segment and triangle have a unique intersection.  
// If true and rpoint is not NULL, returns intersection point.
bool Intersects(const Segment3D& seg, const Triangle3D& tri, Point3D *rpoint, float* time)
{
	//Vector3D normal = Cross(tri[1] - tri[0], tri[2] - tri[0]);
	//normal.normalize();

	//float d = Dot(normal, tri[0]);

	//Plane3D plane(normal[0], normal[1], normal[2], d);

	//Vector3D intersect;
	//if(!Intersects(seg, plane, &intersect))
	//	return false;

	//if(!tri.contains(intersect))
	//	return false;

	//*rpoint = intersect;
	//return true;

	Vector3D rayDelta = seg.point2 - seg.point1;
	rayDelta.normalize();

	Vector3D n = Cross(tri[1] - tri[0], tri[2] - tri[0]);
	//n.normalize();

	float dot = Dot(n, rayDelta);

	if(!(dot < 0.0f))
		return false;

	float d = Dot(n, tri[2]);

	float t = d - Dot(n, seg.point1);

	if(!(t <= 0.0f))
		return false;

	t /= dot;

	Point3D p = seg.point1 + rayDelta * t;

	if(!tri.contains(p))
		return false;

	if(time)
		*time = t;
	
	return true;

	
}

// Determines if 3D ray and sphere have 0, 1, or 2 intersections.  
// If any exist and rpoint is not NULL, returns intersection point(s).
int Intersects(const Ray3D& ray, const Sphere3D& sphere,
			   std::pair<Point3D, Point3D> *rpoints)
{
	if(!rpoints)
		return 0;

	Point3D m = ray.origin;
	Vector3D v = ray.direction;
	Point3D y = sphere.center;
	float r = sphere.radius;

	float a = Dot(v, v);
	float b = 2*Dot(v,(m-y));
	float c = Dot((m-y),(m-y)) - r*r;

	float delta = b*b - 4*a*c;
	float sqrtDelta = sqrt(delta);
	std::pair<float, float> time;
	time.first = (-b + sqrtDelta) / (2*a);
	time.second = (-b - sqrtDelta) / (2*a);

	rpoints->first = ray.origin + time.first * ray.direction;
	rpoints->second = ray.origin + time.second* ray.direction;

	if(delta < 0.0f)
		return 0;
	if(delta == 0.0f)
		return 1;
	else
		return 2;
}

// Determines if 3D ray and triangle have a unique intersection.  
// If true and rpoint is not NULL, returns intersection point.
bool Intersects(const Ray3D& ray, const Triangle3D& tri, Point3D *rpoint)
{
	if(!rpoint)
		return false;

	Vector3D normal = Cross(tri[1] - tri[0], tri[2] - tri[0]);
	normal.normalize();

	float d = -Dot(normal, tri[0]);

	Plane3D plane(normal[0], normal[1], normal[2], d);

	Vector3D intersect;
	Segment3D seg;
	seg.point1 = ray.origin;
	seg.point2 = ray.origin + (float)(unsigned)-1 * ray.direction;
	if(!Intersects(seg, plane, &intersect))
		return false;

	if(!tri.contains(intersect))
		return false;

	*rpoint = intersect;
	return true;
}

// Determines if 3D ray and AABB have a 0, 1, or 2 intersections.  
// If any exist and rpoint is not NULL, returns intersection point(s).
int Intersects(const Ray3D& ray, const Box3D& box,
			   std::pair<Point3D, Point3D> *rpoints)
{

	Vector3D min = box.origin - box.extent;
	Vector3D max = box.origin + box.extent;
	float t = 0.0f;
	float maxt = -999.0f;
	int maxa = 0;
	for(unsigned a = 0; a < 3; a++)
	{
		if(ray.origin[a] < min[a])
		{
			t = min[a] - ray.origin[a];
			if(t > ray.direction[a])
				return false;
			t /= ray.direction[a];
		}
		else if(ray.origin[a] > max[a])
		{
			t = max[a] - ray.origin[a];
			if(t < ray.direction[a])
				return false;
			t /= ray.direction[a];
		}
		else 
			t = -1.0f;

		if(t > maxt)
		{
			maxt = t;
			maxa = a;
		}
	}

	float x, y, z;
	switch(maxa)
	{
	case 0:
		y = ray.origin[1] + ray.direction[1] * maxt;
		if( y < min[1] || y > max[1] )
			return false;
		z = ray.origin[2] + ray.direction[2] * maxt;
		if( z < min[2] || z > max[2] )
			return false;
		break;
	case 1:
		x = ray.origin[0] + ray.direction[0] * maxt;
		if( x < min[0] || x > max[0] )
			return false;
		z = ray.origin[2] + ray.direction[2] * maxt;
		if( z < min[2] || z > max[2] )
			return false;
		break;
	case 2:
		x = ray.origin[0] + ray.direction[0] * maxt;
		if( x < min[0] || x > max[0] )
			return false;
		y = ray.origin[1] + ray.direction[1] * maxt;
		if( y < min[1] || y > max[1] )
			return false;
		
		break;
	}

	return true;

	Vector3D v = ray.direction;
	Point3D m = ray.origin;
	std::pair<Vector3D, Vector3D> times;
	for(unsigned a = 0; a < 3; a++)
	{
		times.first[a] = (box.origin[a] - ray.origin[a]) / ray.direction[a];
		times.second[a] = (box.extent[a] - ray.origin[a]) / ray.direction[a];
	}
	
	std::pair<float, float> time;
	time.first = MAX(MAX(times.first[0], times.first[1]), times.first[2]);
	time.second = MIN(MIN(times.second[0], times.second[1]), times.second[2]);

	if(time.first > time.second)
	{
		float t = time.first;
		time.first = time.second;
		time.second = t;
	}

	rpoints->first = ray.origin + time.first * ray.direction;
	rpoints->second = ray.origin + time.second * ray.direction;



	if(time.first < 0.0f && time.second < 0.0f)
		return 0;
	else if(time.first < 0.0f && time.second >= 0.0f)
		return 1;
	else
		return 2;
}

// Determines if 3D triangles intersect.  
// If parallel, returns false. (This may be considered misleading.)
// If true and rpoint is not NULL, returns two edge/triangle intersections.
int Intersects(const Triangle3D& tri1, const Triangle3D& tri2,
		   std::pair<Point3D, Point3D> *rpoints)
{
	if(!rpoints)
		return false;

	// Check for parallelism
	Vector3D n1 = Cross(tri1[1] - tri1[0], tri1[2] - tri1[0]);
	Vector3D n2 = Cross(tri2[1] - tri2[0], tri2[2] - tri2[0]);
	n1.normalize();
	n2.normalize();
	if(n1 == n2)
		return false;
	
	Vector3D pt;

	unsigned intersects = 0;
	for(unsigned p = 0; p < 3; p++)
	{
		Segment3D s(tri1[(p + 1) % 3], tri1[p]);
		
		if(Intersects(s, tri2, &pt))
		{
			if(intersects == 0)
				rpoints->first = pt;
			else 
				rpoints->second = pt;
			intersects++;
		}
	}

	for(unsigned p = 0; p < 3; p++)
	{
		Segment3D s(tri2[(p + 1) % 3], tri2[p]);
		
		if(Intersects(s, tri1, &pt))
		{
			if(intersects == 0)
				rpoints->first = pt;
			else 
				rpoints->second = pt;
			intersects++;
		}
	}

	return intersects;
}

////////////////////////////////////////////////////////////////////////
// Geometric relationships
////////////////////////////////////////////////////////////////////////

// Compute angle between two geometric entities (in radians;  use acos)
float AngleBetween(const Line3D& line1, const Line3D& line2)
{
	Vector3D u = line1.vector;
	Vector3D v = line2.vector;
	u.normalize();
	v.normalize();
	return acos(Dot(u, v));
}

// Compute angle between two geometric entities (in radians;  use acos)
float AngleBetween(const Line3D& line, const Plane3D& plane)
{
	Vector3D u = line.vector;
	Vector3D v = plane.normal();

	u.normalize();
	v.normalize();

	return asin(Dot(u, v));
}

// Compute angle between two geometric entities (in radians;  use acos)
float AngleBetween(const Plane3D& plane1, const Plane3D& plane2)
{
	Vector3D u = plane1.normal();
	Vector3D v = plane2.normal();

	u.normalize();
	v.normalize();

	return acos(Dot(u, v));
}

// Determine if two lines are coplanar
bool Coplanar(const Line3D& line1, const Line3D& line2)
{
	if(line1.point == line2.point)
		return true;

	Vector3D u = line1.vector;
	Vector3D v = line2.vector;
	Vector3D a = line1.point - line2.point;

	Vector3D n = Cross(u, v);
	
	float f = Dot(a, n);
	if( abs(Dot(a, n)) <= EPSILON )
		return true;
	return false;
}

// Determine if two geometric entities are parallel.
bool Parallel(const Line3D& line1, const Line3D& line2)
{
	if( abs(Cross(line1.vector, line2.vector).length()) <= EPSILON )
		return true;
	return false;
}

// Determine if two geometric entities are parallel.
bool Parallel(const Line3D& line, const Plane3D& plane)
{
	if( abs(Dot(line.vector, plane.normal())) <= EPSILON )
		return true;
	return false;
}

// Determine if two geometric entities are parallel.
bool Parallel(const Plane3D& plane1, const Plane3D& plane2)
{
	if( abs(Cross(plane1.normal(), plane2.normal()).length()) <= EPSILON )
		return true;
	return false;
}

// Determine if two geometric entities are perpendicular.
bool Perpendicular(const Line3D& line1, const Line3D& line2)
{
	if(abs(Dot(line1.vector, line2.vector)) <= EPSILON)
		return true;
	return false;
}

// Determine if two geometric entities are perpendicular.
bool Perpendicular(const Line3D& line, const Plane3D& plane)
{
	if(abs(Cross(line.vector, plane.normal()).length()) <= EPSILON)
		return true;
	return false;
}

// Determine if two geometric entities are perpendicular.
bool Perpendicular(const Plane3D& plane1, const Plane3D& plane2)
{
	if(abs(Dot(plane1.normal(), plane2.normal())) <= EPSILON)
		return true;
	return false;
}
