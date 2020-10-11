///////////////////////////////////////////////////////////////////////////////////////
//
//	Factory.cpp
//	
//	Authors: Chris Peters
//	Copyright 2010, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////

#include "Precompiled.h"
#include "Factory.h"
//#include "Composition.h"
//#include "ComponentCreator.h"
#include "GameData.h"
#include "Utility.h"
//
namespace Wye
{
	CompositionFactory* FACTORY = NULL;
	//
	CompositionFactory::CompositionFactory()
		:_archetypes(new ArchetypeCollection())
	{		
		ErrorIf(FACTORY!=NULL, "Factory already created");
		FACTORY = this;

		//LastGameObjectId = 0;
	}

	CompositionFactory::~CompositionFactory()
	{
		//Delete all component creators
		ComponentMapType::iterator it1 = _componentMap.begin(), it2 = _componentMap.end();
		for(;it1 != it2; ++it1)
		{
			delete it1->second;
		}
		delete _archetypes;
	}

	Composition* CompositionFactory::createEmptyComposition()
	{
		//Create the composition and give it an id
		//this function is used to create compositions
		//programmatically
		Composition* gameObject = new Composition();
		idComposition(gameObject);
		return gameObject;
	}
	Composition* CompositionFactory::buildFromArchetype(const char* archId)
	{
		ISerializer* stream = GAMEDATA->archetypes()->getArchetypeById(archId)->getStream()->clone();
		stream->readProperty("Composition", *stream);
		Composition* obj = buildAndSerialize(*stream);
		delete stream;
		return obj;
	}
	Composition* CompositionFactory::buildAndSerialize(const ISerializer& stream)
	{
		Composition* ret = NULL;
		std::string nm;
		stream.readProperty("id", nm);
		ISerializer* strm = stream.clone();
		//
		strm->readProperty("Components", *strm);
		if(strm->isGood())
		{
			std::vector<ISerializer*> comps;
			if(strm->readProperty("Component", comps))
			{
				Composition* obj = createEmptyComposition();
				obj->_name = nm;
				int type;
				for(std::vector<ISerializer*>::iterator it1 = comps.begin(), it2 = comps.end(); it1 != it2; ++it1)
				{
					(*it1)->readProperty("type", type);
					ComponentMapType::iterator it =  _componentMap.find(static_cast<ComponentTypeId>(type));
					ErrorIf(it == _componentMap.end(), "Could not find component creator with id %d", type);
					if(it != _componentMap.end())
					{
						//ComponentCreator is an object that creates the component
						ComponentCreator* creator = it->second;

						//Create the component by using the interface
						Component* component = creator->create();

						//Serialize the component with current stream (this will move
						//the stream to the next component

						//Note: this invokes the base case of serialization in which
						//the object serializes itself. component->Serialize( stream );
						streamRead(**it1, *component);

						//Add the new component to the composition
						obj->addComponent(component);
					}
				}
				ret = obj;
			}
			deletePointersVec(comps);
		}
		delete strm;
		return ret;
	}
	Component* CompositionFactory::createComponent(ComponentTypeId typeId)
	{
		ComponentMapType::iterator it1 = _componentMap.find(typeId), it2 = _componentMap.end();
		ErrorIf(it1 == it2, "Could not find component creator with id %d" , typeId);
		if(it1 != it2)
		{
			//ComponentCreator is an object that creates the component
			ComponentCreator* creator = it1->second;

			//Create the component by using the interface
			return creator->create();
		}
		return NULL;
	}
	void CompositionFactory::idComposition(Composition* gameObject)
	{
		//Just increment the last id used. Does not handle 
		//overflow but it would take over 4 billion objects
		//to break
		//++LastGameObjectId;
		//gameObject->ObjectId = LastGameObjectId;

		//Store the game object in the global object id map
		_compositionIdMap[gameObject->getId()] = gameObject;
	}

	Composition* CompositionFactory::getObjectWithId(int id)
	{
		CompositionIdMapType::iterator it1 = _compositionIdMap.find(id), it2 = _compositionIdMap.end();
		if(it1 != it2)
		{
			return it1->second;
		}
		else
		{
			return NULL;
		}
	}

	Composition* CompositionFactory::create(const ISerializer& stream)
	{
		Composition* goc;
		std::string arch;
		stream.readProperty("archetype", arch);
		if(!arch.empty())
		{
			goc = buildFromArchetype(arch.c_str());
		}
		else
		{
			goc = buildAndSerialize(stream);
		}
		if(goc) goc->initialize();
		return goc;
	}

	void CompositionFactory::onMessage(const Message& msg)
	{
		CompositionIdMapType::iterator it1 = _compositionIdMap.begin(), it2 = _compositionIdMap.end();
		for(; it1 !=it2; ++it1)
		{
			it1->second->onMessage(msg);
		}
	}

	void CompositionFactory::destroy(Composition* gameObject)
	{
		//Add the object to the to be deleted list they will be delete
		//when the factory is updated
		_objectsToBeDeleted.insert(gameObject);
	}

	void CompositionFactory::update(float dt)
	{
		//Delete all objects in the ObjectsToBeDeleted list 
		std::set<Composition*>::iterator it1 = _objectsToBeDeleted.begin(), it2 = _objectsToBeDeleted.end();
		for(; it1 != it2; ++it1)
		{
			Composition* gameObject = *it1;
			CompositionIdMapType::iterator idItr1 = _compositionIdMap.find(gameObject->getId()), idItr2 = _compositionIdMap.end();
			ErrorIf(idItr1 == idItr2, "Object %d was double deleted or is bad memory.", gameObject->getId());
			if(idItr1 != idItr2)
			{
				//Delete it and remove its entry in the Id map
				delete gameObject;
				_compositionIdMap.erase(idItr1);
			}
		}
		//All objects to be delete have been deleted
		_objectsToBeDeleted.clear();
	}

	//Bind the creator for this component with the component name
	void CompositionFactory::addComponentCreator(ComponentTypeId typeId, ComponentCreator* creator)
	{
		_componentMap[typeId] = creator;
	}

	//Clean up the game world
	void CompositionFactory::destroyAllObjects()
	{
		CompositionIdMapType::iterator it1 = _compositionIdMap.begin(), it2 = _compositionIdMap.end();
		for(; it1 !=it2; ++it1)
		{
			delete it1->second;
		}
		//
		_compositionIdMap.clear();
	}

	ArchetypeCollection* CompositionFactory::archetypes()
	{
		return _archetypes;
	}
}