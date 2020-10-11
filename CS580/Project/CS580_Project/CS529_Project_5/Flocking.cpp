#include "Flocking.h"
#include <iostream>
#include "GameLogic.h"
#include "Utility.h"
#include "Boid.h"
#include "Composition.h"
#include "ComponentTypeIds.h"
//
namespace Wye
{
	//
	Boid *Boids[MAX_BOIDS];
	Flocking::Flocking()
	{

	}
	Flocking::~Flocking()
	{

	}
	void Flocking::Init()
	{
		std::cout<<"Flocking\n";
		//
		Vec3 pos, rot, sc;
		Composition* comp;
		Boid *boid;
		for( int i = 0; i < MAX_BOIDS; ++i )
		{
			//pos.x = randomFloat( -25.0f, 25.0f );
			//pos.y = randomFloat( -25.0f, 25.0f );
			//pos.z = randomFloat( 1.0f, 50.0f );
			pos.x = 0.0f;
			pos.y = 0.0f;
			pos.z = 0.0f;
			//std::cout<<"Pos: "<<pos<<"\n";
			//
			rot.x = 0.0f;
			rot.y = 0.0f;
			rot.z = 0.0f;
			//
			sc.x = 1.0f;
			sc.y = 1.0f;
			sc.z = 1.04f;
			//
			comp = LOGIC->createObjectAt( "a_2", pos, rot, sc );
			//
			boid = comp->getComponent<Boid>(CT_Boid);
			Boids[i] = boid;
			boid->BoidList( Boids );
		}
		Boid::UpdateDispParams();
	}
	void Flocking::Run( float dt )
	{

	}
}