////////////////////////////////////////////////////////////////
//
//	Key.h
//	Impleemnts the key logic which unlocks locked blocks
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef KEY_H
#define KEY_H
//
#include "Component.h"
#include "VMath.h";
//
namespace Wye
{
	class Sprite;
	class Transform;
	//
	class Key: public Component
	{
	public:
		Key();
		~Key();
		void serialize(const ISerializer& stream);
		void initialize();
		void onMessage(const Message& msg);
		void update(float dt);
		inline void value(int val)
		{
			_value = val;
		}
		inline int value()
		{
			return _value;
		}
		inline Vec2 coord()const
		{
			return _currCoord;
		}
		inline void coord(const Vec2& pos)
		{
			_currCoord = pos;
		}
	private:
		//Sprite* _display;
		Transform* _transform;
		int _value;
		float _angle;
		Vec2 _currCoord;
		float _delta;
	};
}
//
#endif