///////////////////////////////////////////////////////////////////////////////////////
//
//	Transform.cpp - The Transform Component
//	Authors: Chris Peters
//	Copyright 2010, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////
#include "Precompiled.h"
#include "Transform.h"
#include "DebugTools.h"
#include "Utility.h"

namespace Framework
{
	Transform::Transform()
	{
		Position = Vec3(0,0,0);
		Scale = Vec3(1,1,1);
		Rotation = 0.0f;
	}

	void Transform::Serialize(ISerializer& stream)
	{
		//StreamRead(stream,Position);
		//StreamRead(stream,Rotation);
		//
		ISerializer* str = stream.clone();
		if(str->isGood())
		{
			str->readProperty("Position", *str);
			str->readValue("value", Position);
			//
			str->goToParent();
			if(str->readProperty("Rotation", *str))
			{
				if(str->readValue("value", Rotation))
				{
					Rotation *= (float)D3DX_PI/180.0f;
				}
			}
			str->goToParent();
			if(str->readProperty("Scale", *str))
			{
				str->readValue("value", Scale);
			}
			//std::cout<<"Transform serialized: Pos: " <<Position << ", rot: "<<Rotation<<", Scale: "<<Scale<<"\n";
		}
		delete str;
	}
	
}



