///////////////////////////////////////////////////////////////////////////////
///
///	\file Factory.h
///	Defines the CompositionFactory system.
///	
///	Authors: Chris Peters
///	Copyright 2010, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#ifndef FACTORY_H
#define FACTORY_H
//#pragma message("Compiling: "__FILE__)
//
#include "Containers.h"
#include "ComponentCreator.h"
#include "Archetype.h"
#include "System.h"

namespace Wye
{
	///Forward Declaration of Component Creator
	class ISerializer;
	class Composition;
	enum ComponentTypeId;
	///Game Object Factory The game object factory creates composition objects 
	///from data streams and manages their lifetimes. As part of controlling the 
	///lifetimes of the Composition it also provides an integer based Id system for safe 
	///referencing of game objects through integer Id Handles.
	class CompositionFactory: public System
	{
#pragma message("Including Class: CompositionFactory")
	public:
		CompositionFactory();
		~CompositionFactory();

		///Create initialize and Id a Composition from the data file.
		Composition* create(const ISerializer& stream);

		///Add a Composition to the destroy list for delayed destruction.
		void destroy(Composition* gameObject);

		///Update the factory, destroying dead objects.
		virtual void update(float dt);

		///Name of the system is factory.
		virtual std::string getName(){return "Factory";}

		///Message Interface see Message.h
		virtual void onMessage(const Message& msg);

		///Destroy all the Compositions in the world. Used for final shutdown.
		void destroyAllObjects();

		///Create and Id a Composition at runtime. Used to dynamically build Composition.
		///After components have been added call Composition->Initialize().
		Composition* createEmptyComposition();

		///Build a composition and serialize from the data file but do not initialize the Composition.
		///Used to create a composition and then adjust its data before initialization
		///see GameObjectComposition::Initialize for details.
		Composition* buildAndSerialize(const ISerializer& stream);
		Composition* buildFromArchetype(const char* archId);

		///Id object and store it in the object map.
		void idComposition(Composition* gameObject);

		///Add a component creator enabling data driven composition
		void addComponentCreator(ComponentTypeId typeId, ComponentCreator* creator);

		///Get the game object with given id. This function will return NULL if
		///the object has been destroyed.
		Composition* getObjectWithId(int id);
		//
		Component* createComponent(ComponentTypeId typeId);
		ArchetypeCollection* archetypes();
	private:
		///Used to incrementally generate unique id's.
		//unsigned LastGameObjectId;

		///Map of component creator used for data driven composition
		typedef std::map<ComponentTypeId, ComponentCreator*> ComponentMapType;
		ComponentMapType _componentMap;

		///Map of Composition to their Ids used for safe referencing of game objects
		typedef std::map<int, Composition*> CompositionIdMapType;
		CompositionIdMapType _compositionIdMap;

		///Objects to be deleted
		///this is a set to prevent problems when a game object
		///is deleted in multiple locations
		std::set<Composition*> _objectsToBeDeleted;
		ArchetypeCollection* _archetypes;
	};
	//
	extern CompositionFactory* FACTORY;
}
//
//Register component macro
//#define RegisterComponent(id, type) FACTORY->addComponentCreator(#id, new ComponentCreatorType<type>(#id));
//
#endif