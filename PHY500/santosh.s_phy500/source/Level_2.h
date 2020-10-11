#ifndef LEVEL_2_H
#define LEVEL_2_H
//
#include "ILevel.h"
#include "LoadLevelClass.h"
//
namespace Framework
{
	class GameObjectComposition;
	class RigidBody;
	class Level_2: public ILevel
	{
	public:
		Level_2();
		~Level_2();
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
		GameObjectComposition *goc_explicit;
		GameObjectComposition *goc_semiExplicit;
		GameObjectComposition *goc_verlet;
		GameObjectComposition *goc_rk4;
		void Integrate( float dt );
	};
}
//
#endif