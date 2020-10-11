///////////////////////////////////////////////////////////////////////////////////////
///
///	\file Transform.h 
/// Defines the Transform Component.
///
///	Authors: Chris Peters
///	Copyright 2010, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////////////
#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H
//#pragma message("Compiling: "__FILE__) //Makes sure this header is only included once
//
#include "VMath.h"
#include "Component.h"

namespace Wye
{
	///The transform component provides a shared position and rotation.
	class Transform: public Component
	{
#pragma message("Including Class: Transform")
	public:
		Transform();
		Transform(const Transform& orig);
		Transform& operator=(const Transform& rhs);
		//
		void initialize();
		//
		Vec3 position;
		Vec3 rotation;
		Vec3 scale;
		void serialize(const ISerializer& str);
	};
}//
#endif