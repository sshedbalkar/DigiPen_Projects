///////////////////////////////////////////////////////////////////////////////////////
///
///	\file Body.h  Define Body GameComponent
///	
///	Authors: Chris Peters
///	Copyright 2010, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////////////
#pragma once //Makes sure this header is only included once
#include "Component.h"
#include "VMath.h"
//#include "Collision.h"
#include "Transform.h"

namespace Wye
{
	class Shape;
	///Body Component provides basic point physics dynamics including mass, 
	///velocity, forces, acceleration, and collision resolution.
	///Component will modify transform component attributes every frame.
	class Body: public Component
	{
#pragma message("Including class: Body")
	public:
		Body();
		~Body();

		void addForce(const Vec3& force);
		void integrate(float dt);
		void setPosition(const Vec3& pos);
		void setVelocity(const Vec3& vel);
		void publishResults();
		void size(const Vec3& sz);
		void onMessage(const Message& msg);
		//Draw the object using the debug drawer
		void debugDraw();

		void initialize();
		void serialize(const ISerializer& stream);

		Body* Next;
		Body* Prev;

		Vec3 Position;
		Vec3 PrevPosition;
		Vec3 Velocity;
		Vec3 Acceleration;
		float Mass;
		float InvMass;
		float Restitution;
		float Friction;
		float Damping;
		Vec3 AccumulatedForce;

		//Transform for this body
		Transform* tx;
		//Shape used for collision with this body
		Shape* BodyShape;
		//Static object are immovable fixed objects
		bool IsStatic;
		//
		bool isHeroBody();
		void makeStationary();
	};
}