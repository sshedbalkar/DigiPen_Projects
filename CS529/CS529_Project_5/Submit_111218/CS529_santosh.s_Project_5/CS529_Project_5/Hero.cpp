#include "Hero.h"
#include "GameData.h"
#include "GameLogic.h"
#include "DebugTools.h"
#include "ComponentTypeIds.h"
#include "Serialization.h"
#include "WindowsSystem.h"
#include "Sprite.h"
#include "GameUtility.h"
#include "Body.h"
#include "Utility.h"
#include "Message.h"
#include "Core.h"
#include "MsgIds.h"
#include "Transform.h"
#include "Physics.h"
#include "CollisionMap.h"
#include "MovieClip.h"
#include "Bullet.h"
#include "Bomb.h"
#include "Block.h"
//
namespace Wye
{
	Hero::Hero()
		:Controller(),
		_lastUpd(0.0f),
		_dir(1, 0),
		_firstKey(true),
		_lastWepTime(0.0f),
		_lastRestart(0),
		_alive(true),
		_airJump(true)
	{
		setType(CT_Hero);
	}
	//
	Hero::~Hero()
	{
		std::cout<<"Hero deletedddddd\n";
	}
	void Hero::initialize()
	{
		LOGIC->hero(this);
		Controller::initialize();
		//
		_display = ((*getParent()).getComponent<MovieClip>(CT_MovieClip));
		_dispTx = _display->getTransform();
		//
		_body = ((*getParent()).getComponent<Body>(CT_Body));
		_tx = ((*getParent()).getComponent<Transform>(CT_Transform));
		_body->Position = coordToPixel(_coord.x, _coord.y);
		//_coord = pixelToCoord(_body->Position);
		_flags = COLLISION_AI_1|COLLISION_BLOCK_1|COLLISION_WALL;
		COLLISION_MAP->setBit((uint)_coord.x, (uint)_coord.y, COLLISION_HERO);
		//_position.x = _coord.x + 0.5f;
		//_position.y = _coord.y + 0.5f;
		//_velocity = Vec3(4, 0, 0);
		//
		//CORE->addEventListener(Msgid::EnterFrame, this, &Hero::onEvent);
		//std::cout<<"HERO inited, Body id: "<<_body->getId()<<", coord: " <<_coord<<", Pos: " <<_body->Position<<"\n";
	}
	
	void Hero::serialize(const ISerializer& stream)
	{
		Controller::serialize(stream);
		ISerializer* str = stream.clone();
		str->readProperty("Data", *str);
		//
		str->readProperty("start", _coord);
		str->readProperty("speed", _force);
		//
		delete str;
	}
	void Hero::restart()
	{
		std::cout<<"Restarting hero\n";
		_lastUpd = 0;
		_lastWepTime = 0;
		_lastRestart = 0;
		_dir.x = 1.0f;
		_dir.y = 0.0f;
		_coord.x = 1;
		_coord.y = 1;
		_body->makeStationary();
		_body->Position = coordToPixel(_coord.x, _coord.y);
	}
	void Hero::update(float dt)
	{
		if(_alive)
		{
			_dt = dt;
			_lastUpd += dt;
			_lastRestart += dt;
			_lastWepTime += dt;
			checkDisplay();
			updateCoord();
		}
	}
	bool Hero::updateCoord()
	{
		//Vec3 v1 = _body->Position;
		//v1.x = (ceil(v1.x) + floor(v1.x))/2;
		//v1.y = (ceil(v1.y) + floor(v1.y))/2;
		Vec2 v2 = pixelToCoord(_body->Position);
		//std::cout<<"Pos: " <<_tx->position<<", Coord: "<<v2<<"\n";
		if(v2.x != _coord.x || v2.y != _coord.y)
		{
			_prevCoord = _coord;
			_coord = v2;
			//
			COLLISION_MAP->clearBit((uint)_prevCoord.x, (uint)_prevCoord.y, COLLISION_HERO);
			COLLISION_MAP->setBit((uint)_coord.x, (uint)_coord.y, COLLISION_HERO);
			//
			//std::cout<<COLLISION_MAP;
			return true;
		}
		return false;
	}
	void Hero::checkDisplay()
	{
		if(lengthSquared(_body->Velocity) > 2.0f)
		{
			_display->gotoAndPlay(20);
		}
		else
		{
			_display->play();
		}
	}
	/*
	void Hero::handleInput()
	{
		if(_lastUpd > 0.5f)
		{
			_lastUpd = 0.0f;
			float mul = _force * _dt * (isShiftDown() ? 50 : 10);
			if(isUpDown())
			{
				if(isOnGround())
				{
					moveDelta(0, mul*0.6);
				}
			}
			if(isDownDown())
			{
				//moveDelta(0, -mul);
			}
			if(isLeftDown())
			{
				moveDelta(-mul, 0);
			}
			if(isRightDown())
			{
				moveDelta(mul, 0);
			}
		}
	}
	*/
	bool Hero::isOnGround()
	{
		//std::cout<<"coords: "<<_coord<<", val: "<<COLLISION_MAP->value(_coord.x, _coord.y+1)<<"\n";
		//std::cout<<COLLISION_MAP;
		bool val= (COLLISION_MAP->testBit(_coord.x, _coord.y+1, COLLISION_WALL) || COLLISION_MAP->testBit(_coord.x, _coord.y+1, COLLISION_BLOCK_1));
		//std::cout<<"On ground: " <<val<<"\n";
		return val;
	}
	void Hero::moveDelta(float xd, float yd)
	{
		//std::cout<<"Adding force: x: " <<xd<<", yd: "<<yd<<", force: " <<_force<<"\n";
		_body->addForce(Vec3(xd, yd, 0.0f));
		if(_firstKey)
		{
			_firstKey = false;
			_display->setShader("s_4");
		}
		if(xd > 0.0f)
		{
			_dir.x = 1.0f;
			_dispTx->rotation = Vec3(0.0f, 0.0f, 0.0f);
		}
		else if(xd < 0.0f)
		{
			_dir.x = -1.0f;
			_dispTx->rotation = Vec3(0.0f, 0.0f, D3DX_PI);
		}
	}
	void Hero::onMessage(const Message& msg)
	{
		//std::cout<<"Msg id: "<<msg.MessageId<<"\n";
		if(!_alive) return;
		switch(msg.MessageId)
		{
			case Msgid::CharacterKey:
				{
					if(_lastUpd > 0.1f)
					{
						_lastUpd = 0.0f;
						MessageCharacterKey* key = (MessageCharacterKey*)&msg;
						float mul = _force * _dt * (isShiftDown() ? 1000 : 2000);
						if(key->character == VK_UP)
						{
							if(isOnGround())
							{
								_airJump = true;
								moveDelta(0, 3*mul);
							}
							else if(_airJump)
							{
								_airJump = false;
								moveDelta(0, 4*mul);
							}
						}
						else if(key->character == VK_DOWN)
						{
							//moveDelta(0, -mul);
						}
						else if(key->character == VK_LEFT)
						{
							moveDelta(-0.5*mul, 0);
						}
						else if(key->character == VK_RIGHT)
						{
							moveDelta(0.5*mul, 0);
						}
						else if(key->character == VK_SPACE)
						{
							Vec3 pos(_body->Position);
						
							Composition* comp;
							if(isShiftDown())
							{
								if(_lastWepTime > 0.1 && LOGIC->playerData.Bombs > 0)
								{
									pos.x += _dir.x * (GAMEDATA->blockSize() + 5);
									comp = LOGIC->createObjectAt("a_12", pos);
									Bomb* bomb = ((*comp).getComponent<Bomb>(CT_Bomb));
									bomb->direction(_dir);
									_lastWepTime = 0.0;
									--LOGIC->playerData.Bombs;
								}
							}
							else
							{
								if(_lastWepTime > 0.1 && LOGIC->playerData.Bullets > 0)
								{
									pos.x += _dir.x * (GAMEDATA->blockSize()/2 + 10);
									comp = LOGIC->createObjectAt("a_11", pos);
									Bullet* bullet = ((*comp).getComponent<Bullet>(CT_Bullet));
									bullet->direction(_dir);
									_lastWepTime = 0.0;
									--LOGIC->playerData.Bullets;
								}
							}
						}
						else if(key->character == 'z')
						{
							//
						}
					}
					break;
				}
			case Msgid::EnterFrame:
				{
					//std::cout<<"Hero: Enter frame\n";
				}
				break;
			case Msgid::Collide:
				{
					const MessageCollide* m = static_cast<const MessageCollide*>(&msg);
					//
					switch(*(m->CollidedWith->getName().c_str()))
					{
					case 'e':
						{
							std::cout<<"Collided with AI\n";
							if(_lastRestart > 0.5)
							{
								if(--LOGIC->playerData.Lives > 0)
								{
									restart();
								}
								else
								{
									Composition* comp = LOGIC->createObjectAt("a_16", Vec3(0, 0, 0.0f));
									//getParent()->destroy();
									_display->gotoAndStop(25);
									_alive = false;
								}
							}
						}
					case 'j':
						{
							std::cout<<"Collided with Gold\n";
							int g;
							GAMEDATA->readSetting("goldValue", g);
							LOGIC->playerData.Coins += g;
						}
						break;
					case 'k':
						{
							std::cout<<"Collided with Key\n";
							Block::removeABlock();
						}
						break;
					}
				}
				break;
		}
	}
}