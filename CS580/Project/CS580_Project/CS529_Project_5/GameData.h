////////////////////////////////////////////////////////////////
//
//	GameData.h
//	Parses an external game settings xml file and holds the data for later usage by other parts of the game.
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef GAMEDATA_H
#define GAMEDATA_H
//#pragma message("Compiling: "__FILE__)
//
#include <vector>
#include "System.h"
#include "XMLSerialization.h"
#include "Archetype.h"
#include "VMath.h"
//
namespace Wye
{
	class GameData: public System
	{
//#pragma message("Including Class: GameData")
	public:
		GameData();
		~GameData();
		void initialize();
		void loadLevelData(const char* file);
		inline ArchetypeCollection* archetypes()
		{
			return _archetypes;
		}
		inline const ISerializer& stream()
		{
			return *_stream;
		}
		inline const std::vector<ISerializer*>* textures()
		{
			return &_textures;
		}
		inline const std::vector<ISerializer*>* shaders()
		{
			return &_shaders;
		}
		//
		template <typename T>
		inline bool readSetting(const char* prop, T& data)
		{
			return _gameSettings->readProperty(prop, data);
		}
		//
	private:
		void parseTextures(ISerializer* stream);
		void parseShaders(ISerializer* stream);
	private:
		XMLReader* _reader;
		ISerializer* _stream;
		ArchetypeCollection* _archetypes;
		std::vector<ISerializer*> _textures;
		std::vector<ISerializer*> _shaders;
		ISerializer* _gameSettings;
	};
	//
	extern GameData* GAMEDATA;
}
//
#endif