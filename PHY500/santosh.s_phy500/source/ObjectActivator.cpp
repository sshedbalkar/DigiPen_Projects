#include "Precompiled.h"
#include "ObjectActivator.h"
#include "GameLogic.h"
#include "Physics.h"
#include "ActivationProperties.h"

namespace Framework
{

	void ObjectActivator::Update(float dt)
	{
		std::vector<GOC*>::iterator  it;
		
		for(it=LOGIC->m_ActivatableObjectsList.begin();it!=LOGIC->m_ActivatableObjectsList.end();++it)
		{

			if((*it)->has(RigidBody)->isColliding)
			{
				int id = (*it)->has(ActivationProperties)->GetActivationId();			//#### added temporary code
				/*if(m_buttons[id]==0 )
					AUDIO->Play("button");*/
				m_buttons[id]=1;
			}
			else
			{
				int id = (*it)->has(ActivationProperties)->GetActivationId();			//#### added temporary code
				m_buttons[id]=0;
			}
			(*it)->has(ActivationProperties)->Update (dt);
		}

		Vec4 temp;
		for(it=LOGIC->m_ActivatableObjectsList.begin();it!=LOGIC->m_ActivatableObjectsList.end();++it)
		{
			bool activated=false;
			temp=(*it)->has(ActivationProperties)->GetObjectActivatedByVec();
			if(m_buttons[(int)temp.x]==1)
				if(m_buttons[(int)temp.y]==1)
					if(m_buttons[(int)temp.z]==1)
						if(m_buttons[(int)temp.w]==1)
							activated=true;
							
			(*it)->has(ActivationProperties)->MarkAsActivated(activated);				
			(*it)->has(ActivationProperties)->Update (dt);
		}
	}

}
