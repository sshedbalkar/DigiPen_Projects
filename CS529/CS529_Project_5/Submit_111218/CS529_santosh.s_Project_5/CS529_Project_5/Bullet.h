////////////////////////////////////////////////////////////////
//
//	Bullet.h
//	implements the bullet logic
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef BULLET_H
#define BULLET_H
//
#include "Component.h"
#include "VMath.h"
//
namespace Wye
{
	class ISerializer;
	class Sprite;
	class Transform;
	class Body;
	class Event;
	//
	class Bullet: public Component
	{
#pragma message("Including class: Bullet")
	public:
		Bullet();
		~Bullet();
		void serialize(const ISerializer& stream);
		void initialize();
		void onMessage(const Message& msg);
		void direction(Vec2 dir);
		//
	private:
		Sprite* _display;
		Transform* _transform;
		Body* _body;
	};
}
//
#endif