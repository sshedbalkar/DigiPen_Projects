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
#include "Block.h"
#include "GameUtility.h"
#include "CollisionMap.h"
#include "AI_1.h"
#include "Key.h"
#include "Gold.h"
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
		
		Vec3 hg = GAMEDATA->gridHalfSize();
		COLLISION_MAP->create((int)hg.x*2, (int)hg.y*2);
		//
		createBorders();
		loadMap(level);
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
	void LevelLoader::loadMap(int id)
	{
		std::string type;
		Vec2 pos(0, 0);
		Vec3 pos3(0, 0, 1.0f);
		Composition* comp;
		Transform* transform;
		//
		//int gridX = 0, gridY = 0;
		int bSize = 0;
		//GAMEDATA->readSetting("gridX", gridX);
		//GAMEDATA->readSetting("gridY", gridY);
		GAMEDATA->readSetting("blockSize", bSize);
		Vec2 size((float)bSize, (float)bSize);
		std::string rowval;
		std::vector<std::string> row;
		//
		const std::vector<ISerializer*>* map = GAMEDATA->map(id);
		int i=0, j;
		for(std::vector<ISerializer*>::const_iterator it1 = map->begin(), it2 = map->end(); it1 != it2; ++it1, ++i)
		{
			row.clear();
			(*it1)->readProperty("values", rowval);
			splitString(rowval, ',', row);
			j=0;
			for(std::vector<std::string>::iterator it11 = row.begin(), it22 = row.end(); it11 != it22; ++it11, ++j)
			{
				const char* c = (*it11).c_str();
				if(*c == 'X')
				{
					continue;
				}
				else if(*c == '0')
				{
					continue;
				}
				else if(*c == '1')
				{
					Block* block;
					//
					comp = FACTORY->buildFromArchetype("a_9");
					block = comp->getComponent<Block>(CT_Block);
					block->type(c);
					block->coord(Vec2((float)j, (float)i));
					transform = comp->getComponent<Transform>(CT_Transform);
					//
					pos3 = coordToPixel((float)j, (float)i);
					transform->position = pos3;
					//
					comp->initialize();
					//
					//COLLISION_MAP->setBit(j, i, Wye::COLLISION_BLOCK_1);
				}
				else if(*c == '2' || *c=='3')
				{
					AI_1 *ai;
					comp = FACTORY->buildFromArchetype("a_10");
					ai = comp->getComponent<AI_1>(CT_AI_1);
					ai->type(c);
					ai->coord(Vec2(j, i));
					transform = comp->getComponent<Transform>(CT_Transform);
					//
					pos3 = coordToPixel(j, i);
					transform->position = pos3;
					//
					comp->initialize();
					//
					//COLLISION_MAP->setBit(j, i, Wye::COLLISION_AI_1);
				}
				else if(*c == '4')
				{
					Gold *gd;
					comp = FACTORY->buildFromArchetype("a_15");
					gd = comp->getComponent<Gold>(CT_Gold);
					//gd->type(c);
					gd->coord(Vec2(j, i));
					transform = comp->getComponent<Transform>(CT_Transform);
					//
					pos3 = coordToPixel(j, i);
					//pos3.z = 25;
					transform->position = pos3;
					//
					comp->initialize();
					//
					//COLLISION_MAP->setBit(j, i, COLLISION_GOLD);
				}
				else if(*c == '5')
				{
					Key *key;
					comp = FACTORY->buildFromArchetype("a_18");
					key = comp->getComponent<Key>(CT_Key);
					//ai->type(c);
					key->coord(Vec2(j, i));
					transform = comp->getComponent<Transform>(CT_Transform);
					//
					pos3 = coordToPixel(j, i);
					//pos3.z = 25;
					transform->position = pos3;
					//
					comp->initialize();
					//
					//COLLISION_MAP->setBit(j, i, COLLISION_GOLD);
				}
				else if(*c == '6')
				{
					Block* block;
					//
					comp = FACTORY->buildFromArchetype("a_19");
					block = comp->getComponent<Block>(CT_Block);
					block->type(c);
					block->coord(Vec2((float)j, (float)i));
					transform = comp->getComponent<Transform>(CT_Transform);
					//
					pos3 = coordToPixel((float)j, (float)i);
					transform->position = pos3;
					//
					comp->initialize();
					//
					//COLLISION_MAP->setBit(j, i, Wye::COLLISION_BLOCK_1);
				}
				else if(*c == '7')
				{
					Block* block;
					//
					comp = FACTORY->buildFromArchetype("a_20");
					block = comp->getComponent<Block>(CT_Block);
					block->type(c);
					block->coord(Vec2((float)j, (float)i));
					transform = comp->getComponent<Transform>(CT_Transform);
					//
					pos3 = coordToPixel((float)j, (float)i);
					transform->position = pos3;
					//
					comp->initialize();
					//
					//COLLISION_MAP->setBit(j, i, Wye::COLLISION_BLOCK_1);
				}
				else if(*c == '8')
				{
					Hero* hero;
					//
					comp = FACTORY->buildFromArchetype("a_0");
					hero = comp->getComponent<Hero>(CT_Hero);
					//block->type(c);
					//block->coord(Vec2((float)j, (float)i));
					//transform = comp->getComponent<Transform>(CT_Transform);
					//
					//pos3 = coordToPixel((float)j, (float)i);
					//body->Position = pos3;
					hero->coord(Vec2(j, i));
					//
					comp->initialize();
					//
					//COLLISION_MAP->setBit(j, i, Wye::COLLISION_BLOCK_1);
				}
			}
			/*
			(*it1)->readProperty("pos", pos);
			(*it1)->readProperty("type", type);
			comp = FACTORY->buildFromArchetype("a_9");
			//
			block = comp->getComponent<Block>(CT_Block);
			block->type(type.c_str());
			block->position(pos);
			
			//
			transform = comp->getComponent<Transform>(CT_Transform);
			//pos3.x = (pos.x - (gridX>>1) + 0.5f) * (bSize + 0);
			//pos3.y = (pos.y - (gridY>>1) + 0.5f) * (-bSize + 0);
			pos3 = coordToPixel(pos.x, pos.y);
			transform->position = pos3;
			//
			comp->initialize();
			//
			COLLISION_MAP->value(pos.x, pos.y, CT_Block);
			*/
		}
	}
	void LevelLoader::createBorders()
	{
		//int gridX = 0, gridY = 0;
		int bSize = 0;
		//GAMEDATA->readSetting("gridX", gridX);
		//GAMEDATA->readSetting("gridY", gridY);
		GAMEDATA->readSetting("blockSize", bSize);
		Vec3 hg = GAMEDATA->gridHalfSize();
		//gridX += 2;
		//gridY += 2;
		Vec2 sz((float)bSize, (float)bSize);
		//
		Vec3 posL = coordToPixel(0.0f, hg.y-0.5f);
		Vec3 posR = coordToPixel(2.0f * hg.x-1.0f, hg.y-0.5f);
		Vec3 posT = coordToPixel(hg.x-0.5f, 0.0f);
		Vec3 posB = coordToPixel(hg.x-0.5f, 2.0f*hg.y-1.0f);
		//
		Vec3 rot(0.0f, 0.0f, 90.0f * D3DX_PI/180);
		//
		Composition* compL = FACTORY->buildFromArchetype("a_8");
		Transform* transform = compL->getComponent<Transform>(CT_Transform);
		transform->position = posL;
		transform->rotation = rot;
		Body* body = compL->getComponent<Body>(CT_Body);
		Vec3 size1((float)bSize/2, (float)(hg.y * bSize), (float)bSize/2);
		body->size(size1);
		//
		Composition* compR = FACTORY->buildFromArchetype("a_8");
		transform = compR->getComponent<Transform>(CT_Transform);
		transform->position = posR;
		transform->rotation = rot;
		body = compR->getComponent<Body>(CT_Body);
		body->size(size1);
		//
		float x, y;
		for(int i = ((int)hg.y*2) - 1; i > -1 ; --i)
		{
			x = (i - (hg.y) + 0.5f) * (bSize + 0);
			y = 0;
			compL->addComponent(createBlockAt(x, y, sz, "t_24", "s_1"));
			//
			x *= -1;
			compR->addComponent(createBlockAt(x, y, sz, "t_24", "s_1"));
			//
			COLLISION_MAP->setBit(0, i, COLLISION_WALL);
			COLLISION_MAP->setBit((int)hg.x*2-1, i, COLLISION_WALL);
		}
		//
		Composition* compT = FACTORY->buildFromArchetype("a_8");
		transform = compT->getComponent<Transform>(CT_Transform);
		transform->position = posT;
		body = compT->getComponent<Body>(CT_Body);
		Vec3 size2((float)(hg.x * bSize), (float)bSize/2, bSize/2);
		body->size(size2);
		//
		Composition* compB = FACTORY->buildFromArchetype("a_8");
		transform = compB->getComponent<Transform>(CT_Transform);
		transform->position = posB;
		body = compB->getComponent<Body>(CT_Body);
		body->size(size2);
		//
		for(int i = ((int)hg.x * 2) - 1; i > -1; --i)
		{
			x = (i - (hg.x) + 0.5f) * (bSize + 0);
			y = 0;
			compT->addComponent(createBlockAt(x, y, sz, "t_24", "s_1"));
			//
			x *= -1;
			compB->addComponent(createBlockAt(x, y, sz, "t_24", "s_1"));
			COLLISION_MAP->setBit(i, 0, COLLISION_WALL);
			COLLISION_MAP->setBit(i, (int)hg.y*2-1, COLLISION_WALL);
		}
		//
		compL->initialize();
		compR->initialize();
		compT->initialize();
		compB->initialize();
	}
	Sprite* LevelLoader::createBlockAt(float x, float y, Vec2& size, const char* spriteName, const char* shaderId)
	{
		Sprite* spr = static_cast<Sprite*>(FACTORY->createComponent(CT_Sprite));
		Transform* trans;
		trans = spr->getTransform();
		trans->position = Vec3(x, y, 10.0f);
		spr->setSpriteName(spriteName);
		spr->setShader(shaderId);
		spr->size(size);
		return spr;
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
		std::cout<<"Finished-----------------------------------\n";
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