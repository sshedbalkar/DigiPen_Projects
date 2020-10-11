///////////////////////////////////////////////////////////////////////
// $Id$
//
// Geometric objects (Points, Vectors, Planes, ...) and operations.
//
// Gary Herron, Pohung Chen
//
// Copyright © 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include "geomlib.h"
#include <vector>
#include <cassert>
#include <float.h> // FLT_EPSILON

////////////////////////////////////////////////////////////////////////
// Distance methods
////////////////////////////////////////////////////////////////////////

// Return the distance from a point to a line.
float Distance(const Point3D& point, const Line3D& line)
{
	return Cross(line.vector, point - line.point).length() / line.vector.length();
}

// Return the distance from a point to a plane.
float Distance(const Point3D& point, const Plane3D& plane)
{
  return std::fabs(Dot(plane.normal(), point) + plane[3]);
}

////////////////////////////////////////////////////////////////////////
// Containment methods
////////////////////////////////////////////////////////////////////////

// Determines if point (known to be on a line) is contained within a segment.
bool Segment3D::contains(const Point3D& point) const
{
  return (point2[0] - point1[0] > 0.f && point[0] >= point1[0] && point[0] <= point2[0]) ||
    (point2[0] - point1[0] < 0.f && point1[0] >= point[0] && point[0] >= point2[0]);
}

// Determines if point (known to be on a line) is contained within a ray.
bool Ray3D::contains(const Point3D& point, float *t) const
{
	float tVal = (point - origin).length() / direction.length();
  float d = Dot(point, origin + direction);
  if (t && d > FLT_EPSILON) *t = tVal;
  return d > FLT_EPSILON;
}

// Determines if point is contained within a box.
bool Box3D::contains(const Point3D& point) const
{
	return point[0] >= center[0] - extents[0] && point[0] <= center[0] + extents[0] &&
         point[1] >= center[1] - extents[1] && point[1] <= center[1] + extents[1] &&
         point[2] >= center[2] - extents[2] && point[2] <= center[2] + extents[2];
}

// Determines if point (known to be on a plane) is contained within a triangle.
bool Triangle3D::contains(const Point3D& point) const
{
  Vector3D normal = Cross(points[1] - points[0], points[2] - points[0]);
	if (Dot(point - points[0], Cross(normal, points[1] - points[0])) < -FLT_EPSILON)
    return false;
  if (Dot(point - points[1], Cross(normal, points[2] - points[1])) < -FLT_EPSILON)
    return false;
  if (Dot(point - points[2], Cross(normal, points[0] - points[2])) < -FLT_EPSILON)
    return false;
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
  // this should also work in the 3d case. might be a faster way to do this
  // but until I run into performance issues I won't worry too much about this.
  Vector2D v1 = seg1.point2 - seg1.point1;
  Vector2D v2 = seg2.point2 - seg2.point1;
	float t = (seg2.point1 - seg1.point1).length() / (v1 - v2).length();
  if (t > 0 && t < 1)
  {
    *rpoint = seg1.point1 + t * v1;
    return true;
  }
  return false;
}

// Determines if 2D lines have a unique intersection.
// If true and rpoint is not NULL, returns intersection point.
// May not have been discussed in class.
bool Intersects(const Line2D& line1, const Line2D& line2,
				Point2D *rpoint)
{
  Vector2D v1 = line1.vector.normalized();
  Vector2D v2 = line2.vector.normalized();
  if (std::fabs(Dot(v1, v2)) + FLT_EPSILON > 1.f ) return false;

  if (rpoint)
  {
    float denom = line2.vector[1] * (line1.vector[0]) - line2.vector[0] * (line1.point[1] - line1.vector[1]);
    if (fabs(denom) < FLT_EPSILON) return false;
    float t = line2.vector[0] * (line1.point[1] - line2.point[1]) - line2.vector[1] * (line1.point[0] - line2.point[0]);
    t /= denom;
    *rpoint = line1.point + t * line1.vector;
  }
  return true;

}

// Determines if 3D line and plane have a unique intersection.  
// If true and rpoint is not NULL, returns intersection point.
bool Intersects(const Line3D& line, const Plane3D& plane, Point3D *rpoint)
{
  float denom = Dot(line.vector, plane.normal());
	if (fabs(denom) < FLT_EPSILON) return false;
  
  if (rpoint)
  {
    float t = (-Dot(line.point, plane.normal()) - plane[3]) / denom;
    *rpoint = line.point + t * line.vector;
  }
  return true;
}

// Determines if 3D segment and plane have a unique intersection.  
// If true and rpoint is not NULL, returns intersection point.
bool Intersects(const Segment3D& seg, const Plane3D& plane, Point3D *rpoint)
{
  Vector3D v = seg.point1 - seg.point2;
  float denom = Dot(v, plane.normal());
  if (fabs(denom) < FLT_EPSILON) return false;

  float t = (-Dot(seg.point2, plane.normal()) - plane[3]) / denom;
  if ( t < 0.f || t > 1.f) return false;

  if (rpoint)
  {
    *rpoint = seg.point2 + t * v;
  }
  return true;
}

bool Intersects(const Triangle3D& tri, const AABB& box)
{
  for (unsigned i = 0; i < 3; ++i)
  {
    if ((tri[i][0] >= box.m_min[0] && tri[i][0] <= box.m_max[0]
     && tri[i][1] >= box.m_min[1] && tri[i][1] <= box.m_max[1]
     || tri[i][2] >= box.m_min[2] && tri[i][2] <= box.m_max[2]))
     {
       return true;
     }
  }
  return false;
}

// Determines if 3D segment and triangle have a unique intersection.  
// If true and rpoint is not NULL, returns intersection point.
bool Intersects(const Segment3D& seg, const Triangle3D& tri, Point3D *rpoint)
{
  Point3D intersection;
  Vector3D v1 = tri[1] - tri[0];
  Vector3D v2 = tri[2] - tri[0];
  Vector3D n = Cross(v1, v2);
  Plane3D triPlane(n[0], n[1], n[2], -Dot(tri[2], n));
  if (Intersects(seg, triPlane, &intersection) == false)
  {
    return false;
  }
  if (tri.contains(intersection))
  {
    if (rpoint) *rpoint = intersection;
    return true;
  }
  return false;

}

// Determines if 3D ray and sphere have 0, 1, or 2 intersections.  
// If any exist and rpoint is not NULL, returns intersection point(s).
int Intersects(const Ray3D& ray, const Sphere3D& sphere,
			   std::pair<Point3D, Point3D> *rpoints)
{
	Vector3D sphereToRay = ray.origin - sphere.center;
  float a = Dot(ray.direction, ray.direction);
  float b = 2 * Dot(sphereToRay, ray.direction);
  float c = Dot(sphereToRay, sphereToRay) - sphere.radius * sphere.radius;

  float disc = b * b - 4 * a * c;
  if (fabs(a) < FLT_EPSILON || disc < 0.f) return 0;

  float t1 = (-b + sqrt(disc)) / (2 * a);
  float t2 = (-b - sqrt(disc)) / (2 * a);

  if (t1 > 0.f && t2 < 0.f)
  {
    if (rpoints)
    {
      rpoints->first = ray.origin + t1 * ray.direction;
    }
    return 1;
  }

  if (t2 > 0.f && t1 < 0.f)
  {
    if (rpoints)
    {
      rpoints->second = ray.origin + t2 * ray.direction;
    }
    return 1;
  }

  if (rpoints)
  {
    rpoints->first = ray.origin + t1 * ray.direction;
    rpoints->second = ray.origin + t2 * ray.direction;
  }
  return 2;

}

// Determines if 3D ray and triangle have a unique intersection.  
// If true and rpoint is not NULL, returns intersection point.
bool Intersects(const Ray3D& ray, const Triangle3D& tri, Point3D *rpoint)
{
  Vector3D ab = tri[1] - tri[0];
  Vector3D ac = tri[2] - tri[0];
  Vector3D qp = ray.direction;

  Vector3D n = Cross(ac, ab);

  float d = -Dot(qp, n);
  if (d <= 0.f)
  {
    return false;
  }

  Vector3D e = Cross(ray.origin - tri[0], qp);

  float v = -Dot(ac, e);

  if ( v < 0.f || v > d )
  {
    return false;
  }

  float w = Dot(ab, e);

  if (w < 0.f || v + w > d )
  {
    return false;
  }

  if (rpoint)
  {
    float denom = 1.f / d;
    v *= denom;
    w *= denom;
    float u = 1.f - v - w;
    *rpoint = u * tri[0] + v * tri[1] + w * tri[2];
  }

  return true;

  //Point3D intersection;
  //Vector3D v1 = tri[1] - tri[0];
  //Vector3D v2 = tri[2] - tri[0];
  //Vector3D n = Cross(v1, v2);
  //Plane3D plane(n[0], n[1], n[2], -Dot(tri[2], n));
  //if (Intersects(Line3D(ray.origin, ray.direction), plane, &intersection) == false)
  //{
  //  return false;
  //}
  //if (tri.contains(intersection) == false)
  //{
  //  return false;
  //}

  //if (ray.contains(intersection) == false)
  //{
  //  return false;
  //}

  //if (rpoint)
  //{
  //  *rpoint = intersection;
  //}

  //return true;

}

// Determines if 3D ray and AABB have a 0, 1, or 2 intersections.  
// If any exist and rpoint is not NULL, returns intersection point(s).
int Intersects(const Ray3D& ray, const AABB& box,
			   std::pair<Point3D, Point3D> *rpoints)
{
	float tmin = -FLT_MAX;
  float tmax = FLT_MAX;
  for ( unsigned i = 0; i < 3; ++i )
  {
    float boxMin = box.m_min[i];
    float boxMax = box.m_max[i];
    if (fabs(ray.direction[i]) < FLT_EPSILON)
    {
      if (ray.origin[i] < boxMin || 
          ray.origin[i] > boxMax)
      {
        return 0;
      }
    } else
    {
      float t1 = (boxMin - ray.origin[i]) / ray.direction[i];
      float t2 = (boxMax - ray.origin[i]) / ray.direction[i];

      if ( t1 > t2 ) std::swap(t1, t2);
      tmin = std::max(tmin, t1);
      tmax = std::min(tmax, t2);
      if ( tmin > tmax )
      {
        return 0;
      }
    }
  }

  if (tmax < 0.f) return 0;

  if (tmin < 0.f)
  {
    if (rpoints)
    {
      rpoints->first = ray.origin + tmax * ray.direction;
    }
    return 1;
  }

  if (rpoints)
  {
    rpoints->second = ray.origin + tmax * ray.direction;
    rpoints->first = ray.origin + tmin * ray.direction;
  }
  return 2;
}

int Intersects(const Ray3D& ray, const AABB& box,
               std::pair<float, float> *rpoints)
{
  float tmin = -FLT_MAX;
  float tmax = FLT_MAX;
  for ( unsigned i = 0; i < 3; ++i )
  {
    float boxMin = box.m_min[i];
    float boxMax = box.m_max[i];
    if (fabs(ray.direction[i]) < FLT_EPSILON)
    {
      if (ray.origin[i] < boxMin || 
        ray.origin[i] > boxMax)
      {
        return 0;
      }
    } else
    {
      float t1 = (boxMin - ray.origin[i]) / ray.direction[i];
      float t2 = (boxMax - ray.origin[i]) / ray.direction[i];

      if ( t1 > t2 ) std::swap(t1, t2);
      tmin = std::max(tmin, t1);
      tmax = std::min(tmax, t2);
      if ( tmin > tmax )
      {
        return 0;
      }
    }
  }

  if (tmax < 0.f) return 0;

  if (tmin < 0.f)
  {
    if (rpoints)
    {
      rpoints->first = tmax;
    }
    return 1;
  }

  if (rpoints)
  {
    rpoints->second = tmax;
    rpoints->first = tmin;
  }
  return 2;
}


// Determines if 3D triangles intersect.  
// If parallel, returns false. (This may be considered misleading.)
// If true and rpoint is not NULL, returns two edge/triangle intersections.
int Intersects(const Triangle3D& tri1, const Triangle3D& tri2,
		   std::pair<Point3D, Point3D> *rpoints)
{
  unsigned clipCount = 0;
  Vector3D intPoint;
  Segment3D edges[3];
  edges[0] = Segment3D(tri1[0],tri1[1]);
  edges[1] = Segment3D(tri1[1],tri1[2]);
  edges[2] = Segment3D(tri1[2],tri1[0]);
  for (unsigned i = 0; i < 3; ++i)
  {
    if (Intersects(edges[i], tri2, &intPoint))
    {
      if (rpoints)
      {
        if (clipCount == 0)
        {
          rpoints->first = intPoint;
        } else
        {
          rpoints->second = intPoint;
        }
      }
      if (clipCount == 1)
      {
        return 2;
      }
      clipCount++;
    }
  }

  edges[0] = Segment3D(tri2[0],tri2[1]);
  edges[1] = Segment3D(tri2[1],tri2[2]);
  edges[2] = Segment3D(tri2[2],tri2[0]);
  for (unsigned i = 0; i < 3; ++i)
  {
    if (Intersects(edges[i], tri1, &intPoint))
    {
      if (rpoints)
      {
        if (clipCount == 0)
        {
          rpoints->first = intPoint;
        } else
        {
          rpoints->second = intPoint;
        }
      }
      if (clipCount == 1)
      {
        return 2;
      }
      clipCount++;
    }
  }

  return clipCount;

}

////////////////////////////////////////////////////////////////////////
// Geometric relationships
////////////////////////////////////////////////////////////////////////

// Compute angle between two geometric entities (in radians;  use acos)
float AngleBetween(const Line3D& line1, const Line3D& line2)
{
  return std::acos(Dot(line1.vector, line2.vector));
}

// Compute angle between two geometric entities (in radians;  use acos)
float AngleBetween(const Line3D& line, const Plane3D& plane)
{
  return 90.f - std::acos(Dot(line.vector, plane.normal()));
}

// Compute angle between two geometric entities (in radians;  use acos)
float AngleBetween(const Plane3D& plane1, const Plane3D& plane2)
{
  return std::acos(Dot(plane1.normal(), plane2.normal()));
}

// Determine if two vectors are parallel.
bool Parallel(const Vector3D& v1, const Vector3D& v2)
{
  return (std::fabs(Dot(v1,v2)) + FLT_EPSILON) > 1.f;
}

bool Perpendicular(const Vector3D& v1, const Vector3D& v2)
{
  return std::fabs(Dot(v1, v2)) < FLT_EPSILON;
}

// Determine if two lines are coplanar
bool Coplanar(const Line3D& line1, const Line3D& line2)
{
  // Get the cross product.
	const Vector3D& cross = Cross(line1.vector, line2.vector);

  // project each point along the crossed vector to see if they're on the
  // same point.
  return fabs(Dot(line1.point, cross) - Dot(line2.point, cross)) < FLT_EPSILON;
}

// Determine if two geometric entities are parallel.
bool Parallel(const Line3D& line1, const Line3D& line2)
{
	return Parallel(line1.vector, line2.vector);
}

// Determine if two geometric entities are parallel.
bool Parallel(const Line3D& line, const Plane3D& plane)
{
	return Perpendicular(line.vector, plane.normal());
}

// Determine if two geometric entities are parallel.
bool Parallel(const Plane3D& plane1, const Plane3D& plane2)
{
	return Parallel(plane1.normal(), plane2.normal());
}

// Determine if two geometric entities are perpendicular.
bool Perpendicular(const Line3D& line1, const Line3D& line2)
{
  return Perpendicular(line1.vector, line2.vector);
}

// Determine if two geometric entities are perpendicular.
bool Perpendicular(const Line3D& line, const Plane3D& plane)
{
	return Parallel(line.vector, plane.normal());
}

// Determine if two geometric entities are perpendicular.
bool Perpendicular(const Plane3D& plane1, const Plane3D& plane2)
{
  return Perpendicular(plane1.normal(), plane2.normal());
}
