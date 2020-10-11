/**
 *	\file	Camera.cpp
 *  \author David Yamanoha
 *  \date   08.08.08
 */
#include "Camera.h"
#include <math.h>
#include <iostream>

/**
*	Constructor. Default angles are set to 0 degrees.  Near-plane defaults to 1.0 units, and far-plane defaults to 
*	100.0 units.
*	\returns n/a.
*	\callergraph
*	\callgraph
*/
Camera::Camera() : m_vAngle(3.14f/2.0f), m_hAngle(0), m_nearPlaneDistance(1.0f), m_farPlaneDistance(1000.0f)
{
	m_nearPlane.SetPosition(GetOrientation());
	SetOrientation(GetOrientation());
}

/**
*	Destructor
*	\returns n/a.
*	\callergraph
*	\callgraph
*/
Camera::~Camera(){}

/** 
*	Displaces the camera from it's current location.
*	\param x the amount of displacement along the x-axis of the global coordinate system.
*	\param y the amount of displacement along the y-axis of the global coordinate system.
*	\param z the amount of displacement along the z-axis of the global coordinate system.
*	\returns void.
*	\callergraph
*	\callgraph
*/
void Camera::SetTranslation(float x, float y, float z)
{
	Vector4 translation(x, y, z);
	m_cs.SetTranslation(translation);
}

/** 
*	Displaces the camera from it's current location.  Homogenous coordinate is ignored.
*	\param v the amount of displacement within the global coordinate system represented as as a numeric touple.
*	\returns void.
*	\callergraph
*	\callgraph
*/
void Camera::SetTranslation(Vector4 v)
{
	SetTranslation(v.x, v.y, v.z);
}

/** 
*	Sets the camera's position.  Homogenous coordinate is ignored.
*	\param x the amount of displacement along the x-axis of the global coordinate system.
*	\param y the amount of displacement along the y-axis of the global coordinate system.
*	\param z the amount of displacement along the z-axis of the global coordinate system.
*	\returns void.
*	\callergraph
*	\callgraph
*/
void Camera::SetPosition(float x, float y, float z)
{
	m_cs.SetPosition(Vector4(x, y, z, 1.0f));
}

/** 
*	Sets the camera's position.  Homogenous coordinate is ignored.
*	\param p the amount of displacement within the global coordinate system represented as as a numeric touple.
*	\returns void.
*	\callergraph
*	\callgraph
*/
void Camera::SetPosition(Vector4 p)
{
	SetPosition(p.x, p.y, p.z);
}

/** 
*	Sets the camera's orientation.  The values passed to this function represent the new 'look-at' direction for the
*	camera.
*	\param x look-at x-component.
*	\param y look-at y-component.
*	\param z look-at z-component.
*	\returns void.
*	\callergraph
*	\callgraph
*/
void Camera::SetOrientation(float x, float y, float z)
{
	m_cs.SetOrientation(Vector4(x, y, z));
	m_nearPlane.SetPosition(GetPosition() + GetOrientation() * m_nearPlaneDistance);
	m_nearPlane.SetNormal(GetOrientation());
	m_farPlane.SetPosition(GetPosition() + GetOrientation() * m_farPlaneDistance);
	m_farPlane.SetNormal(GetOrientation());
}

/** 
*	Sets the camera's orientation.  The vector passed to this function represents the new 'look-at' direction for the
*	camera.
*	\param orientation look-at direction.
*	\returns void.
*	\callergraph
*	\callgraph
*/
void Camera::SetOrientation(Vector4 orientation)
{
	SetOrientation(orientation.x, orientation.y, orientation.z);
}

/** 
*	Sets the camera's near-plane distance.  This distance is relative to the origin of the camera, displaced 
*	<i>distance</i> units along the orientation vector of the camera.
*	\param distance Displacement from camera origin of the near-plane.
*	\returns void.
*	\callergraph
*	\callgraph
*/
void Camera::SetNearPlaneDistance(float distance)
{
	m_nearPlaneDistance = distance;
}

/** 
*	Gets the camera's near-plane distance.  This distance is relative to the origin of the camera, displaced 
*	<i>distance</i> units along the orientation vector of the camera.
*	\returns Displacement from camera origin of the near-plane.
*	\callergraph
*	\callgraph
*/
float Camera::GetNearPlaneDistance()
{
	return m_nearPlaneDistance;
}

/** 
*	Sets the camera's far-plane distance.  This distance is relative to the origin of the camera, displaced 
*	<i>distance</i> units along the orientation vector of the camera.
*	\param distance Displacement from camera origin of the far-plane.
*	\returns void.
*	\callergraph
*	\callgraph
*/
void Camera::SetFarPlaneDistance(float distance)
{
	m_farPlaneDistance = distance;
	m_farPlane.SetWidth(m_nearPlane.GetWidth()/m_nearPlaneDistance * m_farPlaneDistance);
	m_farPlane.SetHeight(m_nearPlane.GetHeight()/m_nearPlaneDistance * m_farPlaneDistance);
}

/** 
*	Gets the camera's far-plane distance.  This distance is relative to the origin of the camera, displaced 
*	<i>distance</i> units along the orientation vector of the camera.
*	\returns Displacement from camera origin of the far-plane.
*	\callergraph
*	\callgraph
*/
float Camera::GetFarPlaneDistance()
{
	return m_farPlaneDistance;
}


/** 
*	Gets the camera's near-plane.
*	\returns Pointer to the camera's near-plane.
*	\callergraph
*	\callgraph
*/
Plane* Camera::GetNearPlane()
{
	return &m_nearPlane;
}


/** 
*	Gets the camera's far-plane.
*	\returns Pointer to the camera's far-plane.
*	\callergraph
*	\callgraph
*/
Plane* Camera::GetFarPlane()
{
	return &m_farPlane;
}

/** 
*	Gets the camera's coordinate system.
*	\returns Pointer to the camera's coordinate system.
*	\callergraph
*	\callgraph
*/
CoordinateSystem Camera::GetCoordinateSystem()
{
	return m_cs;
}

/** 
*	Translates the camera relative to the it's own coordinate system.
*	\param x the amount of displacement along the x-axis of the global coordinate system.
*	\param y the amount of displacement along the y-axis of the global coordinate system.
*	\param z the amount of displacement along the z-axis of the global coordinate system.
*	\returns void.
*	\callergraph
*	\callgraph
*/
void Camera::TranslateLocal(float x, float y, float z)
{
	m_cs.SetLocalTranslation(Vector4(x, y, z, 0.0f));
}

/** 
*	Displaces the camera from it's current location.  Homogenous coordinate is ignored.
*	\param v the amount of displacement within the local coordinate system represented as as a numeric touple.
*	\returns void.
*	\callergraph
*	\callgraph
*/
void Camera::TranslateLocal(Vector4 v)
{
	TranslateLocal(v.x, v.y, v.z);
}

/** 
*	Rotates the camera based upon 2-dimensional input. (Usually mouse position.)
*	\param deltaX The amount of desired change along the x-axis.
*	\param deltaY The amount of desired change along the y-axis.
*	\returns void.
*	\callergraph
*	\callgraph
*/
void Camera::SphericalRotate(int deltaX, int deltaY)
{
	if(!deltaX && !deltaY)
		return;

	m_hAngle += (float)deltaX * 3.14f/180;
	m_vAngle += (float)deltaY * 3.14f/180;

	float x	= cos(m_hAngle) * sin(m_vAngle);
	float y = 0.0f;
	float z = 0.0f;
#ifdef RHC
		z = cos(m_vAngle);
		y = sin(m_hAngle) * sin(m_vAngle);
#else
		y = cos(m_vAngle);
		z = sin(m_hAngle) * sin(m_vAngle);
#endif


	Vector4 position(x, y, z);
	position *= GetPosition().Magnitude();;
	m_cs.SetPosition(position);

	Vector4 orientation = -position;
	m_cs.SetOrientation(orientation);

	m_nearPlane.SetPosition(position + orientation * m_nearPlaneDistance);
	m_nearPlane.SetNormal(orientation);
}

/** 
*	Gets the current position of the camera in the world's coordinate system.
*	\returns Camera's world position.
*	\callergraph
*	\callgraph
*/
Vector4 Camera::GetPosition()
{
	return m_cs.GetTranslation();
}

/** 
*	Gets the current orientation of the camera in the world's coordinate system.
*	\returns Camera's world orientation.
*	\callergraph
*	\callgraph
*/
Vector4 Camera::GetOrientation()
{
	return m_cs.GetYAxis();
}

/** 
*	Computes a ray originating from the camera's origin, through a point on the camera's near plane. (Usually used for 
*	picking.)
*	\param	screenX Screen coordinate mouse x-position.
*	\param	screenY Screen coordinate mouse y-position.
*	\param	cs Desired coordiante system to have the ray returned in.  Options are CS_LOCAL and CS_WORLD.  Default is 
*			CS_WORLD.
*	\returns	Ray cast in the requested reference frame.
*	\callergraph
*	\callgraph
*/
Vector4 Camera::GetRayCast(int screenX, int screenY, COORDINATE_SPACE cs)
{	
	float fscreenX = (float)(screenX - m_nearPlane.GetWidth()/2) / (m_nearPlane.GetWidth()/2);
	float fscreenY = (float)(screenY - m_nearPlane.GetHeight()/2) / (m_nearPlane.GetHeight()/2);
	Vector4 ray(fscreenX, m_nearPlaneDistance, -fscreenY, 1.0f);

	return m_cs * ray;
}