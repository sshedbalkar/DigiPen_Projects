#include "Bomb.h"
#include "ComponentTypeIds.h"
#include "Sprite.h"
#include "Body.h"
#include "Transform.h"
#include "Message.h"
#include "MsgIds.h"
#include "Composition.h"
#include "Physics.h"
#include "DebugTools.h"
#include "BombEffect.h"
#include "Factory.h"
#include "Core.h"
//
static int BOMB_SPEED = 200;
namespace Wye
{
	Bomb::Bomb()
		:Controller(),
		_transform(NULL),
		_body(NULL),
		_fuse(10.0f),
		_destroyed(false),
		_dir(1, 0),
		_exploded(false)
	{
		setType(CT_Bomb);
	}
	Bomb::~Bomb()
	{
		//
	}
	void Bomb::initialize()
	{
		Controller::initialize();
		//
		std::cout<<"Created Bomb: parent: "<<(*getParent()).getId()<<"\n";
		_transform = ((*getParent()).getComponent<Transform>(CT_Transform));
		_display = ((*getParent()).getComponent<Sprite>(CT_Sprite));
		_body = ((*getParent()).getComponent<Body>(CT_Body));

	}
	void Bomb::serialize(const ISerializer& stream)
	{
		
	}
	void Bomb::direction(Vec2 dir)
	{
		normalize(dir);
		dir *= BOMB_SPEED;
		std::cout<<"Adding Bomb speed: x: "<<dir.x<<", y: "<<dir.y<<"\n";
		_body->addForce(Vec3(dir.x, dir.y, 0.0f));
		if(dir.x < 0)
		{
			_display->getTransform()->rotation = Vec3(0.0f, 0.0f, D3DX_PI);
		}
	}
	void Bomb::update(float dt)
	{
		_fuse -= dt;
		//std::cout<<"Fuse: "<<_fuse<<"\n";
		if(!_destroyed && _fuse < 0.0)
		{
			_destroyed = true;
			_effect = dynamic_cast<BombEffect*>(FACTORY->createComponent(CT_BombEffect));
			(*getParent()).addComponent(_effect);
			_effect->radius(300);
			_effect->initialize();
		}
	}
	void Bomb::onMessage(const Message& msg)
	{
		//std::cout<<"bomb msg: " <<msg.MessageId<<"\n";
		if(msg.MessageId == Msgid::Collide)
		{
			const MessageCollide* m = static_cast<const MessageCollide*>(&msg);
			//
			switch(*(m->CollidedWith->getName().c_str()))
			{
			case 'e':
				{
					std::cout<<"Collided with AI\n";
				}
			case 'f':
				{
					std::cout<<"Collided with Bullet\n";
					std::cout<<"destroying Bomb\n";
					getParent()->destroy();
				}
				break;
			}
		}
		else if(msg.MessageId == Msgid::BombEffectDone)
		{
			if(!_exploded)
			{
				_exploded = true;
				std::cout<<"Bomb effect done, dispatching msg\n";
				BombExplodeMsg msg(_effect->radius(), _transform->position);
				CORE->onMessage(msg);
				getParent()->destroy();
			}
		}
	}
	/////////////////////////////////////////
	BombExplodeMsg::BombExplodeMsg(float rad, const Vec3& pos)
		:Message(Msgid::BombExplode),
		Radius(rad),
		Position(pos)
	{
		//
	}
	BombExplodeMsg::~BombExplodeMsg()
	{
		//
	}
}