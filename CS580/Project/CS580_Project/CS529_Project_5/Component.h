////////////////////////////////////////////////////////////////
//
//	Component.h
//	Base class for all the components
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef COMPONENT_H
#define COMPONENT_H
//#pragma message("Compiling: "__FILE__)
//
#include "Object.h"
#include "Serialization.h"
#include "Message.h"
//
namespace Wye
{
	class Composition;
	enum ComponentTypeId;
	//
	class Component: public Object
	{
//#pragma message("Including Class: Component")
	public:
		Component(ComponentTypeId type);
		Component();
		virtual ~Component();
		//
		virtual void initialize();
		virtual void onMessage(const Message& msg);
		virtual Composition* getParent();
		virtual void setParent(Composition* parent);
		virtual ComponentTypeId type() const;
		virtual void serialize(const ISerializer& stream);
	protected:
		Component(const Component& orig);
		Component& operator=(const Component& rhs);
		//
		virtual void setType(ComponentTypeId type);
	private:
		Composition* _parent;
		ComponentTypeId _type;
	};
}
#endif