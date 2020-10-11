#include "PowerGun.h"
#include "WindowsSystem.h"
#include "GameLogic.h"
#include "Physics.h"

namespace Framework
{

	void PowerGun::SendMessage(Message *m)
	{
		switch( m->MessageId )
		{

		case Mid::Collide:
		{
			MessageCollide * msg = (MessageCollide*)m;

			if(magnet.a == msg->CollideObject && magnet.b == msg->CollidedWith )
			{
				/*magnet.b->has(RigidBody)->setVelocity(0,0,0);
				magnet.a->has(RigidBody)->setVelocity(0,0,0);*/
				magnet.a->has(RigidBody)->setRotation(0,0,0);
				magnet.b->has(RigidBody)->setRotation(0,0,0);
				magnet.b->has(RigidBody)->setVelocity(magnet.b->has(RigidBody)->getVelocity()*0.1);
				magnet.a->has(RigidBody)->setVelocity(magnet.a->has(RigidBody)->getVelocity()*0.1);

				magnet.a=magnet.b=0;
			}
			break;
		}				

		case Mid::CharacterKey:
			{
				MessageCharacterKey * key = (MessageCharacterKey*)m;
				if(key->character=='1')
				{
					m_state=powerConnect;
				}
				else if(key->character=='2')
				{
					/*m_state=powerBomb;
					Bomb b;
					b.pos=Vector3(-9,-17,-120);
					b.distance=200;
					b.force=40000;
					bombs.AddBomb(b);*/
				}
				else if(key->character=='3')
				{
					m_state=powerMagnet;
				}
				else if(key->character=='e')
				{
					m_state=gravity;
					if(LOGIC->objectPointedAt && !m_objectPicked)
					{
						if (LOGIC->objectPointedAt->CompositionTypeId==GOC_Type_CUBE || LOGIC->objectPointedAt->CompositionTypeId==GOC_Type_SPHERE)
						{
							gravityGun.AddPickedItem(LOGIC->objectPointedAt);
							m_objectPicked=true;
						}
					}
					else
					{
						m_objectPicked=false;
						gravityGun.RemovePickedItem();
					}
				}
				break;
			}
		case Mid::VirtualKey:
			{
				break;
			}
		case Mid::MouseMove:
			{
				break;
			}
		case Mid::MouseWheel :
			{
				break;
			}
		case Mid::RMouseButton:
			{
				RMouseButton * rmouse = (RMouseButton*)m;
				if(rmouse->ButtonIsPressed)
				{
					if(m_state==powerMagnet)
						magnet.AddBody2(LOGIC->objectPointedAt);	
					else if (m_state==powerConnect)
						connector.AddBody2(LOGIC->objectPointedAt);	
				}
				else
				{
					
				}
				break;
			}
		case Mid::LMouseButton:
			{
				LMouseButton * lmouse = (LMouseButton*)m;
				if(lmouse->ButtonIsPressed)
				{
					if(m_state==powerMagnet)
						magnet.AddBody1(LOGIC->objectPointedAt);	
					else if (m_state==powerConnect)
						connector.AddBody1(LOGIC->objectPointedAt);	
				}
				else
				{
					
				}
				break;
			}
		}
	}


	void PowerGun::Update()
	{
		magnet.Update();
		bombs.Update ();
		connector.Update();
		gravityGun.Update();
	}

}