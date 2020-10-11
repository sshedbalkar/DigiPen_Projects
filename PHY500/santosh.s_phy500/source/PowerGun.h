#pragma once //Makes sure this header is only included once
#include <vector>
#include "Message.h"
#include "BoomBoom.h"
#include "Magnet.h"
#include "Connector.h"
#include "GravityGun.h"

namespace Framework
{
	class PowerGun
	{
		//friend class RigidBody;
	public:
		PowerGun(){m_objectPicked=false;}
		~PowerGun(){}
		void Update();	
	    void SendMessage(Message *);
		BoomBoom bombs;	
		Magnet magnet;
		Connector connector;
		GravityGun gravityGun;
	private:
		bool m_objectPicked;
		enum powerState{powerConnect,powerBomb,powerMagnet,gravity, noOfPower} m_state;
		
	};
}