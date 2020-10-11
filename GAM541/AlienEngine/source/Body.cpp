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
#include "Utility.h"

namespace Framework
{

	Body::Body()
	{
		Position = Vec2(0,0);
		PrevPosition = Vec2(0,0);
		Velocity = Vec2(0,0);
		Mass = 0.0f;
		InvMass = 0.0f;
		Damping = 0.9f;
		Acceleration = Vec2(0,0);
		BodyShape = NULL;
		Friction = 0.0f;
		Restitution = 0.0f;
		IsStatic = false;
		AccumulatedForce = Vec2(0,0);
	}

	Body::~Body()
	{
		delete BodyShape;
		PHYSICS->Bodies.erase(this);
	}

	void Body::Integrate(float dt)
	{
		//Do not integrate static bodies
		if(IsStatic) return;

		//Store prev position
		PrevPosition = Position;

		//Integrate the position using Euler 
		Position = Position + Velocity * dt; //acceleration term is small

		//Determine the acceleration
		Acceleration = PHYSICS->Gravity;
		Vec2 newAcceleration = AccumulatedForce * InvMass + Acceleration;

		//Integrate the velocity
		Velocity = Velocity + newAcceleration * dt;

		//Dampen the velocity for numerical stability and soft drag
		Velocity *= std::pow(Damping, dt);

		//Clamp to velocity max for numerical stability
		if ( dot(Velocity, Velocity) > PHYSICS->MaxVelocitySq )
		{
			normalize(Velocity);
			Velocity = Velocity * PHYSICS->MaxVelocity;
		}

		//Clear the force
		AccumulatedForce = Vec2(0,0);
	}

	void Body::PublishResults()
	{
		tx->Position = Vec3(Position.x,Position.y,0);
	}

	void Body::DebugDraw()
	{


		if(  IsStatic )
		{
			//White
			Drawer::Instance.SetColor( Vec4(1,1,1,1) );

			//Draw the shape of the object
			BodyShape->Draw();
		}
		else
		{		
			//Red
			Drawer::Instance.SetColor( Vec4(1,0,0,1) );

			//Draw the shape of the object
			BodyShape->Draw();

			//Draw the velocity of the object
			Drawer::Instance.SetColor( Vec4(1,1,1,1) );
			Drawer::Instance.MoveTo( Position  );
			Drawer::Instance.LineTo( Position + Velocity * 0.25f );
		

		}

	}

	void Body::Initialize()
	{ 
		//Get the transform to write results to
		tx = GetOwner()->has(Transform);

		//Get the starting position
//		Position = tx->Position;

		//Add this body to the body list
		PHYSICS->Bodies.push_back(this);

		//If mass is zero object is interpreted
		//to be static
		if( Mass > 0.0f )
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
	}

	void Body::Serialize(ISerializer& stream)
	{
		/*
		StreamRead(stream,Mass);
		StreamRead(stream,Friction);
		StreamRead(stream,Restitution);

		std::string shapeName;
		StreamRead(stream,shapeName);

		if( shapeName == "Circle" )
		{
			ShapeCircle * shape = new ShapeCircle();
			StreamRead(stream,shape->Radius);
			this->BodyShape = shape;
		}

		if( shapeName == "Box"  )
		{
			ShapeAAB * shape = new ShapeAAB();
			StreamRead(stream,shape->Extents);
			this->BodyShape  = shape;
		}
		*/
		ISerializer* str = stream.clone();
		if(str->isGood())
		{
			str->readProperty("Mass", *str);
			str->readValue("value", Mass);
			//
			str->goToParent();
			str->readProperty("Restitution", *str);
			str->readValue("value", Restitution);
			//
			str->goToParent();
			str->readProperty("Friction", *str);
			str->readValue("value", Friction);
			//
			std::string s;
			str->goToParent();
			str->readProperty("Shape", *str);
			str->readValue("value", s);
			if(!s.compare("Circle"))
			{
				ShapeCircle* shape = new ShapeCircle();
				str->goToParent();
				str->readProperty("DimensionHalf", *str);
				Vec2 v;
				str->readValue("value", v);
				shape->Radius = v.x;
				this->BodyShape = shape;
				//std::cout<<"Shape: Circle, Radius: "<<v.x<<"\n";
			}
			else if(!s.compare("Box"))
			{
				ShapeAAB* shape = new ShapeAAB();
				str->goToParent();
				str->readProperty("DimensionHalf", *str);
				str->readValue("value", shape->Extents);
				this->BodyShape = shape;
				//std::cout<<"Shape: Box, Extents: "<<shape->Extents<<"\n";
			}
		}
		//std::cout<<"Body: Mass:"<<Mass<<", Restitution: "<<Restitution<<", Friction: " <<Friction<<"\n";
		delete str;
	}

	void Body::AddForce(Vec2 force)
	{
		AccumulatedForce += force;
	}

	void Body::SetPosition(Vec2 p)
	{
		Position = p;
//		tx->Position = p;
	}

	void Body::SetVelocity(Vec2 v)
	{
		Velocity = v;
	}
}
