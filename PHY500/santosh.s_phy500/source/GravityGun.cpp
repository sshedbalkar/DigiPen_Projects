#include "GravityGun.h"
#include "Physics.h"
#include "Hero.h"
#include "cameraclass.h"

namespace Framework
{

	void GravityGun::AddPickedItem(GOC* p)
	{
		m_PickedItem = p;
		p->has(RigidBody)->setAcceleration(0.0, 0.0, 0.0f);
		p->has(RigidBody)->canRotate=false;
		orientation = p->has(RigidBody)->getOrientation();
		dirAtPickTime = CAMERA->movingDirection;
	}

	void GravityGun::RemovePickedItem()
	{
		if(m_PickedItem)
		{
			m_PickedItem->has(RigidBody)->setAcceleration(m_PickedItem->has(RigidBody)->initialAcceleration);
			m_PickedItem->has(RigidBody)->canRotate=true;
			m_PickedItem->has(RigidBody)->clearAccumulators();
			m_PickedItem->has(RigidBody)->setVelocity(0,0,0);
			m_PickedItem->has(RigidBody)->setRotation(0,0,0);
			D3DXQUATERNION q=m_PickedItem->has(Transform)->QUATERNION;
			m_PickedItem->has(RigidBody)->setOrientation(q.w,q.x,q.y,q.z);
			m_PickedItem = NULL;
		}
	}		

	void GravityGun::Update()
	{
		if (m_PickedItem)
		{
			Vector3 a = HERO->heroPos + HERO->heroDir*3.0,b;
			b=a-m_PickedItem->has(RigidBody)->getPosition();
			real magnitude = b.magnitude();
			b.normalise();
			m_PickedItem->has(RigidBody)->setVelocity(0,0,0);
			//if(magnitude>0.01)
				m_PickedItem->has(RigidBody)->addForce(b*magnitude*600);
			//else
				//m_PickedItem->has(RigidBody)->setVelocity(HERO->m_hero->has(RigidBody)->getVelocity());


			//direction calculation
			if(dirAtPickTime != CAMERA->movingDirection)
			{
				real theta = dirAtPickTime*CAMERA->movingDirection;
				theta = acos(theta);
				Vector3 a= dirAtPickTime%Vector3(CAMERA->movingDirection);
				D3DXQUATERNION q; 
				if(a.y<0)
					q =calculateQuaterion(-(float)theta,Vec3(0,1,0));
				else
					q =calculateQuaterion((float)theta,Vec3(0,1,0));
				m_PickedItem->has(Transform)->QUATERNION*=q; 
			}
		}
	}
	
}