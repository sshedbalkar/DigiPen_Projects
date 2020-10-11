#include "ParticleForceGenerator.h"


namespace Framework
{


void ParticleGravity::updateForce(Particle *particle,real dt)
{
	if (!particle->hasFiniteMass()) return;
	particle->addForce(gravity * particle->getMass());
}

void ParticleDrag::updateForce(Particle* particle, real duration)
{
    Vector3 force;
    particle->getVelocity(&force);

    real dragCoeff = force.magnitude();
    dragCoeff = k1 * dragCoeff + k2 * dragCoeff * dragCoeff;

    force.normalise();
    force *= -dragCoeff;
    particle->addForce(force);
}

void ParticleAnchoredSpring::init(Vector3 *anchor, real springConstant,real restLength)
{
    ParticleAnchoredSpring::anchor = anchor;
    ParticleAnchoredSpring::springConstant = springConstant;
    ParticleAnchoredSpring::restLength = restLength;
}

void ParticleAnchoredSpring::updateForce(Particle* particle, real duration)
{
    Vector3 force;
    particle->getPosition(&force);
    force -= *anchor;

	real magnitude = force.magnitude();
    magnitude = (restLength - magnitude) * springConstant;

    force.normalise();
    force *= magnitude;
    particle->addForce(force);
}

void ParticleAnchoredBungee::updateForce(Particle* particle, real duration)
{
    Vector3 force;
    particle->getPosition(&force);
    force -= *anchor;

    real magnitude = force.magnitude();
    if (magnitude < restLength) return;

    magnitude = magnitude - restLength;
    magnitude *= springConstant;

    force.normalise();
    force *= -magnitude;
    particle->addForce(force);
}

void ParticleFakeSpring::updateForce(Particle* particle, real duration)
{
    if (!particle->hasFiniteMass()) return;

    // Calculate the relative position of the particle to the anchor
    Vector3 position;
    particle->getPosition(&position);
    position -= *anchor;

    // Calculate the constants and check they are in bounds.
    real gamma = 0.5f * real_sqrt(4 * springConstant - damping*damping);
    if (gamma == 0.0f) return;
    Vector3 c = position * (damping / (2.0f * gamma)) +
        particle->getVelocity() * (1.0f / gamma);

    // Calculate the target position
    Vector3 target = position * real_cos(gamma * duration) +
        c * real_sin(gamma * duration);
    target *= real_exp(-0.5f * duration * damping);

    // Calculate the resulting acceleration and therefore the force
    Vector3 accel = (target - position) * (1.0f / duration*duration) - particle->getVelocity() * duration;
    particle->addForce(accel * particle->getMass());
}

void ParticleSpring::updateForce(Particle* particle, real duration)
{
    Vector3 force;
    particle->getPosition(&force);
    force -= other->getPosition();

    real magnitude = force.magnitude();
    magnitude = real_abs(magnitude - restLength);
    magnitude *= springConstant;

    force.normalise();
    force *= -magnitude;
    particle->addForce(force);
}

void ParticleBungee::updateForce(Particle* particle, real duration)
{
    Vector3 force;
    particle->getPosition(&force);
    force -= other->getPosition();

    real magnitude = force.magnitude();
    if (magnitude <= restLength) return;

    magnitude = springConstant * (restLength - magnitude);

    force.normalise();
    force *= -magnitude;
    particle->addForce(force);
}

void ParticleBuoyancy::updateForce(Particle* particle, real duration)
{
    real depth = particle->getPosition().y;

    if (depth >= waterHeight + maxDepth) return;
    Vector3 force(0,0,0);

    if (depth <= waterHeight - maxDepth)
    {
        force.y = liquidDensity * volume;
        particle->addForce(force);
        return;
    }

    force.y = liquidDensity*volume*(depth - maxDepth - waterHeight) / 2 * maxDepth;
    particle->addForce(force);
}

void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator *fg)
{
    ParticleForceRegistry::ParticleForceRegistration registration;
    registration.particle = particle;
    registration.fg = fg;
    registrations.push_back(registration);
}

void ParticleForceRegistry::updateForces(real duration)
{
    Registry::iterator i = registrations.begin();
    for (; i != registrations.end(); i++)
    {
        i->fg->updateForce(i->particle, duration);
    }
}

void ParticleForceRegistry::remove(Particle* particle, ParticleForceGenerator *fg)
{
	Registry::iterator i = registrations.begin();
	for (; i != registrations.end(); i++)
    {
        if(i->fg==fg && i->particle==particle) break;
    }
	registrations.erase(i);
}

}