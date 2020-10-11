#pragma once
#include "Vector4.h"

class Plane
{
public:
			Plane(){}
			Plane(Vector4 position, Vector4 normal);
			~Plane();
	float	RayIntersect(Vector4 origin, Vector4 ray);

	void	SetPosition(Vector4 position){m_position = position;}
	Vector4	GetPosition(){return m_position;}
	void	SetNormal(Vector4 normal){normal.Normalize(); m_normal = normal;}
	Vector4	GetNormal(){return m_normal;}
	void	SetHeight(float height){m_height = height;}
	float	GetHeight(){return m_height;}
	void	SetWidth(float width){m_width = width;}
	float	GetWidth(){return m_width;}

private:
	Vector4		m_position;
	Vector4		m_normal;

	float		m_width;
	float		m_height;
};