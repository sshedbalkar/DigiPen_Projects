/**
 *	\file	Camera.h
 *  \author David Yamanoha
 *  \date   08.08.08
 */
#pragma once
#include "../MatLib/CoordinateSystem.h"
#include "../MatLib/Plane.h"

/**
 *	\class	Camera
 *  \brief	Polar coordinate camera class
 *
 *			This class represents a camera in 3-dimensional space.
 */
class Camera
{
public:
						Camera();
						~Camera();
	void				SetTranslation(float x, float y, float z);
	void				SetTranslation(Vector4 v);
	void				SetPosition(float x, float y, float z);
	void				SetPosition(Vector4 p);
	void				SetOrientation(float x, float y, float z);
	void				SetOrientation(Vector4 orientation);
	void				SetNearPlaneDistance(float distance);
	float				GetNearPlaneDistance();
	void				SetFarPlaneDistance(float distance);
	float				GetFarPlaneDistance();
	Plane*				GetNearPlane();
	Plane*				GetFarPlane();
	
	void				TranslateLocal(float x, float y, float z);
	void				TranslateLocal(Vector4 v);
	void				SphericalRotate(int deltaX, int deltay);

	Vector4				GetPosition();
	Vector4				GetOrientation();
	CoordinateSystem	GetCoordinateSystem();

	Vector4				GetRayCast(int screenX, int screenZ, COORDINATE_SPACE cs = CS_LOCAL);
private:
	/**
	*	The coordinate system of the camera.  Represents position and orientation.
	*/
	CoordinateSystem	m_cs;
	/**
	*	The current polar vertical angle.
	*/
	float				m_vAngle;
	/**
	*	The current polar horizontal angle.
	*/
	float				m_hAngle;
	/**
	*	The near-plane distance from origin of the camera.
	*/
	float				m_nearPlaneDistance;
	/**
	*	The far-plane distance from origin of the camera.
	*/
	float				m_farPlaneDistance;
	/**
	*	The near plane of the camera.
	*/
	Plane				m_nearPlane;
	/**
	*	The far plane of the camera.
	*/
	Plane				m_farPlane;
};