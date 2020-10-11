#pragma once
//
#include "Variant.h"
//#include <iostream>
//
/////////////////////////////////////////////////
class PropertyObject
{
public:
	Variant Property;
	//
	template< typename Type >
	PropertyObject( Type *t )
		:Property( t )
	{
		//
	}
	//
	template< typename Type >
	const Type& set( const Type &t )
	{
		*(Property.As< Type* >()) = t;
		return t;
	}
	//
	template< typename Type >
	Type& get()
	{
		return *(Property.As< Type* >());
	}
};
//
template< typename Type >
PropertyObject* BindProperty( Type *prop )
{
	//std::cout<<"Binding Property\n";
	return new PropertyObject( prop );
}