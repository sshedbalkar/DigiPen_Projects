//#pragma message("Compiling: "__FILE__)
//
#include "EventDispatcher.h"
#include "Composition.h"
#include "ComponentTypeIds.h"
#include <iostream>
//
namespace Wye
{
	/*
	EventDispatcher::EventDispatcher(Object* target):
		Object(getUniqueId()), 
	_target(target)
	{
		//
	}
	*/
	EventDispatcher::EventDispatcher():
		Component(CT_EventDispatcher)
	{
		//
	}
	EventDispatcher::EventDispatcher(const EventDispatcher& orig):
		Component(orig)
	{
		//
	}
	EventDispatcher::~EventDispatcher()
	{
		//
	}
	//
	EventDispatcher& EventDispatcher::operator=(const EventDispatcher& rhs)
	{
		return *this;
	}
	//
	void EventDispatcher::dispatchEvent(Event* evt)
	{
		Composition* parent = getParent();
		evt -> setCurrentTarget(parent ? parent : evt -> target());
		//
		Node *n;
		Msgid::MessageIdType id = evt -> MessageId;
		std::vector<Node*>::iterator it1 = _listeners.begin(), it2 = _listeners.end();
		for(; it1 != it2; ++it1)
		{
			if((n = (*it1))->msgid == id)
			{
				(*n->delegate)(evt);
			}
		}
		//
		
		if(parent && evt->bubbles())
		{
			parent -> dispatchEvent(evt);
		}
	}
	bool EventDispatcher::hasEventListener(Msgid::MessageIdType id)
	{
		std::vector<Node*>::iterator it1 = _listeners.begin(), it2 = _listeners.end();
		for(; it1 != it2; ++it1)
		{
			if((*it1)->msgid == id)
			{
				return true;
			}
		}
		return false;
	}
	
	EventDispatcher::Node::Node():
	msgid(Msgid::None),
	delegate(NULL),
	priority(-1),
	funcId(NULL)
	{
		//
	}
	//
	EventDispatcher::Node::~Node()
	{
		delete delegate;
		delegate = NULL;
	}
}