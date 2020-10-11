////////////////////////////////////////////////////////////////
//
//	XMLSerialization.h
//	implements the Serialization interface for the XML serialization. Uses external library "rapidxml.hpp" for parsing xml
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef XML_SERIALIZATION_H
#define XML_SERIALIZATION_H
//#pragma message("Compiling: "__FILE__)
//
#include "rapidxml.hpp"
#include "Serialization.h"
//#include "Utility.h"
//
namespace Wye
{
	///Serializer for a simple text file format.
	class XMLSerializer: public ISerializer
	{
#pragma message("Including Class: XMLSerializer")
	public:
		friend class XMLReader;
		XMLSerializer();
		virtual ~XMLSerializer();
		bool readProperty(const char* prop, int& i) const;
		bool readProperty(const char* prop, float& f) const;
		bool readProperty(const char* prop, std::string& str) const;
		bool readProperty(const char* prop, ISerializer& stream);
		bool readProperty(const char* prop, std::vector<ISerializer*>& vec) const;
		bool readProperty(const char* prop, Vec2& vec) const;
		bool readProperty(const char* prop, Vec3& vec) const;
		bool readProperty(const char* prop, Vec4& vec) const;
		ISerializer* clone();
		ISerializer* clone() const;
		bool isGood() const;
		char* name();
	protected:
		rapidxml::xml_node<>* data();
		void data(rapidxml::xml_node<>* xml);
	private:
		XMLSerializer(rapidxml::xml_node<>* xml);
		rapidxml::xml_node<>* _xml;
	};
	//
	class XMLReader: public Object
	{
	public:
		XMLReader();
		virtual ~XMLReader();
		bool open(const char* file);
		XMLSerializer data();
		//
	private:
		rapidxml::xml_document<> _doc;
		char* _buffer;
	};
}
//
#endif