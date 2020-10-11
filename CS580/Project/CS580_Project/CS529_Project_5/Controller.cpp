
#include "Controller.h"
#include "ComponentTypeIds.h"
#include "GameLogic.h"
#include "WindowsSystem.h"
#include "DebugTools.h"
//
namespace Wye
{
	Controller::Controller()
		:Component(CT_Controller),
		_transform(NULL),
		_speed(20.0f)
	{
		//
	}

	Controller::~Controller()
	{
		//LOGIC->getControllers()->erase(this);
		LOGIC->removeComponent(this);
	}

	void Controller::initialize()
	{
		_transform = ((*getParent()).getComponent<Transform>(CT_Transform));
		//LOGIC->getControllers()->push_back(this);
		LOGIC->addComponent(this);
		//
		std::cout<<"Controller: Speed: "<<_speed<<"\n";
	}

	void Controller::update(float dt)
	{
		int mul = isShiftDown() ? 5 : 1;
		if(isUpDown())
		{
			_transform->position.y += _speed * dt * mul;
		}
		if(isDownDown())
		{
			_transform->position.y -= _speed * dt * mul;
		}
		if(isLeftDown())
		{
			_transform->position.x -= _speed * dt * mul;
		}
		if(isRightDown())
		{
			_transform->position.x += _speed * dt * mul;
		}
	}
	
	float Controller::speed()
	{
		return _speed;
	}
	void Controller::speed(const float& sp)
	{
		_speed = sp;
	}
	void Controller::onMessage(const Message& msg)
	{
		const MouseWheel* m = dynamic_cast<const MouseWheel*>(&msg);
		if(m)
		{
			_transform->position.z += _speed * m->Delta * 0.02f;
		}
	}
	void Controller::serialize(const ISerializer& stream)
	{
		ISerializer* str = stream.clone();
		str->readProperty("Data", *str);
		str->readProperty("speed", _speed);
		//
		delete str;
	}
	Transform* Controller::transform()
	{
		return _transform;
	}
}
