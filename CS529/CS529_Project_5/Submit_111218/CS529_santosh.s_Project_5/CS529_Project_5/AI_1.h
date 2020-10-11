////////////////////////////////////////////////////////////////
//
//	AI_1.h
//	AI componnet, implements 2 types of behaviours. 1. Basic move in one of the available directions. 2. Seek out the hero if he is in the range.
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef AI_1_H
#define AI_1_H
//
#include "Component.h"
#include "VMath.h"
#include <algorithm>
#include <vector>
//
namespace Wye
{
	class Message;
	class ISerializer;
	class Sprite;
	class Body;
	class Hero;
	class Block;
	class MessageCollide;
	class Transform;
	//
	class AI_1: public Component
	{
#pragma message("Including class: AI_1")
	public:
		AI_1();
		virtual ~AI_1();
		void initialize();
		void onMessage(const Message& msg);
		void serialize(const ISerializer& stream);
		inline Vec2 coord()const
		{
			return _currCoord;
		}
		inline void coord(const Vec2& pos)
		{
			_currCoord = pos;
		}
		inline void type(const char* typ)
		{
			_type = typ;
		}
		//
	private:
		float _maxVel;
		float _dirChangeRate;
		float _maxIdleTime;
		//
		enum AI_STATES
		{
			AI_NONE,
			AI_IDLE,
			AI_MOVING,
			AI_GOING_LEFT,
			AI_GOING_RIGHT,
			AI_GOING_TOP,
			AI_GOING_BOTTOM,
			//
			AI_ENTER,
			AI_UPDATE,
			AI_EXIT,
			AI_MAX
		};
		enum AI_TYPES
		{
			AI_REGULAR,
			AI_SEEKER
		};
		int _state_1;
		int _state_2;
		//
		static std::vector<AI_1*> _ai;
		static void registerAI(AI_1* ai)
		{
			std::vector<AI_1*>::iterator it = std::find(_ai.begin(), _ai.end(), ai);
			if(it == _ai.end())
			{
				_ai.push_back(ai);
			}
		}
		static void unregisterAI(AI_1* ai)
		{
			std::vector<AI_1*>::iterator it = std::find(_ai.begin(), _ai.end(), ai);
			if(it != _ai.end())
			{
				_ai.erase(it);
			}
		}
		//
		void update(float dt);
		void onCollideHero(Hero* hero, const MessageCollide* msg);
		void onCollideWall(const MessageCollide* msg);
		void onCollideAI_1(AI_1* ai, const MessageCollide* msg);
		void onCollideBlock(Block* block, const MessageCollide* msg);
		//
		Vec2 _currCoord;
		Vec2 _prevCoord;
		Vec2 _nextCoord;
		Vec2 _dir;
		float _distDelta;
		Transform* _tx;
		Sprite* _display;
		std::string _type;
		//
		float _lastUpd;
		float _dt;
		//
		std::vector<int> AI_1::getAvailableDirections(int dir);
		bool canGoLeft();
		bool canGoRight();
		bool canGoTop();
		bool canGoBottom();
		//
		void updateState();
		bool updateCoord();
		void moveBody(const Vec2& delta);
		bool changeDirection(int fn=-1);
		void snapBody();
		int _dontUseDir;
		bool _dead;
		bool seek();
		int _aiType;
		float _seekRadius;
		int _flags;
	};
}
//
#endif