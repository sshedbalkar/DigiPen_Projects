///////////////////////////////////////////////////////////////////////////////////////
//
//	Camera.cpp
//	Authors: Chris Peters
//	Copyright 2010, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "Containers.h"
#include "Camera.h"
#include "Graphics.h"
#include "ComponentTypeIds.h"
#include "Utility.h"
//
namespace Wye
{
	Camera::Camera()
		:Component(CT_Camera),
		_transform(NULL),
		_lookAt(0.0f, 0.0f, 0.0f),
		_upVec(0.0f, 1.0f, 0.0f)
	{
		GRAPHICS->setMainCamera(this);
		std::cout<<"Set Main Camera\n";
	}

	Camera::~Camera()
	{
		if(GRAPHICS->getMainCamera() == this)
		{
			GRAPHICS->setMainCamera(NULL);
			std::cout<<"removed Main Camera\n";
		}
	}

	void Camera::initialize()
	{
		//_transform = static_cast<Transform*>(FACTORY->createComponent(CT_Transform));
		//_transform->copyFromParent();
		_transform = ((*getParent()).getComponent<Transform>(CT_Transform));
		std::cout<<"Camera: Look: " <<_lookAt<<", Up: "<<_upVec<<"\n";
	}
	Vec3* Camera::getPosition()
	{
		return &_transform->position;
	}
	Vec3* Camera::getRotation()
	{
		return &_transform->rotation;
	}
	Transform* Camera::getTransform()
	{
		return _transform;
	}
	Vec3* Camera::getLookAt()
	{
		return &_lookAt;
	}
	void Camera::setLookAt(Vec3& lookAt)
	{
		_lookAt = lookAt;
	}
	void Camera::setLookAt(float x, float y, float z)
	{
		_lookAt.x = x;
		_lookAt.y = y;
		_lookAt.z = z;
	}
	void Camera::setLookAtDelta(float dx, float dy, float dz)
	{
		_lookAt.x += dx;
		_lookAt.y += dy;
		_lookAt.z += dz;
	}
	Vec3* Camera::getUpVec()
	{
		return &_upVec;
	}
	void Camera::setUpVec(Vec3& upVec)
	{
		_upVec = upVec;
	}
	void Camera::setUpVec(float x, float y, float z)
	{
		_upVec.x = x;
		_upVec.y = y;
		_upVec.z = z;
	}
	void Camera::setUpVecDelta(float dx, float dy, float dz)
	{
		_upVec.x += dx;
		_upVec.y += dy;
		_upVec.z += dz;
	}
	void Camera::serialize(const ISerializer& stream)
	{
		ISerializer* str = stream.clone();
		str->readProperty("Data", *str);
		if(str->isGood())
		{
			str->readProperty("lookAt", _lookAt);
			str->readProperty("up", _upVec);
		}
		delete str;
	}
}