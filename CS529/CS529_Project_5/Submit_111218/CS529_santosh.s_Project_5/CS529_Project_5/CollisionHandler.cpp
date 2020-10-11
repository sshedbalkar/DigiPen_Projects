#include "CollisionHandler.h"
#include "ComponentTypeIds.h"
#include "Composition.h"
#include "DebugTools.h"
#include "MsgIds.h"
#include "Physics.h"
#include "Hero.h"
#include "Block.h"
#include "AI_1.h"
//
namespace Wye
{
	CollisionHandler::CollisionHandler()
	{
		setType(CT_Collision_Handler);
	}
	CollisionHandler::~CollisionHandler()
	{
		//
	}
	void CollisionHandler::initialize()
	{
		switch((*getParent()).getName().c_str())
		{
		case 'a':	// Hero
			_pFn=&CollisionHandler::iAmHero;
			break;
		case 'b':	// Camera
			std::cout<<"CollisionHandler: Error, cannot collide with camera\n";
			break;
		case 'c':	// Wall
			_pFn=&CollisionHandler::iAmWall;
			break;
		case 'd':	// Block 1
			_pFn=&CollisionHandler::iAmBlock_1;
			break;
		case 'e':	// AI 1
			_pFn=&CollisionHandler::iAmAI_1;
			break;
		default:
			std::cout<<"CollisionHandler: Error, unhandled type\n";
		}
	}
	void CollisionHandler::onMessage(const Message& msg)
	{
		if(msg.MessageId == Msgid::Collide)
		{
			const MessageCollide* m = static_cast<const MessageCollide*>(&msg);
			//std::cout<<"AI Collided with: "<<m->CollidedWith->getName()<<"\n";
			((*this).*_pFn)(m);
		}
	}
	void CollisionHandler::iAmWall(const MessageCollide* msg)
	{
		CollidedWithWall m;
		m.Msg = msg;
		msg->CollidedWith->onMessage(m);
	}
	void CollisionHandler::iAmHero(const MessageCollide* msg)
	{
		CollidedWithHero m;
		m.Msg = msg;
		m.hero = ((*getParent()).getComponent<Hero>(CT_Hero));
		msg->CollidedWith->onMessage(m);
	}
	void CollisionHandler::iAmAI_1(const MessageCollide* msg)
	{
		CollidedWitAI_1 m;
		m.Msg = msg;
		m.ai1 = ((*getParent()).getComponent<AI_1>(CT_AI_1));
		msg->CollidedWith->onMessage(m);
	}
	void CollisionHandler::iAmBlock_1(const MessageCollide* msg)
	{
		CollidedWithBlock_1 m;
		m.Msg = msg;
		m.block = ((*getParent()).getComponent<Block>(CT_Block));
		msg->CollidedWith->onMessage(m);
	}
}