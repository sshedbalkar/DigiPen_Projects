#include <cassert>
#include "boolean.variable.h"

#ifndef INLINE_BOOLEAN_VARIABLE
	#include "boolean.variable.inl"
#endif

Bool::Bool (BoolEnum b) : value ( b ) {}
Bool::Bool (bool b) : value ( b?TRUE:FALSE) {}
Bool::Bool (int b) : value ( b==1?TRUE:(b==0?FALSE:UNDEF)) {}
	
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
BoolVariable::BoolVariable(const std::string & name) 
	: value(UNDEF),name(name),is_assigned(false) 
{
	available_values.insert(true);
	available_values.insert(false);
}

////////////////////////////////////////////////////////////
void BoolVariable::Print () const {
	Print( std::cout );
}

////////////////////////////////////////////////////////////
void BoolVariable::Print (std::ostream& os) const {
	std::set<BoolVariable::Value>::const_iterator b = GetDomain().begin();
	std::set<BoolVariable::Value>::const_iterator e = GetDomain().end();
	os << "Variable \"" << Name() << "\" available values: ";
	for ( ;b!=e;++b) { os << *b << " "; }
	os << std::endl;
	if ( IsAssigned() ) 
		os << " assigned value " << GetValue() << std::endl;
}

////////////////////////////////////////////////////////////
const BoolVariable* BoolVariable::clone() const { return this; }

////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const BoolVariable& v) {
	os << v.Name();
	return os;
}

