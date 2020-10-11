///////////////////////////////////////////////////////////////////////////////////////
//
//	Collsion.cpp
//
//	Authors:  Chris Peters
//	Copyright 2010, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////
#include "Precompiled.h"
#include "Collision.h"
#include "Physics.h"
#include "DebugDraw.h"
#include "DebugTools.h"
//
namespace Wye
{

	//Functions that determine how restitution and friction 
	//are determined between body pairs.

	//There really is no physically accurate way to mathematically combine 
	//these factors. A material does not have a single friction or restitution.
	//The physically accurate way is to have a large database that
	//define the relationship between all physical materials or simulate 
	//molecular physics.

	float determineRestitution(Body* a, Body* b)
	{
		return std::min(a->Restitution, b->Restitution);
	}

	float determineFriction(Body* a, Body* b)
	{
		return sqrt(a->Friction * b->Friction);
	}

	/////////////////////Collsion Detection Functions////////////////////

	bool detectCollisionCircleCircle(Shape* a, Vec3 at, Shape* b, Vec3 bt, ContactSet* c)
	{
		ShapeCircle* spA = (ShapeCircle*)a;
		ShapeCircle* spB = (ShapeCircle*)b;

		Vec3 positionDelta = at - bt;
		float disSquared = lengthSquared(positionDelta);
		float combinedRadius = spA->Radius + spB->Radius;

		//Is the squared distance between the two sphere less
		//then the squared radius?
		if(disSquared < combinedRadius * combinedRadius)
		{
			//Get not squared values

			Vec3 normal;
			float penetration;
			if(disSquared == 0.0f)
			{
				//Right on top of each other
				penetration = spA->Radius;
				normal = Vec3(1, 0, 0);
			}
			else
			{
				float dis = normalize(positionDelta);
				penetration = combinedRadius - dis;
				normal = positionDelta;
			}

			//Add a contact
			BodyContact* contact = c->getNextContact();
			contact->Bodies[0] = spA->body;
			contact->Bodies[1] = spB->body;
			contact->ContactNormal = positionDelta;//A to B
			contact->Penetration = penetration;
			contact->Restitution = determineRestitution(a->body, b->body);
			contact->FrictionCof = determineFriction(a->body, b->body);
			return true;
		}
		else
		{
			return false;
		}
	}

	bool detectCollisionCircleAABox(Shape* a, Vec3 at, Shape* b, Vec3 bt, ContactSet* c)
	{
		ShapeCircle* spA = (ShapeCircle*)a;
		ShapeAAB* boxB = (ShapeAAB*)b;

		float radius = spA->Radius;
		Vec3 p = at;
		Vec3 minV(bt.x - boxB->Extents.x, bt.y - boxB->Extents.y, bt.z - boxB->Extents.z);
		Vec3 maxV(bt.x + boxB->Extents.x, bt.y + boxB->Extents.y, bt.z + boxB->Extents.z);
		
		Vec3 q;
		for (int i = 0; i < 3; ++i)
		{
			float v = p[i];
			if (v < minV[i]) v = minV[i];
			if (v > maxV[i]) v = maxV[i];
			q[i] = v;
		}

		Vec3 delta = p - q;
		float disSq = lengthSquared(delta);
		if(disSq < radius * radius)
		{
			if(disSq == 0.0f)
			{
				//internal
				Vec3 bdelta = at - bt;

				float xd = radius + boxB->Extents.x - fabs(bdelta.x);
				float yd = radius + boxB->Extents.y - fabs(bdelta.y);
				float zd = radius + boxB->Extents.z - fabs(bdelta.z);

				Vec3 normal;
				float penetration;

				if(xd < yd && xd < zd)
				{
					normal = bdelta.x < 0 ? Vec3(-1, 0, 0) : Vec3(1, 0, 0);
					penetration = xd;
				}
				else if(yd < xd && yd < zd)
				{
					normal = bdelta.y < 0 ? Vec3(0, -1, 0) : Vec3(0, 1, 0);
					penetration = yd;
				}
				else
				{
					normal = bdelta.z < 0 ? Vec3(0, 0, -1) : Vec3(0, 0, 1);
					penetration = zd;
				}

				BodyContact* contact = c->getNextContact();
				contact->Bodies[0] = spA->body;
				contact->Bodies[1] = boxB->body;
				contact->ContactNormal = normal;
				contact->Penetration = penetration;
				contact->Restitution = determineRestitution(a->body, b->body);
				contact->FrictionCof = determineFriction(a->body, b->body);
			}
			else
			{
				float dis = normalize(delta);
				BodyContact* contact = c->getNextContact();
				contact->Bodies[0] = spA->body;
				contact->Bodies[1] = boxB->body;
				contact->ContactNormal = delta;
				contact->Penetration = -(dis - spA->Radius);
				contact->Restitution = determineRestitution(a->body, b->body);
				contact->FrictionCof = determineFriction(a->body, b->body);
			}
		}
		return false;
	}

	bool detectCollisionAABoxAABox(Shape* a, Vec3 at, Shape* b, Vec3 bt, ContactSet* c)
	{
		ShapeAAB* boxA = (ShapeAAB*)a;
		ShapeAAB* boxB = (ShapeAAB*)b;

		//Check X
		Vec3 positionDelta = at - bt;
		float xDiff = boxA->Extents.x + boxB->Extents.x - fabs(positionDelta.x);

		//Boxes overlapping on x-axis?
		if(0 < xDiff)
		{
			float yDiff = boxA->Extents.y + boxB->Extents.y - fabs(positionDelta.y);
			
			//Boxes overlapping on y-axis?
			if(0 < yDiff)
			{
				float zDiff = boxA->Extents.z + boxB->Extents.z - fabs(positionDelta.z);
				if(0 < zDiff)
				{
					//std::cout<<"Contact: "<<boxA->body->getId()<<", "<<boxB->body->getId()<<"\n";
					//Which axis is overlapping less? that is the axis we want
					//to use for the collision.
					if(xDiff < yDiff && xDiff < zDiff)
					{
						//X is smallest
						BodyContact* contact = c->getNextContact();
						Vec3 normal = positionDelta.x < 0 ? Vec3(-1, 0, 0) : Vec3(1, 0, 0);
						contact->Bodies[0] = boxA->body;
						contact->Bodies[1] = boxB->body;
						contact->ContactNormal = normal;
						contact->Penetration = xDiff;
						contact->Restitution = determineRestitution(a->body, b->body);
						contact->FrictionCof = determineFriction(a->body, b->body);
						return true;
					}
					else if(yDiff < xDiff && yDiff < zDiff)
					{
						//Y is smallest
						BodyContact* contact = c->getNextContact();
						Vec3 normal = positionDelta.y < 0 ? Vec3(0, 1, 0) : Vec3(0,-1, 0);
						contact->Bodies[1] = boxA->body;
						contact->Bodies[0] = boxB->body;
						contact->ContactNormal = normal;
						contact->Penetration = yDiff;
						contact->Restitution = determineRestitution(a->body, b->body);
						contact->FrictionCof = determineFriction(a->body, b->body);
						return true;
					}
					else
					{
						//Z is smallest
						BodyContact* contact = c->getNextContact();
						Vec3 normal = positionDelta.z < 0 ? Vec3(0, 0, -1) : Vec3(0, 0, 1);
						contact->Bodies[1] = boxA->body;
						contact->Bodies[0] = boxB->body;
						contact->ContactNormal = normal;
						contact->Penetration = yDiff;
						contact->Restitution = determineRestitution(a->body, b->body);
						contact->FrictionCof = determineFriction(a->body, b->body);
						return true;
					}
				}
			}
		}
		return false;
	}


	//Auxiliary
	bool detectCollisionBoxCircle(Shape*a, Vec3 at, Shape* b, Vec3 bt, ContactSet* c)
	{
		return detectCollisionCircleAABox(b, bt, a, at, c);
	}

	float BodyContact::calculateSeparatingVelocity()
	{
		Vec3 relativeVelocity = Bodies[0]->Velocity - Bodies[1]->Velocity;

		//Get the separating velocity by projecting along the contact normal
		SeperatingVelocity = dot(relativeVelocity, ContactNormal);

		return SeperatingVelocity;
	}

	CollsionDatabase::CollsionDatabase()
	{
		//Register collision tests for all the shape types
		registerCollsionTest(Shape::Sid_Circle, Shape::Sid_Circle, detectCollisionCircleCircle);
		registerCollsionTest(Shape::Sid_Box, Shape::Sid_Box, detectCollisionAABoxAABox);
		registerCollsionTest(Shape::Sid_Circle, Shape::Sid_Box, detectCollisionCircleAABox);
		registerCollsionTest(Shape::Sid_Box, Shape::Sid_Circle, detectCollisionBoxCircle);
	}

	void CollsionDatabase::registerCollsionTest(Shape::ShapeId a, Shape::ShapeId b, CollisionTest test)
	{
		CollsionRegistry[a][b] = test;
	}

	bool CollsionDatabase::generateContacts(Shape* shapeA, Vec3 poistionA, Shape* shapeB, Vec3 poistionB, ContactSet* c)
	{
		return (*CollsionRegistry[shapeA->Id][shapeB->Id])(shapeA, poistionA, shapeB, poistionB, c);
	}
}