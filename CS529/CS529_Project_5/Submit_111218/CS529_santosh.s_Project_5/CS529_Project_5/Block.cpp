#include "Block.h"
#include "ComponentTypeIds.h"
#include "GameData.h"
#include "Sprite.h"
#include "Body.h"
#include "Transform.h"
#include "Core.h"
#include "Message.h"
#include "MsgIds.h"
#include "CollisionMap.h"
#include "DebugTools.h"
#include "Message.h"
#include "Physics.h"
#include "GameLogic.h"
//
namespace Wye
{
	std::vector<Block*> Block::_blockList;
	//
	Block::Block()
		:Component(CT_Block),
		_transform(NULL),
		_body(NULL),
		_type(""),
		_position(0, 0),
		_blockType(BLOCK_REGULAR)
	{
		//
	}
	Block::~Block()
	{
		if(_blockType == BLOCK_LOCKED)
		{
			unregisterBlock(this);
		}
		COLLISION_MAP->clearBit((uint)_position.x, (uint)_position.y, COLLISION_BLOCK_1);
	}
	void Block::initialize()
	{
		_transform = ((*getParent()).getComponent<Transform>(CT_Transform));
		_display = ((*getParent()).getComponent<Sprite>(CT_Sprite));
		_body = ((*getParent()).getComponent<Body>(CT_Body));
		//
		_data = GAMEDATA->getBlockDataById(_type.c_str());
		std::string tex;
		_data->readProperty("tex", tex);
		_display->setSpriteName(tex);
		COLLISION_MAP->setBit((uint)_position.x, (uint)_position.y, COLLISION_BLOCK_1);
		if(*(_type.c_str()) == '6')
		{
			_blockType = BLOCK_LOCKED;
			registerBlock(this);
		}
		else if(*(_type.c_str()) == '7')
		{
			_blockType = BLOCK_CROWN;
		}
		//
		//switch(*_type.c_str())
		//{
		//case '1':
			//{
				//CORE->addEventListener(Msgid::EnterFrame, this, &Block::onEnterFrame);
				//Body* body = static_cast<Body*>(FACTORY->createComponent(CT_Body));
			//}
		//}
	}
	Vec2 Block::coord()
	{
		return _position;
	}
	void Block::coord(const Vec2& pos)
	{
		_position = pos;
	}
	std::string Block::type()
	{
		return _type;
	}
	void Block::type(const char* typ)
	{
		_type = typ;
	}
	void Block::serialize(const ISerializer& stream)
	{
		Vec2 pos;
		stream.readProperty("pos", pos);
		stream.readProperty("type", _type);
	}
	void Block::remove()
	{
		getParent()->destroy();
	}
	void Block::onMessage(const Message& msg)
	{
		if(msg.MessageId == Msgid::Collide)
		{
			const MessageCollide* m = static_cast<const MessageCollide*>(&msg);
			//
			switch(*(m->CollidedWith->getName().c_str()))
			{
			case 'a':
				{
					if(_blockType == BLOCK_CROWN)
					{
						//std::cout<<"CROWN Collided with Hero\n";
						//getParent()->destroy();
						int i;
						GAMEDATA->readSetting("levelBonus", i);
						LOGIC->playerData.Coins += i;
						LOGIC->loadLevel(LOGIC->playerData.Level + 1);
					}
				}
				break;
			}
		}
	}
}