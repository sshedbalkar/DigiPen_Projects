////////////////////////////////////////////////////////////////
//
//	EventDispatcher.h
//	Definition of an event dispatcher class. Used to fire an event on an object. Uses external library "FastDelegate.h" for delegation.
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H
//#pragma message("Compiling: "__FILE__)
//
#include "FastDelegate.h"
#include "Component.h"
#include "Message.h"
//
namespace Wye
{
	typedef fastdelegate::FastDelegate1<Event*, void> EventDelegate;
	class Event;
	class Composition;
	//
	class EventDispatcher: public Component
	{
#pragma message("Including Class: EventDispatcher")
	public:
		//EventDispatcher(Object* target);
		EventDispatcher();
		EventDispatcher(const EventDispatcher& orig);
		~EventDispatcher();
		//
		template <typename Base, typename Obj, typename Param1, typename RetType>
		void addEventListener(Msgid::MessageIdType id, Obj* x, RetType (Base::*func)(Param1 p1), unsigned priority = -1)
		{
			Node *n = new Node();
			n -> msgid = id;
			n -> delegate = new EventDelegate(fastdelegate::MakeDelegate(x, func));
			n -> priority = priority;
			FuncId<Obj*, RetType (Base::*)(Param1)> u;
			n->funcId = u.getId(x, func);
			//std::cout << "Func id: " << n->funcId <<"\n";
			//
			if(priority == -1)
			{
				_listeners.push_back(n);
			}
			else
			{
				std::vector<Node*>::iterator it1 = _listeners.begin(), it2 = _listeners.end();
				for(; it1 != it2; ++it1)
				{
					if((*it1)->priority > priority)
					{
						_listeners.insert(it1, n);
						it1 = it2;
					}
				}
			}
		};
		virtual void dispatchEvent(Event* evt);
		virtual bool hasEventListener(Msgid::MessageIdType id);
		//
		template <typename Base, typename Obj, typename Param1, typename RetType>
		void removeEventListener(Msgid::MessageIdType id, Obj* x, RetType (Base::*func)(Param1 p1))
		{
			Node *n;
			unsigned funcId;
			std::vector<Node*>::iterator it1 = _listeners.begin(), it2 = _listeners.end();
			for(; it1 != it2; ++it1)
			{
				FuncId<Obj*, RetType (Base::*)(Param1)> u;
				funcId = u.getId(x, func);
				if((n = (*it1))->msgid == id && funcId == n->funcId)
				{
					_listeners.erase(it1);
					delete n;
					return;
				}
				
			}
		};
		//
	private:
		//Object* _target;
		struct Node
		{
			Msgid::MessageIdType msgid;
			EventDelegate *delegate;
			unsigned priority;
			unsigned funcId;
			//
			Node();
			~Node();
		};
		template<typename Obj, typename Func>
		union FuncId
		{
			Obj obj;
			Func func;
			unsigned id;
			//
			unsigned getId(Obj o, Func f)
			{
				obj = o;
				unsigned i = id;
				func = f;
				return i | id;
			};
		};
		std::vector<Node*> _listeners;
		EventDispatcher& operator=(const EventDispatcher& rhs);
	};
}
//#include "EventDispatcher.cpp"
//
#endif