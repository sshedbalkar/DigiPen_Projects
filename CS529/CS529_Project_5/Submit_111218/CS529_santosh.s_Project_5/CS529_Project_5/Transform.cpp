///////////////////////////////////////////////////////////////////////////////////////
//
//	Transform.cpp - The Transform Component
//	Authors: Chris Peters
//	Copyright 2010, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////
//#include "Precompiled.h"
#include "Transform.h"
#include "ComponentTypeIds.h"
#include <iostream>
#include "Utility.h"
//
namespace Wye
{
	Transform::Transform():
		Component(CT_Transform),
		position(Vec3(0.0f, 0.0f, -1.0f)),
		rotation(Vec3(0.0f, 0.0f, 0.0f)),
		scale(Vec3(1.0f, 1.0f, 1.0f))
	{
		//
	}
	Transform::Transform(const Transform& orig):
		Component(CT_Transform),
		position(orig.position),
		rotation(orig.rotation),
		scale(orig.scale)
	{
		//
	}
	Transform& Transform::operator=(const Transform& rhs)
	{
		position = rhs.position;
		rotation = rhs.rotation;
		scale = rhs.scale;
		//
		return *this;
	}
	void Transform::initialize()
	{
		//std::cout<<"Transform, Pos: " << position << ", rot: "<<rotation<<", scale: "<<scale<<"\n";
	}
	void Transform::serialize(const ISerializer& stream)
	{
		//std::cout<<"Transform: Serializing\n";
		ISerializer* str = stream.clone();
		str->readProperty("Data", *str);
		if(str->isGood())
		{
			str->readProperty("position", position);
			if(str->readProperty("rotation", rotation))
			{
				rotation *= D3DX_PI/180;
			}
			str->readProperty("scale", scale);
			//std::cout<<"Transform serialized: Pos: " <<position << ", rot: "<<rotation<<", scale: "<<scale<<"\n";
		}
		delete str;
	}
}
