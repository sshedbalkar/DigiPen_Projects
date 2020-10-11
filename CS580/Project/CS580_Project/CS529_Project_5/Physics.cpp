///////////////////////////////////////////////////////////////////////////////////////
//
//	Physics.cpp
//	
//	Authors: Chris Peters
//	Copyright 2010, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////

#include "Precompiled.h"
#include "Physics.h"
#include "DebugDraw.h"
#include "ComponentCreator.h"
#include "Core.h"
#include "DebugTools.h"
//
namespace Wye
{
	Physics* PHYSICS = NULL;

	Physics::Physics()
	{
		ErrorIf(PHYSICS!=NULL,"Physics already initialized");
		PHYSICS = this;
		DebugDrawingActive = false;
		TimeAccumulation = 0.0f;
		//Gravity = Vec3(0, -400, 0);
		Gravity = Vec3(0, -400, 0);
		MaxVelocity = 1000;
		MaxVelocitySq = MaxVelocity * MaxVelocity;
		PenetrationEpsilon = 0.2f;
		PenetrationResolvePercentage = 0.2f;
		StepModeActive = false;
		AdvanceStep = false;
	}

	void Physics::initialize()
	{
		//RegisterComponent(Body);
		FACTORY->addComponentCreator(CT_Body, new ComponentCreatorType<Body>(CT_Body));
		std::cout<<"Physics inites\n";
	}

	void Physics::integrateBodies(float dt)
	{
		for(BodyIterator it=Bodies.begin(); it!=Bodies.end(); ++it)
		{
			it->integrate(dt);
		}
	}

	void Physics::detectContacts(float dt)
	{
		BodyIterator bodyA = Bodies.begin();
		BodyIterator lastBody = Bodies.last(); //end - 1
		
		//Broad phase should be added this is N^2
		for(; bodyA!=lastBody; ++bodyA)
		{
			BodyIterator bodyB = bodyA;
			++bodyB;
			for(; bodyB!=Bodies.end(); ++bodyB)
			{
				//Do not collide static bodies with other static bodies
				if(!bodyA->IsStatic || !bodyB->IsStatic)
				{
					Collsion.generateContacts((bodyA)->BodyShape, (bodyA)->Position, (bodyB)->BodyShape, (bodyB)->Position, &Contacts);
				}
			}
		}
	}



	void Physics::publishResults()
	{
		//Commit all physics updates
		for(BodyIterator it=Bodies.begin(); it!=Bodies.end(); ++it)
		{
			(it)->publishResults();
		}

		//Broadcast physics collision messages AFTER physics
		//has update the bodies
		for(unsigned i=0; i<Contacts.NumberOfContacts; ++i)
		{
			BodyContact* contact = &Contacts.contactArray[i];
			MessageCollide messageCollide;
			messageCollide.ContactNormal = contact->ContactNormal;
			messageCollide.Impulse = contact->ContactImpulse;
			messageCollide.CollidedWith = contact->Bodies[1]->getParent();
			contact->Bodies[0]->getParent()->onMessage(messageCollide);
			if( contact->Bodies[1] != NULL )
			{
				messageCollide.ContactNormal = -contact->ContactNormal;
				messageCollide.Impulse = contact->ContactImpulse;
				messageCollide.CollidedWith = contact->Bodies[0]->getParent();
				contact->Bodies[1]->getParent()->onMessage(messageCollide);
			}
		}
	}

	void Physics::step(float dt)
	{

		integrateBodies(dt);

		Contacts.reset();

		detectContacts(dt);

		Contacts.resolveContacts(dt);

		publishResults();

	}

	void Physics::update(float dt)
	{
		const float TimeStep = 1.0f / 60.0f;

		if(!StepModeActive)
		{
			TimeAccumulation += dt;
			TimeAccumulation = std::min(TimeAccumulation, TimeStep * 5);
			if(TimeAccumulation > TimeStep)
			{
				TimeAccumulation-= TimeStep;
				step(TimeStep);
			}

		}
		else
		{
			TimeAccumulation = 0.0f;
			if(AdvanceStep)
			{
				step(TimeStep);
				AdvanceStep = false;
			}
		}

		if(DebugDrawingActive)
		{
			debugDraw();
		}
		//std::cout<<"physics update\n";
	}

Composition* Physics::testPoint(Vec3 testPosition)
	{
		for(BodyIterator it=Bodies.begin(); it!=Bodies.end(); ++it)
		{
			if(it->BodyShape->testPoint(testPosition))
			{
				return it->getParent();
			}
		}
		return NULL;
	}

	void Physics::debugDraw()
	{
		//std::cout<<"Debug drawing\n";
		for(BodyIterator it=Bodies.begin(); it!=Bodies.end(); ++it)
		{
			it->debugDraw();
		}
	}

	void Physics::onMessage(const Message& m)
	{
		if(m.MessageId == Msgid::ToggleDebugInfo)
		{
			ToggleDebugDisplay* debugM = (ToggleDebugDisplay*)&m;
			DebugDrawingActive = debugM->debugActive;
		}
	};
}
