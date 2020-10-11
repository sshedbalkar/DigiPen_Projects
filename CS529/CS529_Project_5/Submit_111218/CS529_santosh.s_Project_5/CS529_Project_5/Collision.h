///////////////////////////////////////////////////////////////////////////////////////
///
///	\file Collision.h
///	Provides shapes that are used by Body Component for collision detection.
///	
///	Authors:  Chris Peters
///	Copyright 2010, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////////////
#ifndef COLLISION_H
#define COLLISION_H
//#pragma once //Makes sure this header is only included once
//
#include "VMath.h"
#include "Engine.h"
#include "Shape.h"
//
namespace Wye
{
	class Body;
	
	///Data for a contact between two bodies.
	///Used to resolve world collisions.
	struct BodyContact
	{
#pragma message("Including class: BodyContact")
		Body* Bodies[2];
		Vec3 Movement[2];
		Vec3 ContactNormal;
		float Penetration;
		float Restitution;
		float FrictionCof;

		float SeperatingVelocity;
		float ContactImpulse;
		float calculateSeparatingVelocity();
	};

	class ContactSet;
	typedef bool (*CollisionTest)(Shape* a, Vec3 at, Shape* b, Vec3 bt, ContactSet* c);

	///The collision database provides collision detection between shape types.
	class CollsionDatabase
	{
	public:
		CollsionDatabase();
		CollisionTest CollsionRegistry[Shape::SidNumberOfShapes][Shape::SidNumberOfShapes];
		bool generateContacts(Shape* shapeA, Vec3 poistionA, Shape* shapeB, Vec3 poistionB, ContactSet* c);
		void registerCollsionTest(Shape::ShapeId a, Shape::ShapeId b, CollisionTest test);
	};
}
//
#endif