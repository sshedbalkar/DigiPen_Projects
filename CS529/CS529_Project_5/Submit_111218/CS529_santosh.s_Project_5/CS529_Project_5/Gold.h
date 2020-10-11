////////////////////////////////////////////////////////////////
//
//	Gold.h
//	implements the Gold coin logic
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef GOLD_H
#define GOLD_H
//
#include "Component.h"
#include "VMath.h";
//
namespace Wye
{
	class Sprite;
	class Transform;
	//
	class Gold: public Component
	{
	public:
		Gold();
		~Gold();
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