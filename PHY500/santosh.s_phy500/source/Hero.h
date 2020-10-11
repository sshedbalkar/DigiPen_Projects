#pragma once //Makes sure this header is only included once
#include "Composition.h"

namespace Framework
{
	class Hero
	{
		friend class LoadLevelClass;
		friend class GravityGun;
		friend class EditorFunctionality;
	public:
		Hero();
		~Hero(){}
		void Update(const float &);	
		void SendMessage(Message *);
		Vector3 heroPos,heroDir,collisionNormal;
	private:
		GOC * m_hero;
		float m_pickDistanceFromSurface;
		float m_pickDistanceFromCenter;
		Vector3 m_velocityMove;
		Vector3 m_velocitySide;
		float m_fowardSpeed;
		float m_backwardSpeed;
		float m_SideMoveSpeed,m_jumpSpeed;
		bool m_allowSpace;
	};
	extern Hero* HERO;
}