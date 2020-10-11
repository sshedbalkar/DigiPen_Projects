#include "Level_2.h"
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
	Level_2::Level_2()
	{
		//
	}
	Level_2::~Level_2()
	{
		delete m_loader;						//#### added temporary line
	}
	int Level_2::load(void)
	{
		std::cout<<"Load Level: 2\n";
		m_loader = new LoadLevelClass();		//#### added temporary line
		m_loader->Initialize();
		return 1;
	}
	int Level_2::initialize(void)
	{
		std::cout<<"Init Level: 2\n";
		m_loadingEffectTimer = 0.0f;
		m_loadingEffectDone = false;
		//
		GOC *g;
		{
			g = FACTORY->BuildFromArchetype( "GOC_Type_Sphere_Red" );
			g->has( Transform )->Position = Vec3( 0, 0, 16 );
			g->has( Transform )->Scale = Vec3( 2, 2, 2);
			g->Initialize();
			g->has( RigidBody )->setMass( 1 );
			g->has( RigidBody )->setVelocity( 15, 0, 0 );
			goc_explicit = g;
		}
		
		{
			g = FACTORY->BuildFromArchetype( "GOC_Type_Sphere_Blue" );
			g->has( Transform )->Position = Vec3( 0, 0, 16 );
			g->has( Transform )->Scale = Vec3( 2, 2, 2 );
			g->Initialize();
			g->has( RigidBody )->setMass( 1 );
			g->has( RigidBody )->setVelocity( 15, 0, 0 );
			goc_semiExplicit = g;
		}
		{
			g = FACTORY->BuildFromArchetype( "GOC_Type_Sphere_Green" );
			g->has( Transform )->Position = Vec3( 0, 0, 16 );
			g->has( Transform )->Scale = Vec3( 2, 2, 2 );
			g->Initialize();
			g->has( RigidBody )->setMass( 1 );
			g->has( RigidBody )->setVelocity( 15, 0, 0 );
			goc_verlet = g;
		}
		{
			g = FACTORY->BuildFromArchetype( "GOC_Type_Sphere_Orange" );
			g->has( Transform )->Position = Vec3( 0, 0, 16 );
			g->has( Transform )->Scale = Vec3( 2, 2, 2 );
			g->Initialize();
			g->has( RigidBody )->setMass( 1 );
			g->has( RigidBody )->setVelocity( 15, 0, 0 );
			goc_rk4 = g;
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
	int Level_2::sendMessage( Message *m )
	{
		return 1;
	}
	int Level_2::update(float dt)
	{
		if(!m_loadingEffectDone){
			m_loadingEffectTimer+=dt;
			if (m_loadingEffectTimer>1.0f){
				m_loadingEffectDone=true;
			}
		}
		GRAPHICS->SetIntegrator( "" );
		Integrate( dt );
		//std::cout<<"Level_2: update: "<<dt<<"\n";
		CORE->updateSystems(dt);
		return 1;
	}
	void Level_2::Integrate( float dt )
	{
		PhysicsProj::Integrate_Explicit( goc_explicit ->has(RigidBody), dt );
		goc_explicit->has( Transform )->Position = Vector3ToVec3( goc_explicit->has(RigidBody)->getPosition() );
		//
		PhysicsProj::Integrate_SemiImplicit( goc_semiExplicit ->has(RigidBody), dt );
		goc_semiExplicit->has( Transform )->Position = Vector3ToVec3( goc_semiExplicit->has(RigidBody)->getPosition() );
		//
		PhysicsProj::Integrate_Verlet( goc_verlet ->has(RigidBody), dt );
		goc_verlet->has( Transform )->Position = Vector3ToVec3( goc_verlet->has(RigidBody)->getPosition() );
		//
		PhysicsProj::Integrate_RK4( goc_rk4 ->has(RigidBody), dt );
		goc_rk4->has( Transform )->Position = Vector3ToVec3( goc_rk4->has(RigidBody)->getPosition() );
	}
	int Level_2::draw(void)
	{
		return 1;
	}
	int Level_2::free(void)
	{
		std::cout<<"Free Level: 2\n";
		m_loader->freeLogicData();
		FACTORY->DestroyAllObjects();
		GRAPHICS->Unload();
		return 1;
	}
	int Level_2::unload(void)
	{
		std::cout<<"Unload Level: 2\n";
		return 1;
	}
	int Level_2::id(void)
	{
		return 2;
	}
	int Level_2::restart(void)
	{
		std::cout<<"Restart Level: 2\n";
		CORE->gsm()->next(GSM::LEVEL_RESTART);
		return 1;
	}
}