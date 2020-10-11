#include "AI_1.h"
#include "DebugTools.h"
#include "Hero.h"
#include "ComponentTypeIds.h"
#include "GameUtility.h"
#include "Sprite.h"
#include "Composition.h"
#include "Physics.h"
#include "MsgIds.h"
#include "Block.h"
#include "GameUtility.h"
#include "CollisionMap.h"
#include "GameLogic.h"
#include "Transform.h"
#include "GameData.h"
#include "Bomb.h"
#include "Core.h"
#include "VMath.h"
//
namespace Wye
{
	std::vector<AI_1*> AI_1::_ai;
	//
	AI_1::AI_1()
		:Component(CT_AI_1),
		_currCoord(),
		_type(""),
		_state_1(AI_NONE),
		_maxVel(100),
		_dirChangeRate(0.25),
		_maxIdleTime(0.5f),
		_dontUseDir(-1),
		_dead(false),
		_aiType(AI_REGULAR)
	{
		//setType(CT_AI_1);
		//
		registerAI(this);
	}
	AI_1::~AI_1()
	{
		COLLISION_MAP->clearBit((uint)_currCoord.x, (uint)_currCoord.y, COLLISION_AI_1);
		CORE->unregisterForUpdateMsg(this);
		unregisterAI(this);
	}
	void AI_1::initialize()
	{
		//Controller::initialize();
		CORE->registerForUpdateMsg(this);
		//
		_display = ((*getParent()).getComponent<Sprite>(CT_Sprite));
		//
		_tx = ((*getParent()).getComponent<Transform>(CT_Transform));
		if(*(_type.c_str()) == '3')
		{
			_aiType = AI_SEEKER;
			_display->setSpriteName("t_29");
		}
		_flags = COLLISION_BLOCK_1|COLLISION_WALL|COLLISION_GOLD|COLLISION_KEY;
		//goLeft();
		COLLISION_MAP->setBit((uint)_currCoord.x, (uint)_currCoord.y, COLLISION_AI_1);
	}
	void AI_1::serialize(const ISerializer& stream)
	{
		ISerializer* str = stream.clone();
		str->readProperty("Data", *str);
		//
		str->readProperty("maxVel", _maxVel);
		str->readProperty("dirChangeRate", _dirChangeRate);
		str->readProperty("maxIdleTime", _maxIdleTime);
		str->readProperty("seekRad", _seekRadius);
		_seekRadius *= _seekRadius;
		//
		delete str;
	}
	void AI_1::update(float dt)
	{
		_dt = dt;
		_lastUpd += dt;
		updateState();
	}
	void AI_1::updateState()
	{
		switch(_state_1)
		{
		case AI_NONE:
			{
				_state_1 = AI_IDLE;
				_state_2 = AI_ENTER;
			}
			break;
		case AI_IDLE:
			{
				switch(_state_2)
				{
				case AI_ENTER:
					{
						//std::cout<<"enter\n";
						_lastUpd = 0.0f;
						_state_2 = AI_UPDATE;
					}
					break;
				case AI_UPDATE:
					{
						//std::cout<<"update\n";
						if(_lastUpd > _maxIdleTime)
						{
							_lastUpd = 0.0f;
							if(changeDirection(_dontUseDir))
							{
								_state_1 = AI_MOVING;
								_state_2 = AI_ENTER;
								_dontUseDir = AI_NONE;
							}
						}
					}
					break;
				}
			}
			break;
		case AI_MOVING:
			{
				switch(_state_2)
				{
				case AI_ENTER:
					{
						_distDelta = 0.0f;
						_state_2 = AI_UPDATE;
					}
				case AI_UPDATE:
					{
						if(_distDelta < GAMEDATA->blockSize())
						{
							moveBody(_dir);
						}
						else
						{
							//std::cout<<"Stopped\n";
							_prevCoord = _currCoord;
							_currCoord += _dir;
							COLLISION_MAP->clearBit((uint)_prevCoord.x, (uint)_prevCoord.y, COLLISION_AI_1);
							COLLISION_MAP->setBit((uint)_currCoord.x, (uint)_currCoord.y, COLLISION_AI_1);
							//
							snapBody();
							//_state_2 = AI_EXIT;
							if(randomFloat(0.0f, 1.0f) < _dirChangeRate)
							{
								_state_1 = AI_IDLE;
								if(_dir.x < 0)
								{
									_dontUseDir = AI_GOING_LEFT;
								}
								else if(_dir.x > 0)
								{
									_dontUseDir = AI_GOING_RIGHT;
								}
								else if(_dir.y < 0)
								{
									_dontUseDir = AI_GOING_TOP;
								}
								else if(_dir.y > 0)
								{
									_dontUseDir = AI_GOING_BOTTOM;
								}
							}
							else
							{
								if( ((_dir.x < 0) && canGoLeft()) ||
									((_dir.x > 0) && canGoRight()) ||
									((_dir.y < 0) && canGoTop()) ||
									((_dir.y > 0) && canGoBottom()))
								{
									//
								}
								else
								{
									_state_1 = AI_IDLE;
								}
							}
							_state_2 = AI_ENTER;
						}
					}
					break;
				case AI_EXIT:
					{
						
					}
					break;
				}
				
			}
			break;
		}
		
	}
	void AI_1::snapBody()
	{
		_tx->position = coordToPixel(_currCoord.x, _currCoord.y);
	}
	bool AI_1::seek()
	{
		Vec2 diff = LOGIC->hero()->coord() - _currCoord;
		float dist = lengthSquared(diff);
		if(dist < _seekRadius)
		{
			float d1 = 99999999, d2 = 99999999, d3 = 99999999, d4 = 99999999;
			Vec2 dr;
			if(canGoLeft())
			{
				dr = _currCoord + Vec2(-1, 0);
				diff = LOGIC->hero()->coord() - dr;
				d1 = lengthSquared(diff);
			}
			if(canGoRight())
			{
				dr = _currCoord + Vec2(1, 0);
				diff = LOGIC->hero()->coord() - dr;
				d2 = lengthSquared(diff);
			}
			if(canGoTop())
			{
				dr = _currCoord + Vec2(0, -1);
				diff = LOGIC->hero()->coord() - dr;
				d3 = lengthSquared(diff);
			}
			if(canGoBottom())
			{
				dr = _currCoord + Vec2(0, 1);
				diff = LOGIC->hero()->coord() - dr;
				d4 = lengthSquared(diff);
			}
			if(d1 <= d2 && d1 <= d3 && d1 <= d4)
			{
				_dir = Vec2(-1, 0);
			}
			else if(d2 <= d1 && d2 <= d3 && d2 <= d4)
			{
				_dir = Vec2(1, 0);
			}
			else if(d3 <= d1 && d3 <= d2 && d3 <= d4)
			{
				_dir = Vec2(0, -1);
			}
			else if(d4 <= d1 && d4 <= d2 && d4 <= d3)
			{
				_dir = Vec2(0, 1);
			}
			return true;
		}
		return false;
	}
	bool AI_1::changeDirection(int fn)
	{
		if(_aiType != AI_SEEKER || (_aiType == AI_SEEKER && !seek()))
		{
			
			std::vector<int> fns = getAvailableDirections(fn);
			if(fns.size() > 0)
			{
				int i = floor(randomFloat(0.0f, fns.size()-0.2f));
				//std::cout<<"Launching: "<<i<<", Size: "<< fns.size()<<"\n";
				std::vector<int>::iterator it = fns.begin()+i;
				if(*it == AI_GOING_LEFT)
				{
					_dir = Vec2(-1, 0);
				}
				else if(*it == AI_GOING_RIGHT)
				{
					_dir = Vec2(1, 0);
				}
				else if(*it == AI_GOING_TOP)
				{
					_dir = Vec2(0, -1);
				}
				else if(*it == AI_GOING_BOTTOM)
				{
					_dir = Vec2(0, 1);
				}
				return true;
			}
		}
		else
		{
			return true;
		}
		return false;
	}
	void AI_1::moveBody(const Vec2& delta)
	{
		float f = _maxVel * _dt;
		Vec3 pos(delta.x * f, -delta.y * f, 0.0f);
		_tx->position += pos;

		_distDelta += f * sqrt(lengthSquared(delta));
		//updateCoord();
	}
	bool AI_1::canGoLeft()
	{
		int val = COLLISION_MAP->value(_currCoord.x-1, _currCoord.y);
		if(!(val & _flags))
		{
			//std::cout<<"Curr: " <<_currCoord<<" Left Val: " <<val<<"TRUE\n";
			return true;
		}
		return false;
	}
	bool AI_1::canGoRight()
	{
		int val = COLLISION_MAP->value(_currCoord.x+1, _currCoord.y);
		//std::cout<<"Curr: " <<_currCoord<<" Right Val: " <<val<<"\n";
		if(!(val & _flags))
		{
			//std::cout<<"Curr: " <<_currCoord<<" Right Val: " <<val<<"TRUE\n";
			return true;
		}
		return false;
	}
	bool AI_1::canGoTop()
	{
		int val = COLLISION_MAP->value(_currCoord.x, _currCoord.y-1);
		if(!(val & _flags))
		{
			return true;
		}
		return false;
	}
	bool AI_1::canGoBottom()
	{
		int val = COLLISION_MAP->value(_currCoord.x, _currCoord.y+1);
		if(!(val & _flags))
		{
			return true;
		}
		return false;
	}
	void AI_1::onMessage(const Message& msg)
	{
		switch(msg.MessageId)
		{
		case Msgid::EnterFrame:
			{
				const EnterFrameMsg* m = static_cast<const EnterFrameMsg*>(&msg);
				update(m->dt);
			}
			break;
		case Msgid::Collide:
			{
				const MessageCollide* m = static_cast<const MessageCollide*>(&msg);
				//
				switch(*(m->CollidedWith->getName().c_str()))
				{
				case 'f':
				case 'g':
					{
						getParent()->destroy();
					}
					break;
				}
			}
			break;
		case Msgid::BombExplode:
			{
				if(_dead) return;
				const BombExplodeMsg* m = static_cast<const BombExplodeMsg*>(&msg);
				Vec3& diff = _tx->position - m->Position;
				float lensq = lengthSquared(diff);
				if(lensq < (m->Radius * m->Radius))
				{
					_dead = true;
					getParent()->destroy();
				}
			}
			break;
		}
	}
	bool AI_1::updateCoord()
	{
		Vec2 v2 = pixelToCoord(_tx->position);
		//std::cout<<"Pos: " <<_tx->position<<", Coord: "<<v2<<"\n";
		if(v2.x != _currCoord.x || v2.y != _currCoord.y)
		{
			_prevCoord = _currCoord;
			_currCoord = v2;
			//
			COLLISION_MAP->clearBit((uint)_prevCoord.x, (uint)_prevCoord.y, COLLISION_AI_1);
			COLLISION_MAP->setBit((uint)_currCoord.x, (uint)_currCoord.y, COLLISION_AI_1);
			//
			return true;
		}
		return false;
	}
	std::vector<int> AI_1::getAvailableDirections(int dir)
	{
		std::vector<int> dirs;
		if(dir != AI_GOING_LEFT && canGoLeft())
		{
			dirs.push_back(AI_GOING_LEFT);
		}
		if(dir != AI_GOING_RIGHT && canGoRight())
		{
			dirs.push_back(AI_GOING_RIGHT);
		}
		if(dir != AI_GOING_TOP && canGoTop())
		{
			dirs.push_back(AI_GOING_TOP);
		}
		if(dir != AI_GOING_BOTTOM && canGoBottom())
		{
			dirs.push_back(AI_GOING_BOTTOM);
		}
		return dirs;
	}
}