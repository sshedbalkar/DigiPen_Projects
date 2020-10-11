////////////////////////////////////////////////////////////////
//
//	Composition.h
//	Defines a composition class which holds various components together
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef COMPOSITION_H
#define COMPOSITION_H
//#pragma message("Compiling: "__FILE__)
//
#include <vector>
#include <string>
#include "EventDispatcher.h"
//
namespace Wye
{
	class CompositionFactory;
	enum ComponentTypeId;
	//
	//A vector of components pointers is used to store components.
	typedef std::vector<Component*> ComponentArray;
	//
	class Composition: public EventDispatcher
	{
//#pragma message("Including Class: Composition")
	public:
		friend class CompositionFactory;
		virtual ~Composition();
		//
		Component* getComponentByType(ComponentTypeId typeId);
		//
		template<typename type>
		type* getComponent(ComponentTypeId typeId)
		{
			return static_cast<type*>(getComponentByType(typeId));
		}
		//
		void addComponent(Component* component);
		void removeComponent(Component* component);
		//
		virtual void initialize();
		virtual void onMessage(const Message& msg);
		virtual std::string getName();
		virtual void setName(const char* name);
		void destroy();
		//void serialize(const ISerializer& stream)
		//virtual void dispatchEvent(Event* evt);
		//
	protected:
		Composition();
		Composition(const Composition& orig);
		Composition& operator=(const Composition& rhs);
		//
		ComponentArray _components;
		typedef std::vector<Component*>::iterator CompsIt;
		std::string _name;
	};
}
//
#endif