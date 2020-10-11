#include "RigidBody.h"
#include <vector>
#include <list>


#ifndef FORCEGENERATOR_H
#define FORCEGENERATOR_H


namespace Framework
{


class ForceGenerator
{
public:
	virtual void updateForce(RigidBody *body, real duration) = 0;
};

//sample force generators
class Gravity : public ForceGenerator
{
	Vector3 gravity;
public:
	Gravity(const Vector3 &g):gravity(g){}
	virtual void updateForce(RigidBody *body, real duration);
};

class Spring : public ForceGenerator
{
	//both in local coordinates
    RigidBody *other;
	real springConstant;
	real restLength;
public:
	Spring(RigidBody *other,real springConstant, real restLength):
	  other(other), springConstant(springConstant), restLength(restLength){}
	virtual void updateForce(RigidBody *body, real duration);
};

class ForceRegistry
{
protected:
	struct ForceRegistration
    {
        RigidBody *body;
        ForceGenerator *fg;
    };

    typedef std::list<ForceRegistration> Registry;
    Registry registrations;

public:
    void add(RigidBody* body, ForceGenerator *fg);
	void remove(RigidBody* body, ForceGenerator *fg);

	void clear(){registrations.clear();}
	void updateForces(real dt);
};


}
#endif