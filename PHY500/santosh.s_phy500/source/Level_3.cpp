#include "Level_3.h"
#include "Core.h"
#include "LoadLevelClass.h"
#include "DebugTools.h"
#include "GSM.h"
#include "Factory.h"
#include "Integration.h"
#include "WindowsSystem.h"
#include "Graphics.h"
//
float RandomFloat( float min, float max )
{
	float r = ( (float)::rand() ) / ( float )RAND_MAX;
	return min + ( r * (max - min) );
}
int RandomInt( int min, int max )
{
	return ( ::rand() % (max-min) )+ min;
}
namespace Framework
{
	//LoadLevelClass* ll;					//#### added temporary line
	Level_3::Level_3()
	{
		//
	}
	Level_3::~Level_3()
	{
		delete m_loader;						//#### added temporary line
	}
	int Level_3::load(void)
	{
		std::cout<<"Load Level: 3\n";
		m_loader = new LoadLevelClass();		//#### added temporary line
		m_loader->Initialize();
		return 1;
	}
	int Level_3::initialize(void)
	{
		std::cout<<"Init Level: 3\n";
		m_loadingEffectTimer = 0.0f;
		m_loadingEffectDone = false;
		//
		for( int i = 0; i < 3; ++i )
		{
			AddObjExplicit();
			AddObjSemiexplicit();
			AddObjVerlet();
			AddObjRK4();
		}
		//
		GOC *g = FACTORY->BuildFromArchetype( "GOC_Type_Sphere_Yellow" );
		g->has( Transform )->Position = Vec3( 0, 0, 0 );
		g->has( Transform )->Scale = Vec3( 5, 5, 5 );
		g->Initialize();
		//
		return 1;
	}
	void Level_3::AddObjExplicit()
	{
		GOC *g = FACTORY->BuildFromArchetype( "GOC_Type_Sphere_Red" );
		g->has(Transform)->Position = Vec3( RandomInt( -20, 20 ), RandomInt( -20, 20 ), RandomInt( -20, 20 ) );
		g->has(Transform)->Scale = Vec3( 2, 2, 2 );
		g->Initialize();
		g->has( RigidBody )->setMass( 1 );
		//
		Vector3 vel = g->has(RigidBody)->position.crossProduct( Vector3(0,1,0) );
		vel.normalise();
		vel *= 15;
		g->has( RigidBody )->setVelocity( vel );
		//
		m_explicits.push_back( g );
	}
	void Level_3::AddObjSemiexplicit()
	{
		GOC *g = FACTORY->BuildFromArchetype( "GOC_Type_Sphere_Blue" );
		g->has( Transform )->Position = Vec3( RandomInt( -20, 20 ), RandomInt( -20, 20 ), RandomInt( -20, 20 ) );
		g->has( Transform )->Scale = Vec3( 2, 2, 2 );
		g->Initialize();
		g->has( RigidBody )->setMass( 1 );
		//
		Vector3 vel = g->has(RigidBody)->position.crossProduct( Vector3(0, 1, 0) );
		vel.normalise();
		vel *= 15;
		g->has( RigidBody )->setVelocity( vel );
		//
		m_semiExplicits.push_back( g );
	}
	void Level_3::AddObjVerlet()
	{
		GOC *g = FACTORY->BuildFromArchetype( "GOC_Type_Sphere_Green" );
		g->has( Transform )->Position = Vec3( RandomInt( -20, 20 ), RandomInt( -20, 20 ), RandomInt( -20, 20 ) );
		g->has( Transform )->Scale = Vec3( 2, 2, 2 );
		g->Initialize();
		g->has( RigidBody )->setMass( 1 );
		//
		Vector3 vel = g->has(RigidBody)->position.crossProduct( Vector3(0, 1, 0) );
		vel.normalise();
		vel *= 15;
		g->has( RigidBody )->setVelocity( vel );
		//
		m_verlets.push_back( g );
	}
	void Level_3::AddObjRK4()
	{
		GOC *g= FACTORY->BuildFromArchetype( "GOC_Type_Sphere_Orange" );
		g->has( Transform )->Position = Vec3( RandomInt( -20, 20 ), RandomInt( -20, 20 ), RandomInt( -20, 20 ) );
		g->has( Transform )->Scale = Vec3( 2, 2, 2 );
		g->Initialize();
		g->has( RigidBody )->setMass( 1 );
		//
		Vector3 vel = g->has(RigidBody)->position.crossProduct( Vector3(0, 1, 0) );
		vel.normalise();
		vel *= 15;
		g->has( RigidBody )->setVelocity( vel );
		//
		m_rk4s.push_back( g );
	}
	int Level_3::sendMessage( Message *m )
	{
		switch( m->MessageId )
		{
		case Mid::CharacterKey:
			{
				MessageCharacterKey *key = static_cast<MessageCharacterKey*>(m);
				//
				if( key->character =='1' )
				{
					AddObjExplicit();
				}
				else if( key->character == '2' )
				{
					AddObjSemiexplicit();
				}
				else if( key->character == '3' )
				{
					AddObjVerlet();
				}
				else if( key->character == '4' )
				{
					AddObjRK4();
				}
			}
			break;
		}
		//
		return 1;
	}
	int Level_3::update(float dt)
	{
		if(!m_loadingEffectDone){
			m_loadingEffectTimer+=dt;
			if (m_loadingEffectTimer>1.0f){
				m_loadingEffectDone=true;
			}
		}
		GRAPHICS->SetIntegrator( "" );
		Integrate( dt );
		//std::cout<<"Level_3: update: "<<dt<<"\n";
		CORE->updateSystems(dt);
		return 1;
	}
	void Level_3::Integrate( float dt )
	{
		for each( GOC *goc in m_explicits )
		{
			PhysicsProj::Integrate_Explicit( goc ->has(RigidBody), dt );
			goc->has( Transform )->Position = Vector3ToVec3( goc->has(RigidBody)->getPosition() );
		}
		//
		for each( GOC *goc in m_semiExplicits )
		{
			PhysicsProj::Integrate_SemiImplicit( goc ->has(RigidBody), dt );
			goc->has( Transform )->Position = Vector3ToVec3( goc->has(RigidBody)->getPosition() );
		}
		//
		for each( GOC *goc in m_verlets )
		{
			PhysicsProj::Integrate_Verlet( goc ->has(RigidBody), dt );
			goc->has( Transform )->Position = Vector3ToVec3( goc->has(RigidBody)->getPosition() );
		}
		//
		for each( GOC *goc in m_rk4s )
		{
			PhysicsProj::Integrate_RK4( goc ->has(RigidBody), dt );
			goc->has( Transform )->Position = Vector3ToVec3( goc->has(RigidBody)->getPosition() );
		}
	}
	int Level_3::draw(void)
	{
		return 1;
	}
	int Level_3::free(void)
	{
		std::cout<<"Free Level: 3\n";
		m_loader->freeLogicData();
		FACTORY->DestroyAllObjects();
		GRAPHICS->Unload();
		return 1;
	}
	int Level_3::unload(void)
	{
		std::cout<<"Unload Level: 3\n";
		return 1;
	}
	int Level_3::id(void)
	{
		return 3;
	}
	int Level_3::restart(void)
	{
		std::cout<<"Restart Level: 3\n";
		CORE->gsm()->next(GSM::LEVEL_RESTART);
		return 1;
	}
}