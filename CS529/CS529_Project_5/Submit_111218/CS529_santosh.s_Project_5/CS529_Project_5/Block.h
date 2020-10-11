////////////////////////////////////////////////////////////////
//
//	Block.h
//	implements various immovable blocks in the game
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef BLOCK_H
#define BLOCK_H
//
#include "Component.h"
#include "VMath.h"
#include <algorithm>
#include "Utility.h"
//
namespace Wye
{
	class ISerializer;
	class Sprite;
	class Transform;
	class Body;
	class Event;
	class Message;
	//
	class Block: public Component
	{
#pragma message("Including class: Block")
	public:
		Block();
		~Block();
		void serialize(const ISerializer& stream);
		void initialize();
		void onMessage(const Message& msg);
		Vec2 coord();
		void coord(const Vec2& pos);
		std::string type();
		void type(const char* typ);
		static void removeABlock()
		{
			int i = floor(randomFloat(0.0f, _blockList.size()-0.2f));
			std::vector<Block*>::iterator it = _blockList.begin()+i;
			(*it)->remove();
		}
		//
	private:
		enum BLOCK_TYPES
		{
			BLOCK_REGULAR,
			BLOCK_LOCKED,
			BLOCK_CROWN
		};
		static std::vector<Block*> _blockList;
		static void registerBlock(Block* blk)
		{
			std::vector<Block*>::iterator it = std::find(_blockList.begin(), _blockList.end(), blk);
			if(it == _blockList.end())
			{
				_blockList.push_back(blk);
			}
		}
		static void unregisterBlock(Block* blk)
		{
			std::vector<Block*>::iterator it = std::find(_blockList.begin(), _blockList.end(), blk);
			if(it != _blockList.end())
			{
				_blockList.erase(it);
			}
		}
		Sprite* _display;
		Transform* _transform;
		Body* _body;
		std::string _type;
		Vec2 _position;
		const ISerializer* _data;
		int _blockType;
		void remove();
	};
}
//
#endif