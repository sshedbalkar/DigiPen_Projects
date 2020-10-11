#include "Particle.h"


namespace Framework
{


void Particle::integrate(real duration)
{
    if (inverseMass <= 0.0f) return;

    position.addScaledVector(velocity, duration);
    
	Vector3 resultingAcc = acceleration;
    resultingAcc.addScaledVector(forceAccum, inverseMass);

    velocity.addScaledVector(resultingAcc, duration);

    // Impose drag.
    velocity *= real_pow(damping, duration);
    clearAccumulator();
}

void Particle::setMass(const real mass)
{
	Particle::mass = mass;
	if(mass>0)
		inverseMass = ((real)1.0)/mass;
	else
		inverseMass = 0.0;
}

real Particle::getMass() const
{
    return mass;
}

void Particle::setInverseMass(const real inverseMass)
{
    Particle::inverseMass = inverseMass;
	if(inverseMass!=0)
		mass=((real)1.0)/inverseMass;
	else
		mass=0.0;
}

real Particle::getInverseMass() const
{
    return inverseMass;
}

bool Particle::hasFiniteMass() const
{
    return inverseMass >= 0.0f;
}

void Particle::setDamping(const real damping)
{
    Particle::damping = damping;
}

real Particle::getDamping() const
{
    return damping;
}

void Particle::setPosition(const Vector3 &position)
{
    Particle::position = position;
}

void Particle::setPosition(const real x, const real y, const real z)
{
    position.x = x;
    position.y = y;
    position.z = z;
}

void Particle::getPosition(Vector3 *position) const
{
    *position = Particle::position;
}

Vector3 Particle::getPosition() const
{
    return position;
}

void Particle::setVelocity(const Vector3 &velocity)
{
    Particle::velocity = velocity;
}

void Particle::setVelocity(const real x, const real y, const real z)
{
    velocity.x = x;
    velocity.y = y;
    velocity.z = z;
}

void Particle::getVelocity(Vector3 *velocity) const
{
    *velocity = Particle::velocity;
}

Vector3 Particle::getVelocity() const
{
    return velocity;
}

void Particle::setAcceleration(const Vector3 &acceleration)
{
    Particle::acceleration = acceleration;
}

void Particle::setAcceleration(const real x, const real y, const real z)
{
    acceleration.x = x;
    acceleration.y = y;
    acceleration.z = z;
}

void Particle::getAcceleration(Vector3 *acceleration) const
{
    *acceleration = Particle::acceleration;
}

Vector3 Particle::getAcceleration() const
{
    return acceleration;
}

void Particle::clearAccumulator()
{
    forceAccum.clear();
}

void Particle::addForce(const Vector3 &force)
{
    forceAccum += force;
}


}