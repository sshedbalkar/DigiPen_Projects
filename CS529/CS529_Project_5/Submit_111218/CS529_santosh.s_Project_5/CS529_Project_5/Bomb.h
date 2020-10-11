////////////////////////////////////////////////////////////////
//
//	Bomb.h
//	implements the bomb logic
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef BOMB_H
#define BOMB_H
//
#include "Component.h"
#include "VMath.h"
#include "Controller.h"
//
namespace Wye
{
	class ISerializer;
	class Sprite;
	class Transform;
	class Body;
	class Event;
	class BombEffect;
	//
	class Bomb: public Controller
	{
#pragma message("Including class: Bomb")
	public:
		Bomb();
		~Bomb();
		void serialize(const ISerializer& stream);
		void initialize();
		void onMessage(const Message& msg);
		void update(float dt);
		inline void fuse(float d)
		{
			_fuse = d;
		}
		void direction(const Vec2 dir);
		//
	private:
		Sprite* _display;
		Transform* _transform;
		Body* _body;
		float _fuse;
		bool _destroyed;
		BombEffect* _effect;
		Vec2 _dir;
		bool _exploded;
	};
	//
	class BombExplodeMsg: public Message
	{
	public:
		BombExplodeMsg(float rad, const Vec3& pos);
		~BombExplodeMsg();
		float Radius;
		Vec3 Position;
	};
}
//
#endif