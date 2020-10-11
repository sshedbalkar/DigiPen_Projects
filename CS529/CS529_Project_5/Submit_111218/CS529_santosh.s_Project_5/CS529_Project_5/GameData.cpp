#include "GameData.h"
#include "Archetype.h"
#include "Precompiled.h"
#include "Utility.h"
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
		_shaders(),
		_blocks(),
		_map(),
		_gridHalfSize(),
		_blockSize(0)
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
		deletePointersVec(_blocks);
		//
		for(std::vector<std::vector<ISerializer*>* >::iterator it1 = _map.begin(), it2 = _map.end(); it1 != it2; ++it1)
		{
			deletePointersVec(*(*it1));
		}
		deletePointersVec(_map);
		//
	}
	void GameData::initialize()
	{
		_reader = new XMLReader();
		_archetypes = new ArchetypeCollection();
		//_textures = new std::vector<ISerializer*>();
		//_shaders = new std::vector<ISerializer*>();
		//_blocks = new std::vector<ISerializer*>();
	}
	void GameData::loadLevelData(const char* file)
	{
		if(_reader->open(file))
		{
			ISerializer* str; 
			_stream = _reader->data().clone();
			str = _stream->clone();
			//
			_gameSettings = _stream->clone();
			_gameSettings->readProperty("Settings", *_gameSettings);
			float gx = 0.0f, gy = 0.0f;
			int bSize = 0;
			readSetting("gridX", gx);
			readSetting("gridY", gy);
			readSetting("blockSize", bSize);
			_gridHalfSize.x = gx/2.0f;
			_gridHalfSize.y = gy/2.0f;
			_blockSize = bSize;
			//
			_stream->readProperty("Blocks", *str);
			str->readProperty("Block", _blocks);
			//
			std::vector<ISerializer*> map1, *map2;
			_stream->readProperty("Map", map1);
			for(std::vector<ISerializer*>::iterator it1 = map1.begin(), it2 = map1.end(); it1 != it2; ++it1)
			{
				map2 = new std::vector<ISerializer*>();
				(*it1)->readProperty("Row", *map2);
				_map.push_back(map2);
			}
			deletePointersVec(map1);
			//str->readProperty("Row", _map);
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
	const ISerializer* GameData::getBlockDataById(const std::string id)const
	{
		const char* prop = id.c_str();
		std::string str;
		for(std::vector<ISerializer*>::const_iterator it1 = _blocks.begin(), it2 = _blocks.end(); it1 != it2; ++it1)
		{
			if((**it1).readProperty("id", str))
			{
				if(!strcmp(prop, str.c_str()))
				{
					return *it1;
				}
			}
		}
		return NULL;
	}

}