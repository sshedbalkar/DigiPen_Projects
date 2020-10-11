#include "Plane.h"

Plane::Plane(Vector4 position, Vector4 normal) : m_position(position), m_normal(normal){}
Plane::~Plane(){}

float Plane::RayIntersect(Vector4 origin, Vector4 ray)
{
	return (m_normal * Vector4(origin - m_position)) / (m_normal * (origin + ray));
}