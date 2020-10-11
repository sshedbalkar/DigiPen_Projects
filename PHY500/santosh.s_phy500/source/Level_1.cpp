#include "Level_1.h"
#include "Core.h"
#include "LoadLevelClass.h"
#include "DebugTools.h"
#include "GSM.h"
#include "Factory.h"
#include "Integration.h"
#include "WindowsSystem.h"
#include "Graphics.h"
//
namespace Framework
{
	//LoadLevelClass* ll;					//#### added temporary line
	Level_1::Level_1()
	{
		//
	}
	Level_1::~Level_1()
	{
		delete m_loader;						//#### added temporary line
	}
	int Level_1::load(void)
	{
		std::cout<<"Load Level: 1\n";
		m_loader = new LoadLevelClass();		//#### added temporary line
		m_loader->Initialize();
		return 1;
	}
	int Level_1::initialize(void)
	{
		std::cout<<"Init Level: 1\n";
		m_loadingEffectTimer = 0.0f;
		m_loadingEffectDone = false;
		//
		Integrator = PhysicsProj::Integrate_Explicit;
		::sprintf_s( int_name, 32, "Integrator: Explicit" );
		//
		GOC *g;
		{
			g = FACTORY->BuildFromArchetype( "GOC_Type_Sphere_Red" );
			g->has( Transform )->Position = Vec3( 0, 0, 10 );
			g->has( Transform )->Scale = Vec3( 2, 2, 2);
			g->Initialize();
			g->has( RigidBody )->setMass( 1 );
			g->has( RigidBody )->setVelocity( 15, 0, 0 );
			m_gocs.push_back( g );
		}
		
		{
			g = FACTORY->BuildFromArchetype( "GOC_Type_Sphere_Blue" );
			g->has( Transform )->Position = Vec3( 0, 0, 12 );
			g->has( Transform )->Scale = Vec3( 2, 2, 2 );
			g->Initialize();
			g->has( RigidBody )->setMass( 1 );
			g->has( RigidBody )->setVelocity( 15, 0, 0 );
			m_gocs.push_back( g );
		}
		{
			g = FACTORY->BuildFromArchetype( "GOC_Type_Sphere_Green" );
			g->has( Transform )->Position = Vec3( 0, 0, 14 );
			g->has( Transform )->Scale = Vec3( 2, 2, 2 );
			g->Initialize();
			g->has( RigidBody )->setMass( 1 );
			g->has( RigidBody )->setVelocity( 15, 0, 0 );
			m_gocs.push_back( g );
		}
		{
			g = FACTORY->BuildFromArchetype( "GOC_Type_Sphere_Orange" );
			g->has( Transform )->Position = Vec3( 0, 0, 16 );
			g->has( Transform )->Scale = Vec3( 2, 2, 2 );
			g->Initialize();
			g->has( RigidBody )->setMass( 1 );
			g->has( RigidBody )->setVelocity( 15, 0, 0 );
			m_gocs.push_back( g );
		}
		
		{
			g = FACTORY->BuildFromArchetype( "GOC_Type_Sphere_Purple" );
			g->has( Transform )->Position = Vec3( 0, 0, 18 );
			g->has( Transform )->Scale = Vec3( 2, 2, 2 );
			g->Initialize();
			g->has( RigidBody )->setMass( 1 );
			g->has( RigidBody )->setVelocity( 15, 0, 0 );
			m_gocs.push_back( g );
		}
		{
			g = FACTORY->BuildFromArchetype( "GOC_Type_Sphere_Grey" );
			g->has( Transform )->Position = Vec3( 0, 0, 20 );
			g->has( Transform )->Scale = Vec3( 2, 2, 2 );
			g->Initialize();
			g->has( RigidBody )->setMass( 1 );
			g->has( RigidBody )->setVelocity( 15, 0, 0 );
			m_gocs.push_back( g );
		}
		{
			g = FACTORY->BuildFromArchetype( "GOC_Type_Sphere_Yellow" );
			g->has( Transform )->Position = Vec3( 0, 0, 0 );
			g->has( Transform )->Scale = Vec3( 5, 5, 5 );
			g->Initialize();
		}
		
		//
		return 1;
	}
	int Level_1::sendMessage( Message *m )
	{
		switch( m->MessageId )
		{
		case Mid::CharacterKey:
			{
				MessageCharacterKey *key = static_cast<MessageCharacterKey*>(m);
				//
				if( key->character== '1' )
				{
					Integrator = PhysicsProj::Integrate_Explicit;
					::sprintf_s( int_name, 32, "Integrator: Explicit" );
				}
				else if( key->character == '2' )
				{
					Integrator = PhysicsProj::Integrate_SemiImplicit;
					::sprintf_s( int_name, 32, "Integrator: Semi Explicit" );
				}
				else if( key->character == '3' )
				{
					Integrator = PhysicsProj::Integrate_Verlet;
					::sprintf_s( int_name, 32, "Integrator: Verlet" );
				}
				else if( key->character == '4' )
				{
					Integrator = PhysicsProj::Integrate_RK4;
					::sprintf_s( int_name, 32, "Integrator: RK4" );
				}
			}
			break;
		}
		return 1;
	}
	int Level_1::update(float dt)
	{
		if(!m_loadingEffectDone){
			m_loadingEffectTimer+=dt;
			if (m_loadingEffectTimer>1.0f){
				m_loadingEffectDone=true;
			}
		}
		Integrate( dt );
		GRAPHICS->SetIntegrator( int_name );
		//std::cout<<"Level_1: update: "<<dt<<"\n";
		CORE->updateSystems(dt);
		return 1;
	}
	void Level_1::Integrate( float dt )
	{
		for each( GOC* goc in m_gocs )
		{
			Integrator( goc->has(RigidBody), dt );
			goc->has( Transform )->Position = Vector3ToVec3( goc->has(RigidBody)->getPosition() );
		}
	}
	int Level_1::draw(void)
	{
		return 1;
	}
	int Level_1::free(void)
	{
		std::cout<<"Free Level: 1\n";
		m_loader->freeLogicData();
		FACTORY->DestroyAllObjects();
		GRAPHICS->Unload();
		return 1;
	}
	int Level_1::unload(void)
	{
		std::cout<<"Unload Level: 1\n";
		return 1;
	}
	int Level_1::id(void)
	{
		return 1;
	}
	int Level_1::restart(void)
	{
		std::cout<<"Restart Level: 1\n";
		CORE->gsm()->next(GSM::LEVEL_RESTART);
		return 1;
	}
}