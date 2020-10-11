////////////////////////////////////////////////////////////////
//
//	Controller.h
//	implements the controller which handles user inputs
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef MOVE_CONTROLLER_H
#define MOVE_CONTROLLER_H
//#pragma message("Compiling: "__FILE__)
//
#include "Transform.h"
//
namespace Wye
{
	class Message;
	class ISerializer;
	class Component;
	///Sample Demo Component Movement Controller. Used
	///to move objects around the world not under
	///the influence of physics.
	class Controller: public Component
	{
//#pragma message("Including Class: Controller")
	public:
		Controller* Next;
		Controller* Prev;
		Controller();
		~Controller();
		virtual void initialize();
		virtual void update(float dt);
		Transform* transform();
		float speed();
		void speed(const float& sp);
		virtual void onMessage(const Message& msg);
		void serialize(const ISerializer& stream);
	private:
		Transform* _transform;
		float _speed;
	};
}
//
#endif
