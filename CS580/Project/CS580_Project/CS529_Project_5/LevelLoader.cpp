#include "LevelLoader.h"
#include "GameData.h"
#include "Graphics.h"
#include "Factory.h"
#include "Utility.h"
#include "DebugTools.h"
#include "GameLogic.h"
#include "Core.h"
#include "ComponentTypeIds.h"
#include "Body.h"
//
namespace Wye
{
	LevelLoader::LevelLoader()
	{
		//
	}
	LevelLoader::~LevelLoader()
	{
		//
	}
	void LevelLoader::loadLevel(int level)
	{
		ISerializer* stream = GAMEDATA->stream().clone();
		//
		loadCompositions(*stream);
		//std::cout<<COLLISION_MAP;
		//
		delete stream;
	}
	void LevelLoader::loadAssets()
	{
		loadTextures();
		loadShaders();
	}
	void LevelLoader::loadCompositions(ISerializer& stream)
	{
		std::cout<<"Creating Compositions-----------------------:\n";
		ISerializer* str = stream.clone();
		str->readProperty("Compositions", *str);
		//
		std::vector<ISerializer*> comps;
		str->readProperty("Composition", comps);
		Composition* obj;
		//
		for(std::vector<ISerializer*>::iterator it1 = comps.begin(), it2 = comps.end(); it1 != it2; ++it1)
		{
			ISerializer* stream = *it1;
			std::string id, arch;
			stream->readProperty("id", id);
			std::cout<<"---------"<<id<<"--------\n";
			stream->readProperty("archetype", arch);
			if(!arch.empty())
			{
				Vec3 pos, rot, sc;
				if(stream->readProperty("position", pos))
				{
					if(stream->readProperty("rotation", rot))
					{
						if(stream->readProperty("scale", sc))
						{
							LOGIC->createObjectAt(arch.c_str(), pos, rot, sc);
						}
						else
						{
							LOGIC->createObjectAt(arch.c_str(), pos, rot);
						}
					}
					else
					{
						LOGIC->createObjectAt(arch.c_str(), pos);
					}
				}
				else
				{
					obj = FACTORY->create(*stream);
				}
			}
			else
			{
				obj = FACTORY->create(*stream);
			}
		}
		std::cout<<"--------------------Finished------------------------\n";
		//
		deletePointersVec(comps);
		delete str;
	}
	void LevelLoader::loadTextures()
	{
		const std::vector<ISerializer*>* vec = GAMEDATA->textures();
		std::string id;
		std::string file;
		//
		for(std::vector<ISerializer*>::const_iterator it1 = vec->begin(), it2 = vec->end(); it1 != it2; ++it1)
		{
			(*it1)->readProperty("id", id);
			(*it1)->readProperty("url", file);
			GRAPHICS->loadTexture(id.c_str(), file);
		}
	}
	void LevelLoader::loadShaders()
	{
		const std::vector<ISerializer*>* vec = GAMEDATA->shaders();
		std::string id;
		std::string file;
		int debug;
		//
		for(std::vector<ISerializer*>::const_iterator it1 = vec->begin(), it2 = vec->end(); it1 != it2; ++it1)
		{
			debug = 0;
			(*it1)->readProperty("id", id);
			(*it1)->readProperty("url", file);
			GRAPHICS->loadEffect(id.c_str(), file);
			(*it1)->readProperty("debug", debug);
			if(debug)
			{
				GRAPHICS->debugShader(id);
			}
		}
	}
}