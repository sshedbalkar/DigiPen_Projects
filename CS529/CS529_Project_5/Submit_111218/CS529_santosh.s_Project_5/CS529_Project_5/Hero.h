////////////////////////////////////////////////////////////////
//
//	Hero.h
//	implements the player controlled character component
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef HERO_H
#define HERO_H
//
#include "Controller.h"
#include "VMath.h"
#include "MovieClip.h"
//
namespace Wye
{
	class Message;
	class ISerializer;
	class Sprite;
	class Body;
	class Event;
	class Transform;
	//
	class Hero: public Controller
	{
#pragma message("Including class: Hero")
	public:
		Hero();
		~Hero();
		void initialize();
		void onMessage(const Message& msg);
		void serialize(const ISerializer& stream);
		void moveDelta(float xd, float yd);
		void update(float dt);
		//
		inline Vec2 coord()const
		{
			return _coord;
		}
		inline void coord(const Vec2& pos)
		{
			_coord = pos;
		}
		inline const Sprite* display()const
		{
			return _display;
		}
		inline const Body* body()const
		{
			return _body;
		}
	private:
		//
		float _lastUpd;
		Vec2 _coord;
		Vec2 _prevCoord;
		MovieClip* _display;
		Body* _body;
		Transform *_tx;
		Transform *_dispTx;
		int _flags;
		//
		Vec2 _dir;
		float _dt;
		Vec3 _velocity;
		Vec3 _position;
		float _force;
		bool _firstKey;
		void checkDisplay();
		bool updateCoord();
		float _lastWepTime;
		void restart();
		float _lastRestart;
		bool _alive;
		bool isOnGround();
		bool _airJump;
	};
}
//
#endif