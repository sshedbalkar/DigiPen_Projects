#include "Precompiled.h"
#include "ActivationProperties.h"
#include <algorithm>
#include <vector>
#include "GameLogic.h"
#include "Audio.h"

namespace Framework
{

	ActivationProperties::ActivationProperties()
	{
		m_ResetOnLoad=0;
		IsActivated=false;
		doorVelocity=2;
	}

	ActivationProperties::~ActivationProperties()
	{
	}


	void ActivationProperties::Initialize()
	{ 
		m_owner = this->GetOwner();
		m_Body = this->GetOwner()->has(RigidBody);
		m_Transform = this->GetOwner()->has(Transform);
		m_InitialPos = m_Body->getPosition();
	}



	void ActivationProperties::Activate()
	{
	}

	void ActivationProperties::ButtonAction()
	{
		/*if(!(m_Body->isColliding))
			{
				if(abs(m_Body->getPosition().y-m_Body->getInitialPosition.y)>EPSILON)
				{
					m_Body->Position.y=m_Body->InitialPosition.y-(abs(m_Body->Position.y-m_Body->InitialPosition.y)*0.8f);
					
				}
		}*/
	}
	void ActivationProperties::DoorAction(float dt)
	{
		if (IsActivated)
		{
			Vector3 vec=m_Body->getPosition ();
			vec.y+=dt*doorVelocity;
			if  (vec.y > m_InitialPos.y + m_Transform->Scale.y)
			{
				vec.y=m_InitialPos.y + m_Transform->Scale.y;
				m_Body->setPosition(vec);
			}
			else
				m_Body->setPosition(vec);
		}
		else
		{
			Vector3 vec=m_Body->getPosition ();
			vec.y-=dt*doorVelocity;
			if (vec.y<=m_InitialPos.y)
				m_Body->setPosition(m_InitialPos);
			else
				m_Body->setPosition(vec);
		}
	}



	void ActivationProperties::Update(const float & dt)
	{
		static_cast<void>(dt);
		if (m_owner->CompositionTypeId ==GOC_Type_Button1)
		{
			ButtonAction();
		}
		else if (m_owner->CompositionTypeId ==GOC_Type_Door1)
		{
			DoorAction(dt);
		}
	}

	void ActivationProperties::SendMessageA(Message *m)
	{
		if(m->MessageId== Mid::Collide)
		{
		}
	}


	void ActivationProperties::Serialize(ISerializer& stream)
	{	
		ISerializer* str = stream.clone();
		if(str->isGood())
		{
			str->readProperty("ObjectActivationId", *str);
			str->readValue("value", m_ActivationId);
			//
			str->goToParent();
			str->readProperty("ResetOnLoad", *str);
			str->readValue("value", m_ResetOnLoad);
			//
			str->goToParent();
			str->readProperty("ActivatedBy", *str);
			str->readValue("value", m_ActivatedByVec,4);
			
		}
		delete str;
	}

}
