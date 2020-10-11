#ifndef LEVEL_3_H
#define LEVEL_3_H
//
#include "ILevel.h"
#include "LoadLevelClass.h"
#include <vector>
//
namespace Framework
{
	class GameObjectComposition;
	class RigidBody;
	class Level_3: public ILevel
	{
	public:
		Level_3();
		~Level_3();
		//
		int load(void);
		int initialize(void);
		int update(float dt);
		int draw(void);
		int free(void);
		int unload(void);
		int id(void);
		int restart(void);
		int sendMessage( Message *m );
		//
	private:
		float m_loadingEffectTimer;
		bool m_loadingEffectDone;
		LoadLevelClass *m_loader;
		void AddObjExplicit();
		void AddObjSemiexplicit();
		void AddObjVerlet();
		void AddObjRK4();
		std::vector< GameObjectComposition* > m_explicits;
		std::vector< GameObjectComposition* > m_semiExplicits;
		std::vector< GameObjectComposition* > m_verlets;
		std::vector< GameObjectComposition* > m_rk4s;
		void Integrate( float dt );
	};
}
//
#endif