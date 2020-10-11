///////////////////////////////////////////////////////////////////////////////////////
///
///	\file Transform.h 
/// Defines the Transform Component.
///
///	Authors: Chris Peters
///	Copyright 2010, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////////////
#pragma once //Makes sure this header is only included once

#include "VMath.h"
#include "Composition.h"

namespace Framework
{
	///The transform component provides a shared position and rotation.
	class Transform : public GameComponent
	{
	public:	
		Transform();
		Vec3 Position;
		float Rotation;
		Vec3 Scale;
		void Serialize(ISerializer& str);
	};
}
