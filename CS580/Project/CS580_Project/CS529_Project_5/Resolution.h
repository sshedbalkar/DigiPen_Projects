///////////////////////////////////////////////////////////////////////////////////////
///
///	\file Resolution.h
///	Iterative impulse collision resolution system.
///	
///	Authors: Chris Peters
///	Copyright 2010, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Collision.h"

namespace Wye
{
	///A Set of contacts that need to be resolved.
	class ContactSet
	{
//#pragma message("Including Class: ContactSet")
	public:
		BodyContact* getNextContact();
		void resolveContacts(float dt);
		void reset();
	private:
		friend class Physics;
		static const int MaxContacts = 1024;
		BodyContact contactArray[MaxContacts];
		unsigned NumberOfContacts;
		void resolveVelocities(float dt);
		void resolvePositions(float dt);
	};

}