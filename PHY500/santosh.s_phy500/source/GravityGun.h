#pragma once //Makes sure this header is only included once
#include "PhysicsMath.h"
#include <vector>
#include "Composition.h"

namespace Framework
{
	class GravityGun
	{
		//friend class GameLogic;
	public:
		GravityGun(){m_PickedItem=NULL;}
		~GravityGun(){}
		void Update();		
		void AddPickedItem(GOC* i);	
		void RemovePickedItem();
	private:
		GOC * m_PickedItem;
		Quaternion orientation;
		Vector3 dirAtPickTime;
	};
}