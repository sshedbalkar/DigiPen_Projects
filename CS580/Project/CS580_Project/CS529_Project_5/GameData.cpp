#include "GameData.h"
#include "Archetype.h"
#include "Precompiled.h"
#include "Utility.h"
#include <iostream>
//
namespace Wye
{
	GameData* GAMEDATA = NULL;
	//
	GameData::GameData()
		:_reader(NULL),
		_archetypes(NULL),
		_stream(NULL),
		_textures(),
		_shaders()
	{
		ErrorIf(GAMEDATA!=NULL,"Gamedata already initialized\n");
		GAMEDATA = this;
	}
	GameData::~GameData()
	{


		safeDelete(_archetypes);
		safeDelete(_gameSettings);
		safeDelete(_stream);
		safeDelete(_reader);
		//
		deletePointersVec(_textures);
		deletePointersVec(_shaders);
		//
	}
	void GameData::initialize()
	{
		_reader = new XMLReader();
		_archetypes = new ArchetypeCollection();
		//_textures = new std::vector<ISerializer*>();
		//_shaders = new std::vector<ISerializer*>();
		//_blocks = new std::vector<ISerializer*>();
		std::cout<<"GameData inited\n";
	}
	void GameData::loadLevelData(const char* file)
	{
		if(_reader->open(file))
		{
			ISerializer* str = 0; 
			_stream = _reader->data().clone();
			str = _stream->clone();
			//
			_gameSettings = _stream->clone();
			_gameSettings->readProperty("Settings", *_gameSettings);
			//
			//
			_archetypes->createCollectionFrom(*_stream);
			//_archetypes->testFn();
			parseTextures(_stream);
			parseShaders(_stream);
			//
			delete str;
		}
	}
	void GameData::parseTextures(ISerializer* stream)
	{
		ISerializer* str = stream->clone();
		//
		str->readProperty("Textures", *str);
		str->readProperty("Texture", _textures);
		//
		delete str;
	}
	void GameData::parseShaders(ISerializer* stream)
	{
		ISerializer* str = stream->clone();
		//
		str->readProperty("Shaders", *str);
		str->readProperty("Shader", _shaders);
		//
		delete str;
	}
}