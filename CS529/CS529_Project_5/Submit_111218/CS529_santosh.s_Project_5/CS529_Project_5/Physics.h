///////////////////////////////////////////////////////////////////////////////////////
///
///	\file Physics.h
///	Basic 2D iterative impulse physics engine.
///	
///	Authors: Chris Peters
///	Copyright 2010, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////////////
#pragma once //Makes sure this header is only included once

#include "VMath.h"
#include "Engine.h"
#include "Collision.h"
#include "Body.h"
#include "Resolution.h"

namespace Wye
{
	class Composition;
	///Message sent when there is a Collsion between two
	///Body Components.
	class MessageCollide: public Message
	{
	public:
		MessageCollide(): Message(Msgid::Collide) {};
		Vec3 ContactNormal;
		float Impulse;
		Composition* CollidedWith;
	};

	///	Basic 2D iterative impulse physics engine system.
	/// Provides the Body Component.
	class Physics: public System
	{
	public:
#pragma message("Including class: Physics")
		Physics();
		virtual void update(float dt);
		virtual std::string getName(){return "Physics";}
		void onMessage(const Message& m);
		Composition* testPoint(Vec3 testPosition);
		void initialize();
	private:
		void integrateBodies(float dt);
		void detectContacts(float dt);
		void publishResults();
		void debugDraw();
		void step(float dt);
		bool DebugDrawingActive;
		float TimeAccumulation;
		CollsionDatabase Collsion;
		ContactSet Contacts;

	public:
		bool AdvanceStep;
		bool StepModeActive;

		typedef ObjectLinkList<Body>::iterator BodyIterator;
		ObjectLinkList<Body> Bodies;

		//Gravity of the world
		Vec3 Gravity;
		//Max velocity for a physics body
		float MaxVelocity;
		float MaxVelocitySq;

		//See Resolution.cpp for use
		//Position correction tolerance
		float PenetrationEpsilon;
		//Position correction resolve percentage
		float PenetrationResolvePercentage;
	};

	//A global pointer to the Physics system, used to access it globally.
	extern Physics* PHYSICS;
}