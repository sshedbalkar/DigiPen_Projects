#include "Gold.h"
#include "ComponentTypeIds.h"
#include "Core.h"
#include "DebugTools.h"
#include "Serialization.h"
#include "Message.h"
#include "Transform.h"
#include "CollisionMap.h"
#include "Physics.h"
#include "Utility.h"
//
namespace Wye
{
	Gold::Gold()
		:Component(CT_Gold),
		_angle(0.0f)
	{
		//
	}
	Gold::~Gold()
	{
		COLLISION_MAP->clearBit((uint)_currCoord.x, (uint)_currCoord.y, COLLISION_GOLD);
		CORE->unregisterForUpdateMsg(this);
	}
	void Gold::initialize()
	{
		//std::cout<<"Created Gold: parent: "<<(*getParent()).getId()<<"\n";
		_transform = ((*getParent()).getComponent<Transform>(CT_Transform));
		_delta = randomFloat(-1.0f, 1.0f) * D3DX_PI;
		//
		COLLISION_MAP->setBit((uint)_currCoord.x, (uint)_currCoord.y, COLLISION_GOLD);
		CORE->registerForUpdateMsg(this);

	}
	void Gold::serialize(const ISerializer& stream)
	{
		ISerializer* str = stream.clone();
		str->readProperty("Data", *str);
		//
		str->readProperty("value", _value);
		//
		delete str;
	}
	void Gold::onMessage(const Message& msg)
	{
		if(msg.MessageId == Msgid::EnterFrame)
		{
			const EnterFrameMsg* m = static_cast<const EnterFrameMsg*>(&msg);
			_angle += _delta * m->dt;
			if(_angle > 2 * D3DX_PI)
			{
				_angle = 0;
			}
			else if(_angle < 0.0f)
			{
				_angle = 2 * D3DX_PI;
			}
			_transform->rotation = Vec3(0.0f, _angle, 0.0f);
		}
		else if(msg.MessageId == Msgid::Collide)
		{
			const MessageCollide* m = static_cast<const MessageCollide*>(&msg);
			//
			switch(*(m->CollidedWith->getName().c_str()))
			{
			case 'a':
				{
					std::cout<<"Gold Collided with Hero\n";
					getParent()->destroy();
				}
				break;
			}
		}
	}
}
