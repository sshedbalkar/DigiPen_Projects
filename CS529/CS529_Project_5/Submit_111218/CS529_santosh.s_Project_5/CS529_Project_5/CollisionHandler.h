#ifndef COLLISION_HANDLER_H
#define COLLISION_HANDLER_H
//
#include "Component.h"
#include "Message.h"
#include "MsgIds.h"
//
namespace Wye
{
	class Message;
	class MessageCollide;
	class Hero;
	class AI_1;
	class Block;
	//
	class CollisionHandler: public Component
	{
	public:
		CollisionHandler();
		~CollisionHandler();
		void onMessage(const Message& msg);
		void initialize();
	private:
		typedef void (CollisionHandler::*MemFn)(const MessageCollide* msg);
		//
		void iAmWall(const MessageCollide* msg);
		void iAmHero(const MessageCollide* msg);
		void iAmAI_1(const MessageCollide* msg);
		void iAmBlock_1(const MessageCollide* msg);
		//
		MemFn _pFn;
	};
	//
	class CollidedWithWall: public Message
	{
	public:
		const MessageCollide* Msg;
		CollidedWithWall(): Message(Msgid::CollideWall){};
	};
	//
	class CollidedWithHero: public Message
	{
	public:
		const MessageCollide* Msg;
		Hero* hero;
		CollidedWithHero(): Message(Msgid::CollideHero){};
	};
	//
	class CollidedWitAI_1: public Message
	{
	public:
		const MessageCollide* Msg;
		AI_1* ai1;
		CollidedWitAI_1(): Message(Msgid::CollideAI_1){};
	};
	//
	class CollidedWithBlock_1: public Message
	{
	public:
		const MessageCollide* Msg;
		Block* block;
		CollidedWithBlock_1(): Message(Msgid::CollideBlock_1){};
	};
	//
}
//
#endif