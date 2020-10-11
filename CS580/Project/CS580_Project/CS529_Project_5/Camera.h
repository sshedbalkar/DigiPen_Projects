///////////////////////////////////////////////////////////////////////////////////////
///
///	\file Camera.h
///	Defines Camera Game Component
///	
///	Authors: Chris Peters
///	Copyright 2010, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////////////
#ifndef CAMERA_H
#define CAMERA_H
//#pragma message("Compiling: "__FILE__)
//
#include "Component.h"
#include "Transform.h"
//
namespace Wye
{	
	///Simple orthographic camera for viewing the game world.
	///Depends on Transform.
	class Camera: public Component
	{
//#pragma message("Including Class: Camera")
	public:
		Camera();
		~Camera();
		void initialize();
		//
		Vec3* getPosition();
		Vec3* getRotation();
		Transform* getTransform();
		Vec3* getLookAt();
		void setLookAt(Vec3& lookAt);
		void setLookAt(float x, float y, float z);
		void setLookAtDelta(float dx, float dy, float dz);
		Vec3* getUpVec();
		void setUpVec(Vec3& upVec);
		void setUpVec(float x, float y, float z);
		void setUpVecDelta(float dx, float dy, float dz);
		void serialize(const ISerializer& stream);
		//
	private:
		Transform* _transform;
		Vec3 _lookAt;
		Vec3 _upVec;
	};

}
//
#endif