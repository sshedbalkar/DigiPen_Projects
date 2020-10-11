////////////////////////////////////////////////////////////////
//
//	Serialization.h
//	Provides interface for the serialization
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef SERIALIZATION_H
#define SERIALIZATION_H
//#pragma message("Compiling: "__FILE__)
//
#include <string>
#include <vector>
#include "Object.h"
#include "VMath.h"
//
namespace Wye
{
	class ISerializer: public Object
	{
#pragma message("Including Class: ISerializer")
	public:
		virtual bool readProperty(const char* prop, int& i) const = 0;
		virtual bool readProperty(const char* prop, float& f) const = 0;
		virtual bool readProperty(const char* prop, std::string& str) const = 0;
		virtual bool readProperty(const char* prop, ISerializer& stream) = 0;
		virtual bool readProperty(const char* prop, std::vector<ISerializer*>& vec) const = 0;
		virtual bool readProperty(const char* prop, Vec2& vec) const = 0;
		virtual bool readProperty(const char* prop, Vec3& vec) const = 0;
		virtual bool readProperty(const char* prop, Vec4& vec) const = 0;
		virtual ISerializer* clone() = 0;
		virtual ISerializer* clone() const = 0;
		virtual bool isGood() const = 0;
	};

	//Serialization Operators
	//Base case of serialization is that the object serializes itself.
	template<typename type>
	inline void streamRead(ISerializer& stream, type& typeInstance)
	{
		typeInstance.serialize(stream);
	}
}
//
#endif