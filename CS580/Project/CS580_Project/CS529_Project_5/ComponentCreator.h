///////////////////////////////////////////////////////////////////////////////////////
///
///	\file ComponentCreator.h Defines IComponentCreator interface.
///
///	Authors: Chris Peters
///	Copyright 2010, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////////////
#ifndef COMPONENT_CREATEOR_H
#define COMPONENT_CREATEOR_H
//#pragma message("Compiling: "__FILE__)
//
#include "Component.h"
//
namespace Wye
{
	//Forward declaration of component
	class Object;
	enum ComponentTypeId;
	//
	///Game Component Creator Interface.
	///Used to create components in data driven method in the GameObjectFactory.
	class ComponentCreator: public Object
	{
//#pragma message("Including Class: ComponentCreator")
	public:
		ComponentCreator(ComponentTypeId typeId):
			Object(),
			_typeId(typeId)
		{
			//
		}
		//
		ComponentTypeId _typeId;
		///Create the component
		virtual Component* create()=0;
	};
	//
	///Templated helper class used to make concrete component creators.
	template<typename type>
	class ComponentCreatorType: public ComponentCreator
	{
	public:
		ComponentCreatorType(ComponentTypeId typeId)
			:ComponentCreator(typeId)
		{
			//
		}
		//
		virtual Component* create()
		{
			return new type();
		}
	};
}
//
#endif