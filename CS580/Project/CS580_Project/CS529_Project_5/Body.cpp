///////////////////////////////////////////////////////////////////////////////////////
//
//	Body.cpp
//	
//	Authors: Chris Peters
//	Copyright 2010, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////
#include "Precompiled.h"
#include "Body.h"
#include "DebugDraw.h"
#include "Physics.h"
#include "DebugTools.h"
#include "GameLogic.h"
//
namespace Wye
{
	Body::Body()
		:Component(CT_Body),
		Position(0.0f, 0.0f, 0.0f),
		PrevPosition(0.0f, 0.0f, 0.0f),
		Velocity(0.0f, 0.0f, 0.0f),
		Acceleration(0.0f, 0.0f, 0.0f),
		Mass(0.0f),
		InvMass(0.0f),
		Restitution(0.0f),
		Friction(0.0f),
		Damping(0.9f),
		AccumulatedForce(0.0f, 0.0f, 0.0f),
		IsStatic(false),
		BodyShape(NULL)
	{
		//
	}
	void Body::onMessage(const Message& msg)
	{
		//std::cout<<"Body, Msg: " <<msg.MessageId<<", parent: " <<getParent()->getId()<<"\n";
	}
	Body::~Body()
	{
		delete BodyShape;
		PHYSICS->Bodies.erase(this);
	}

	void Body::integrate(float dt)
	{
		//Do not integrate static bodies
		if(IsStatic) return;

		//Store prev position
		PrevPosition = Position;

		//Integrate the position using Euler 
		Position = Position + Velocity * dt; //acceleration term is small

		//Determine the acceleration
		Acceleration = PHYSICS->Gravity;
		Vec3 newAcceleration = AccumulatedForce * InvMass + Acceleration;

		//Integrate the velocity
		Velocity = Velocity + newAcceleration * dt;

		//Dampen the velocity for numerical stability and soft drag
		Velocity *= std::pow(Damping, dt);

		//Clamp to velocity max for numerical stability
		if (dot(Velocity, Velocity) > PHYSICS->MaxVelocitySq)
		{
			normalize(Velocity);
			Velocity = Velocity * PHYSICS->MaxVelocity;
		}

		//Clear the force
		AccumulatedForce = Vec3(0.0f, 0.0f, 0.0f);
	}

	void Body::publishResults()
	{
		if(!IsStatic)
		{
			tx->position = Position;
		}
		else
		{
			Position = tx->position;
		}
	}

	void Body::debugDraw()
	{


		if(IsStatic)
		{
			//White
			Drawer::Instance.setColor(Vec4(1, 1, 1, 1));

			//Draw the shape of the object
			BodyShape->draw();
		}
		else
		{		
			//Red
			Drawer::Instance.setColor(Vec4(1, 0, 0, 1));

			//Draw the shape of the object
			BodyShape->draw();

			//Draw the velocity of the object
			Drawer::Instance.setColor(Vec4(1, 1, 1, 1));
			Drawer::Instance.moveTo(Position);
			Drawer::Instance.lineTo(Position + Velocity * 0.25f);
		}
	}

	void Body::initialize()
	{ 
		//Get the transform to write results to
		tx = ((*getParent()).getComponent<Transform>(CT_Transform));
		
		//Get the starting position
		Position = tx->position;

		//Add this body to the body list
		PHYSICS->Bodies.push_back(this);

		//If mass is zero object is interpreted
		//to be static
		if(Mass > 0.0f)
		{
			IsStatic = false;
			InvMass = 1.0f / Mass;
		}
		else
		{
			IsStatic = true;
			InvMass = 0.0f;
		}

		BodyShape->body = this;
		//
		//std::cout<<"Body Inited: " <<getId()<<"\n";
	}

	void Body::serialize(const ISerializer& stream)
	{
		//std::cout<<"In Body::serialize\n";
		ISerializer* str = stream.clone();
		str->readProperty("Data", *str);
		int s;
		str->readProperty("shape", s);
		if(s == Shape::Sid_Circle)
		{
			ShapeCircle* shape = new ShapeCircle();
			shape->serialize(*str);
			this->BodyShape = shape;
		}
		else if(s == Shape::Sid_Box)
		{
			ShapeAAB* shape = new ShapeAAB();
			shape->serialize(*str);
			this->BodyShape = shape;
		}
		str->readProperty("mass", Mass);
		str->readProperty("resitution", Restitution);
		str->readProperty("friction", Friction);
		str->readProperty("damping", Damping);
	}

	void Body::addForce(const Vec3& force)
	{
		AccumulatedForce += force;
	}

	void Body::setPosition(const Vec3& pos)
	{
		Position = pos;
		tx->position = pos;
	}

	void Body::setVelocity(const Vec3& vel)
	{
		Velocity = vel;
	}
	void Body::size(const Vec3& sz)
	{
		BodyShape->size(sz);
	}
	void Body::makeStationary()
	{
		AccumulatedForce = Vec3(0.0f, 0.0f, 0.0f);
		Velocity = Vec3(0.0f, 0.0f, 0.0f);
		Acceleration = Vec3(0.0f, 0.0f, 0.0f);
		//std::cout<<"Body: Made Zero\n";
	}
}
