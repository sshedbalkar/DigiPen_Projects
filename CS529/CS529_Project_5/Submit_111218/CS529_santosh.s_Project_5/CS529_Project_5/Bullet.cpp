#include "Bullet.h"
#include "ComponentTypeIds.h"
#include "Sprite.h"
#include "Body.h"
#include "Transform.h"
#include "Message.h"
#include "MsgIds.h"
#include "Composition.h"
#include "Physics.h"
#include "DebugTools.h"
//
static int BULLET_SPEED = 200;
namespace Wye
{
	Bullet::Bullet()
		:Component(CT_Bullet),
		_transform(NULL),
		_body(NULL)
	{
		//
	}
	Bullet::~Bullet()
	{
		//
	}
	void Bullet::initialize()
	{
		std::cout<<"Created bullet\n";
		_transform = ((*getParent()).getComponent<Transform>(CT_Transform));
		_display = ((*getParent()).getComponent<Sprite>(CT_Sprite));
		_body = ((*getParent()).getComponent<Body>(CT_Body));
	}
	void Bullet::direction(Vec2 dir)
	{
		normalize(dir);
		dir *= BULLET_SPEED;
		std::cout<<"Adding bullet speed: x: "<<dir.x<<", y: "<<dir.y<<"\n";
		_body->addForce(Vec3(dir.x, dir.y, 0.0f));
		if(dir.x < 0)
		{
			_display->getTransform()->rotation = Vec3(0.0f, 0.0f, D3DX_PI);
		}
	}
	void Bullet::serialize(const ISerializer& stream)
	{
		
	}
	void Bullet::onMessage(const Message& msg)
	{
		if(msg.MessageId == Msgid::Collide)
		{
			const MessageCollide* m = static_cast<const MessageCollide*>(&msg);
			//
			switch(*(m->CollidedWith->getName().c_str()))
			{
			case 'a':
				{
					std::cout<<"Collided with Hero\n";
				}
				break;
			case 'b':
				{
					std::cout<<"Collided with Camera\n";
				}
				break;
			case 'c':
				{
					std::cout<<"Collided with Wall\n";
				}
				break;
			case 'd':
				{
					std::cout<<"Collided with Block\n";
				}
				break;
			case 'e':
				{
					std::cout<<"Collided with AI\n";
				}
				break;
			case 'f':
				{
					std::cout<<"Collided with Bullet\n";
				}
				break;
			}
			std::cout<<"destroying bullet\n";
			getParent()->destroy();
		}
	}
}