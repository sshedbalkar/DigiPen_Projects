//#pragma message("Compiling: "__FILE__)
//
#include "Component.h"
#include "Utility.h"
#include "MsgIds.h"
#include "ComponentTypeIds.h"
//
namespace Wye
{
	Component::Component(ComponentTypeId type):
		Object(getUniqueId()), 
		_parent(0), 
		_type(type)
	{
		//
	}
	Component::Component():
		Object(getUniqueId()), 
		_parent(0), 
		_type(CT_Component)
	{
		//
	}
	Component::Component(const Component& orig):
		Object(orig), 
		_parent(orig._parent), 
		_type(orig._type)
	{
		//
	}
	Component::~Component()
	{
		//
	}
	//
	Component& Component::operator=(const Component& rhs)
	{
		_parent = rhs._parent;
		_type = rhs._type;
		Object::operator=(rhs);
		//
		return *this;
	}
	//
	void Component::initialize()
	{
		//
	}
	void Component::onMessage(const Message& msg)
	{
		//
	}
	Composition* Component::getParent()
	{
		return _parent;
	}
	void Component::setParent(Composition* parent)
	{
		_parent = parent;
	}
	ComponentTypeId Component::type() const
	{
		return _type;
	}
	/*
	void Component::dispatchEvent(Event* evt)
	{
		EventDispatcher::dispatchEvent(evt);
		//
		if(_parent)
		{
			_parent -> dispatchEvent(evt);
		}
	}
	*/
	void Component::setType(ComponentTypeId type)
	{
		_type = type;
	}
	void Component::serialize(const ISerializer& stream)
	{
		//
	}
}