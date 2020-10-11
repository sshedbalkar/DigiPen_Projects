//#pragma message("Compiling: "__FILE__)
//
#include <algorithm>
#include "Composition.h"
#include "ComponentTypeIds.h"
#include <iostream>
#include "Factory.h"
//
namespace Wye
{
	Composition::Composition()
		:EventDispatcher(),
		_name("Composition")
	{
		setType(CT_Composition);
	}
	Composition::~Composition()
	{
		//Delete each component using the component's virtual destructor
		//takes care of all resources and memory.
		CompsIt it1 = _components.begin(), it2 = _components.end();
		//std::cout<<"Total comps: " <<_components.size()<<"\n";
		while(it1 != it2)
		{
			//std::cout<<"~Composition::Deleting: "<<*it1<<"\n";
			delete *it1;
			//std::cout<<"~Composition::Deletedddddd, size: " <<_components.size()<<"\n";
			++it1;
			//std::cout<<"incremented iterator\n";
		}
	}
	//
	struct ComponentSorter
	{
		bool operator()(Component* left, Component* right)const
		{
			return left->type() < right->type();
		}
	};
	//Binary search a sorted array of components.
	Component* BinaryCompoentSearch(ComponentArray& components, ComponentTypeId name)
	{
		size_t begin = 0;
		size_t end = components.size();
		//
		while(begin < end)
		{
			size_t mid = (begin+end) / 2;
			if(components[mid]->type() < name)
				begin = mid + 1;
			else
				end = mid;
		}
		//
		if((begin < components.size()) && (components[begin]->type() == name))
		{
			return components[begin];
		}
		else
		{
			return NULL;
		}
	}
	void Composition::addComponent(Component* component)
	{
		Composition* prt = component->getParent();
		if(prt != NULL)
		{
			prt->removeComponent(component);
		}
		_components.push_back(component);
		std::sort(_components.begin(), _components.end(), ComponentSorter());
		component->setParent(this);
	}
	void Composition::removeComponent(Component* component)
	{
		CompsIt it = std::find(_components.begin(), _components.end(), component);
		if(it!= _components.end())
		{
			(*it) -> setParent(NULL);
			_components.erase(it);
		}
	}
	void Composition::initialize()
	{
		Component::initialize();
		CompsIt it1 = _components.begin(), it2 = _components.end();
		//
		for(; it1 != it2; ++it1)
		{
			(*it1) -> setParent(this);
			(*it1) -> initialize();
		}
	}
	Component* Composition::getComponentByType(ComponentTypeId typeId)
	{
		return BinaryCompoentSearch(_components, typeId);
	}
	/*
	void Composition::dispatchEvent(Event* evt)
	{
		evt -> setCurrentTarget(this);
		Component::dispatchEvent(evt);
	}
	*/
	void Composition::onMessage(const Message& msg)
	{
		CompsIt it1 = _components.begin(), it2 = _components.end();
		//
		for(; it1 != it2; ++it1)
		{
			(*it1) -> onMessage(msg);
		}
	}
	std::string Composition::getName()
	{
		return _name;
	}
	void Composition::setName(const char* name)
	{
		_name = name;
	}
	/*
	void Composition::serialize(const ISerializer&* stream)
	{
		stream->readProperty("Components", *stream);
		if(stream->isGood())
		{
			std::vector<ISerializer*> comps;
			stream->readProperty("Component", comps);
			for(std::vector<ISerializer*>::iterator it1 = comps.begin(), it2 = comps.end(); it1 != it2; ++it1)
			{

			}
		}
	}
	*/
	void Composition::destroy()
	{ 
		//Signal the factory that is object needs to be destroyed
		//this will happen at the end of the frame.
		FACTORY->destroy(this); 
	}
}