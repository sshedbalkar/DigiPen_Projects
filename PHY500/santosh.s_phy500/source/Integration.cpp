#include "Integration.h"
//
namespace PhysicsProj
{
	Framework::Vector3 calculateForce( Framework::Vector3 pos, Framework::real mass )
	{
		Framework::real sun			= 3000;
		Framework::real G			= 1;
		Framework::Vector3 r		= pos;
		Framework::real forceMag	= G*sun*mass;
		//
		forceMag /= ( r*r );
		r.normalise();
		Framework::Vector3 force = r*( -forceMag );
		//
		return force;
	}
	//
	void Integrate_Explicit( Framework::RigidBody *body, float dt )
	{
		Framework::real damp = 0.99;
		body->position.addScaledVector( body->velocity, dt );
		body->forceAccum = calculateForce(body->getPosition(), body->getMass() );
		body->linearDamping = damp;
		//
		Framework::Vector3 acc;
		acc.addScaledVector( body->forceAccum, body->inverseMass );
		//
		body->velocity.addScaledVector( acc, dt );
		body->velocity *= real_pow( (float)body->linearDamping, dt );
		//
		body->clearAccumulators();
	}
	//
	void Integrate_SemiImplicit( Framework::RigidBody *body, float dt )
	{
		Framework::real damp = 0.99;
		body->forceAccum = calculateForce( body->getPosition(), body->getMass() );
		body->linearDamping = damp;
		//
		Framework::Vector3 acc;
		acc.addScaledVector(body->forceAccum, body->inverseMass );
		//
		body->velocity.addScaledVector( acc, dt );
		body->velocity *= real_pow( (float)body->linearDamping, dt );
		body->position.addScaledVector( body->velocity, dt );
		//
		body->clearAccumulators();
	}
	//
	void Integrate_Verlet( Framework::RigidBody *body, float dt )
	{
		Framework::real damp = 0.99;
		Framework::Vector3 currentpos = body->position;
		dt = ( 1.0f/60.0f );
		//
		if( body->firstTime )
		{
			Integrate_SemiImplicit( body, dt );
			body->previousPosition = currentpos;
			body->firstTime = false;
			return;
		}
		//
		body->forceAccum = calculateForce( body->getPosition(), body->getMass() );
		body->linearDamping = damp;
		//
		Framework::Vector3 acc;
		acc.addScaledVector( body->forceAccum, body->inverseMass );
		//
		body->position = currentpos*2.0 - body->previousPosition + acc*( dt*dt );
		body->previousPosition = currentpos;
		//
		body->clearAccumulators();
	}
	//
	void Integrate_RK4( Framework::RigidBody *body, float dt )
	{
		float damp = 0.99f;
		Framework::Vector3 x[4], v[4], a[4];
		Framework::Vector3 f;
		Framework::real m = body->getMass();
		Framework::real im = body->inverseMass;
		//
		x[0] = body->position;
		v[0] = body->velocity;
		f = calculateForce( x[0], m );
		a[0] = f*im;
		//
		x[1] = x[0] + v[0]*( dt*0.5 );
		v[1] = v[0] + a[0]*( dt*0.5 );
		f = calculateForce( x[1], m );
		a[1] = f*im;
		//
		x[2] = x[0] + v[1]*( dt*0.5 );
		v[2] = v[0] + a[1]*( dt*0.5 );
		f = calculateForce( x[2], m );
		a[2] = f*im;
		//
		x[3] = x[0] + v[2]*dt;
		v[3] = v[0] + a[2]*dt;
		f = calculateForce( x[3], m );
		a[3] = f*im;
		//
		body->position = x[0] + ( v[0] + v[1]*2.0 + v[2]*2.0 + v[3] )*( dt/6.0f );
		body->velocity = v[0] + ( a[0] + a[1]*2.0 + a[2]*2.0 + a[3] )*( dt/6.0f );
		body->velocity *= real_pow( damp, dt );
		//
		body->clearAccumulators();
	}
}