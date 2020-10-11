#include "BombEffect.h"
#include "Sprite.h"
#include "Transform.h"
#include "ComponentTypeIds.h"
#include "Composition.h"
#include "Factory.h"
#include "GameData.h"
#include "Core.h"
#include "DebugTools.h"
//
static float BOMB_RADIUS = 100;
namespace Wye
{
	BombEffect::BombEffect()
		:Component(),
		_done(false),
		_maxRadius(BOMB_RADIUS)
	{
		//
	}
	BombEffect::~BombEffect()
	{
		CORE->unregisterForUpdateMsg(this);
	}
	void BombEffect::initialize()
	{
		_tx = ((*getParent()).getComponent<Transform>(CT_Transform));
		_display = ((*getParent()).getComponent<Sprite>(CT_Sprite));
		//_display = FACTORY->createComponent(CT_Sprite);
		_radius = GAMEDATA->blockSize();
		_display->setSpriteName("t_34");
		_display->size(Vec2(_radius, _radius));
		CORE->registerForUpdateMsg(this);
	}
	void BombEffect::onMessage(const Message& msg)
	{
		if(_done) return;
		if(msg.MessageId == Msgid::EnterFrame)
		{
			const EnterFrameMsg* m = static_cast<const EnterFrameMsg*>(&msg);
			_radius += 2 * _maxRadius * m->dt;
			_display->size(Vec2(_radius, _radius));
			if(_radius > _maxRadius)
			{
				_done = true;
				Message msg(Msgid::BombEffectDone);
				(*getParent()).onMessage(msg);
				(*getParent()).destroy();
			}
		}
	}
}