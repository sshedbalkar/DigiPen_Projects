#pragma once;
//
#include "RigidBody.h"
//
namespace PhysicsProj
{
	Framework::Vector3	calculateForce( Framework::Vector3 pos, Framework::real mass );
	void				Integrate_Explicit( Framework::RigidBody *body, float dt );
	void				Integrate_SemiImplicit( Framework::RigidBody *body, float dt );
	void				Integrate_Verlet( Framework::RigidBody *body, float dt );
	void				Integrate_RK4( Framework::RigidBody *body, float dt );
}
