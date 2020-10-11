////////////////////////////////////////////////////////////////
//
//	Object.h
//	Most basic class from which everything is going to derived
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef OBJECT_H
#define OBJECT_H
//#pragma message("Compiling: "__FILE__)
//
//#include "Engine.h"
//
namespace Wye
{
	class Object
	{
#pragma message("Including Class: Object")
	public:
		Object(int ObjectId);
		Object();
		Object(const Object& orig);
		virtual ~Object();
		//
		virtual operator int() const;
		virtual Object& operator =(const Object& rhs);
		//
		virtual int getId() const;
		//
	private:
		int _id;
	};
}
//
#endif