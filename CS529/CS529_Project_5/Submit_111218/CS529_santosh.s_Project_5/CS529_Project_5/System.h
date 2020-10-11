////////////////////////////////////////////////////////////////
//
//	System.h
//	Defines a system which is updated periodically
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef SYSTEM_H
#define SYSTEM_H
//#pragma message("Compiling: "__FILE__)
//
#include "Composition.h"
//
namespace Wye
{
	class System: public Composition
	{
#pragma message("Including Class: System")
	public:
		System();
		virtual ~System();
		//
		///All systems are updated every game frame.
		virtual void update(float timeslice);
		//
		///All systems provide a string name for debugging.
	private:
		System(const System& orig);
		System& operator=(const System& rhs);
	};
}
//
#endif