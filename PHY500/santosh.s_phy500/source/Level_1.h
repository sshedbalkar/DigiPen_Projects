#ifndef LEVEL_1_H
#define LEVEL_1_H
//
#include "ILevel.h"
#include "LoadLevelClass.h"
#include <vector>
//
namespace Framework
{
	class GameObjectComposition;
	class RigidBody;
	class Level_1: public ILevel
	{
	public:
		Level_1();
		~Level_1();
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
		std::vector<GameObjectComposition*> m_gocs;
		void ( *Integrator )( RigidBody*, float );
		void Integrate( float dt );
		char int_name[32];
	};
}
//
#endif