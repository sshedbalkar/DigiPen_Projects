////////////////////////////////////////////////////////////////
//
//	BombEffect.h
//	Bomb visual effect
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef BOMB_EFFECT_H
#define BOMB_EFFECT_H
//
#include "Component.h"
#include "Message.h"
#include "MsgIds.h"
//
namespace Wye
{
	class Transform;
	class Sprite;
	//
	class BombEffect: public Component
	{
	public:
		BombEffect();
		~BombEffect();
		void initialize();
		void onMessage(const Message& msg);
		inline void radius(float r)
		{
			_maxRadius = r;
		}
		inline float radius()
		{
			return _maxRadius;
		}
	private:
		Transform *_tx;
		Sprite *_display;
		float _radius;
		float _maxRadius;
		bool _done;
	};
}
//
#endif