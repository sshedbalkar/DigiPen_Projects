#ifdef INLINE_BOOLEAN_VARIABLE
    #define INLINE inline
#else
    #define INLINE 
#endif
#include <iterator>

INLINE Bool Bool::operator&& (const Bool& rhs) const { 
//     F   T   U 
//  F  0   0   0      F F F 
//  T  0   1   10     F T U
//  U  0   10  100    F U U
	return Bool(value*rhs.value);
}

INLINE Bool Bool::operator|| (const Bool& rhs) const { 
//     F   T   U 
//  F  0   1   10     F T U
//  T  1   1   1      T T T
//  U  10  1   -80    U T U
	return value + rhs.value - value*rhs.value;
}

INLINE Bool Bool::operator! () const { 
	if (value==UNDEF) return UNDEF;
	else 
		return (value==FALSE)?TRUE:FALSE;
}

INLINE bool Bool::operator!= (const BoolEnum& rhs) const { 
	return value != rhs;
}

INLINE bool Bool::operator== (const BoolEnum& rhs) const { 
	return value == rhs;
}

INLINE std::ostream& operator<<(std::ostream& os, const Bool& b) {
	if (b.value==TRUE) os << "TRUE";
	else  {
		if (b.value==FALSE) os << "FALSE";
		else {
			os << "UNDEF";
		}
	}
	return os ;
}


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
INLINE void BoolVariable::RemoveValue(Value val) {
	std::set<Value>::iterator it = available_values.find(val);
	if ( it != available_values.end() ) {
		available_values.erase(it);
	}
}
////////////////////////////////////////////////////////////
INLINE const std::string & BoolVariable::Name() const { return name; }

////////////////////////////////////////////////////////////
INLINE int BoolVariable::SizeDomain() const { return available_values.size(); }

////////////////////////////////////////////////////////////
INLINE void BoolVariable::SetDomain(const std::set<Value>& vals) { 
	available_values.clear();
	std::copy(vals.begin(),vals.end(),
			std::inserter( available_values, 
				available_values.begin() ));
}

////////////////////////////////////////////////////////////
//returns a reference to the set of remaining values for this variable
//used to save state of the variable
INLINE const std::set<BoolVariable::Value>& BoolVariable::GetDomain() const { 
	return available_values;
}

////////////////////////////////////////////////////////////
//whether variable has a value assigned to it
INLINE bool BoolVariable::IsAssigned() const { return value!=UNDEF; }

////////////////////////////////////////////////////////////
//true if no more values left for this variable
INLINE bool BoolVariable::IsImpossible() const { 
	return available_values.size() == 0; 
}

////////////////////////////////////////////////////////////
//assign a specific value. Given value should be legal, that is 
//in the set of available values for this variable
INLINE bool BoolVariable::Assign(Value val) {
	std::set<Value>::iterator it = available_values.find(val);
	//check value is in available_values
	if ( it != available_values.end() ) {
		is_assigned = true;
		value = val?TRUE:FALSE;
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////
//assign first available value
INLINE bool BoolVariable::Assign() {
	if ( IsImpossible() ) return false;
	is_assigned = true;
	value = *available_values.begin() ? TRUE:FALSE;
	return true;
}

////////////////////////////////////////////////////////////
//returns assigned value of this variable
//aborts if value is not assigned
INLINE bool BoolVariable::GetValue() const {
	if ( !is_assigned ) 
		assert(true); 
	return value==TRUE;
}

////////////////////////////////////////////////////////////
//remove assigned flag
INLINE void BoolVariable::UnAssign() { value=UNDEF; }

////////////////////////////////////////////////////////////
INLINE bool BoolVariable::Satisfiable() const {
	return value!=FALSE;  //if not EQUAL "f", then is Satisfiable
}

////////////////////////////////////////////////////////////
INLINE Bool BoolVariable::SatisfiableAux() const { 
	//std::cout << this << " " << name << " ->SatisfiableAux()\n";
	return value; 
}

INLINE std::vector<BoolVariable*> BoolVariable::GetVars() const {
	return std::vector<BoolVariable*> (1,const_cast<BoolVariable*>(this) );
}

