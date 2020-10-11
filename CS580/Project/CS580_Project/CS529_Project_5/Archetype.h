////////////////////////////////////////////////////////////////
//
//	Archetype.h
//	Abstraction layer for archetypes
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef WYE_ARCHETYPE_H
#define WYE_ARCHETYPE_H
//#pragma message("Compiling: "__FILE__)
//
#include <map>
#include "Serialization.h"
//
namespace Wye
{
	class Archetype
	{
//#pragma message("Including Class: Archetype")
	public:
		Archetype();
		Archetype(ISerializer& stream);
		~Archetype();
		void createFrom(ISerializer& stream);
		ISerializer* getStream();
		ISerializer* getStream() const;
	private:
		ISerializer* _stream;
	};
	/////////////////////////
	class ArchetypeCollection
	{
//#pragma message("Including Class: ArchetypeCollection")
	public:
		ArchetypeCollection();
		~ArchetypeCollection();
		void createCollectionFrom(ISerializer& stream);
		const Archetype* getArchetypeById(const std::string id) const;
		void testFn();
	private:
		void empty();
		std::map<std::string, const Archetype*> _archetypeMap;
	};
}
//
#endif