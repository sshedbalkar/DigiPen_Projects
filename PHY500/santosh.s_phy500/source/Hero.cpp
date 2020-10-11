#include "Precompiled.h"
#include "Hero.h"
#include "Physics.h"
#include "ModelComponent.h"
#include "PhysicsMath.h"
#include "VMath.h"
#include "WindowsSystem.h"
#include "cameraclass.h"
#include "Graphics.h"
#include "GameLogic.h"


namespace Framework
{
	Hero* HERO = NULL;

	Hero::Hero()
	{
		m_hero=NULL;
		m_fowardSpeed = 12.0f;
		m_backwardSpeed = 12.0f;
		m_SideMoveSpeed = 10.0f;
		m_jumpSpeed = 5.0f;
		m_allowSpace=true;
		m_pickDistanceFromSurface=3.0f;
		m_pickDistanceFromCenter=3.0f;
		ErrorIf(HERO!=NULL,"HERO already initialized");
		HERO = this;
	}


	void Hero::SendMessage(Message *m)
	{
		if(!m_hero)
			return;
		switch( m->MessageId )
		{

		case Mid::CharacterKey:
			{
				MessageCharacterKey * key = (MessageCharacterKey*)m;
				if(key->character==VK_SPACE)
				{
					if(m_hero->has(RigidBody)->getAcceleration().y==0.0 && m_allowSpace)
					{
						Vector3 temp=m_hero->has(RigidBody)->getVelocity();
						temp.y = m_jumpSpeed;
						m_hero->has(RigidBody)->setVelocity (temp);
						m_allowSpace=false;
					}
				}
				break;
			}
		case Mid::VirtualKey:
			{
				MessageVirtualKey* key = (MessageVirtualKey*)m;
				
				break;
			}
		case Mid::CharacterKeyRelease:
			{
				MessageCharacterKeyRelease* key = (MessageCharacterKeyRelease*)m;
				if(key->character==VK_SPACE)
				{
					m_allowSpace=true;
				}
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
				}
				break;
			}
		case Mid::LMouseButton:
			{
				break;
			}
		}
	}


	void Hero::Update(const float & dt)
	{
		if(m_hero)
		{	
			//initialize every time
			m_velocityMove = Vector3 (0.0,0.0,0.0);
			m_velocitySide = Vector3 (0.0,0.0,0.0);
			Vector3 heroVelocity = m_hero->has(RigidBody)->getVelocity();

			if(heroVelocity.y>m_jumpSpeed)
			    heroVelocity.y=0;

			if (IsWHeld() )      m_velocityMove =  m_fowardSpeed * CAMERA->movingDirection;
			else if (IsSHeld() )	m_velocityMove = -m_fowardSpeed * CAMERA->movingDirection;
			
			if (IsAHeld() )		m_velocitySide= -m_SideMoveSpeed * CAMERA->sideDirection;
			else if (IsDHeld() )	m_velocitySide=  m_SideMoveSpeed * CAMERA->sideDirection;
			
			if ((IsWHeld() && (IsAHeld() || IsDHeld())) || (IsSHeld() && (IsAHeld() || IsDHeld())) )
			{
				Vector3 temp=(m_velocityMove + m_velocitySide)*0.5f;
				temp.y=heroVelocity.y;
				if(collisionNormal.y>=0.5 && m_hero->has(RigidBody)->isColliding)
				{
					real mag=temp.magnitude();
					temp.normalise();
					Vector3 a= temp%collisionNormal;
					a= collisionNormal%a;
					a=a*mag;
					temp=a;
				}
				m_hero->has(RigidBody)->setVelocity (temp);
			}
			else if(IsWHeld() || IsAHeld() || IsDHeld() || IsSHeld())
			{
				Vector3 temp=(m_velocityMove + m_velocitySide)*0.5f;
				temp.y = heroVelocity.y;
				if(collisionNormal.y>=0.5 && m_hero->has(RigidBody)->isColliding)
				{
					real mag=temp.magnitude();
					temp.normalise();
					Vector3 a= temp%collisionNormal;
					a= collisionNormal%a;
					a=a*mag;
					temp=a;
				}
				m_hero->has(RigidBody)->setVelocity (temp);
			}else if(!IsWHeld() || !IsAHeld() || !IsDHeld() || !IsSHeld())
			{
				m_hero->has(RigidBody)->setVelocity (heroVelocity);
			}
					

			CAMERA->position = m_hero->has(Transform)->Position;
			CAMERA->position.y += (m_hero->has(Transform)->Scale.y/2.0f+0.5);			
		}
	}
}