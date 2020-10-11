#include "Key.h"
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
	Key::Key()
		:Component(CT_Key),
		_angle(0.0f)
	{
		//
	}
	Key::~Key()
	{
		COLLISION_MAP->clearBit((uint)_currCoord.x, (uint)_currCoord.y, COLLISION_KEY);
		CORE->unregisterForUpdateMsg(this);
	}
	void Key::initialize()
	{
		std::cout<<"Created Key: parent: "<<(*getParent()).getId()<<"\n";
		_transform = ((*getParent()).getComponent<Transform>(CT_Transform));
		_delta = randomFloat(-1.0f, 1.0f) * D3DX_PI;
		//
		std::cout<<"Key x: "<<_currCoord.x<<", y: " <<_currCoord.y<<"\n";
		COLLISION_MAP->setBit((uint)_currCoord.x, (uint)_currCoord.y, COLLISION_KEY);
		CORE->registerForUpdateMsg(this);

	}
	void Key::serialize(const ISerializer& stream)
	{
		ISerializer* str = stream.clone();
		str->readProperty("Data", *str);
		//
		str->readProperty("value", _value);
		//
		delete str;
	}
	void Key::onMessage(const Message& msg)
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
					std::cout<<"Key Collided with Hero\n";
					getParent()->destroy();
				}
				break;
			}
		}
	}
}
