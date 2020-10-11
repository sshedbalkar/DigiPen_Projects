#pragma once
//
#include <string>
#include <map>
#include "FunctionBind.h"
#include "PropertyBind.h"
//
class MetaType
{
public:
	std::map< std::string, FunctionObject* > Functions;
};
//
template< typename type >
void BindMeta()
{
	MetaType* metaType = new MetaType();
	type::BindMeta( metaType );
	type::Meta = metaType;
}

//
class MetaClass
{
public:
	typedef MetaClass this_type;
	static MetaType* Meta;
	static void BindMeta( MetaType* type );
};