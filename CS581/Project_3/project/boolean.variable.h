/*
 * Boolean variable supporting 3 values (true, false, undef)
 * Extended boolean implements functions and,or,not based
 * on the following definition: "undef" means the value is not
 * set, so function is evaluated to undef if the result depends
 * on the true/false of on the arguments: 
 * undef OR false = undef
 * undef AND false = false
 */
#ifndef BOOLEAN_VARIABLE_H
#define BOOLEAN_VARIABLE_H
#include <set>
#include <string>
#include "contraints.h"

#ifdef INLINE_BOOLEAN_VARIABLE
	#define INLINE inline
#else   
	#define INLINE 
#endif

enum BoolEnum { FALSE=0, TRUE=1, UNDEF=10};

struct Bool {
	BoolEnum value;

	Bool (BoolEnum b);
	Bool (bool b);
	Bool (int  b);
	
	Bool operator&& (const Bool& rhs) const;
	Bool operator|| (const Bool& rhs) const;
	Bool operator!  () const;
	bool operator!= (const BoolEnum& rhs) const;
	bool operator== (const BoolEnum& rhs) const;
};

std::ostream& operator<<(std::ostream& os, const Bool& b);

class BoolVariable {
	public:
		typedef bool Value; 
	private:
		friend std::ostream& operator<<(std::ostream& os, const BoolVariable& v);
		Bool value; //for internal use
		std::string name;
		std::set<Value> available_values;
		bool is_assigned; 
	public:
		BoolVariable(const std::string & name);
		////////////////////////////////////////////////////////////
		void RemoveValue(Value val);
		////////////////////////////////////////////////////////////
		const std::string & Name() const;
		////////////////////////////////////////////////////////////
		int SizeDomain() const;
		////////////////////////////////////////////////////////////
		void SetDomain(const std::set<Value>& vals);
		////////////////////////////////////////////////////////////
		//returns a reference to the set of remaining values for 
		//this variable -- used to save state of the variable
		const std::set<Value>& GetDomain() const;
		////////////////////////////////////////////////////////////
		//whether variable has a value assigned to it
		bool IsAssigned() const;
		////////////////////////////////////////////////////////////
		//true if no more values left for this variable
		bool IsImpossible() const;
		////////////////////////////////////////////////////////////
		void Print () const;
		////////////////////////////////////////////////////////////
		//assign a specific value. Given value should be legal, that is 
		//in the set of available values for this variable
		bool Assign(Value val);
		////////////////////////////////////////////////////////////
		//assign first available value
		bool Assign();
		////////////////////////////////////////////////////////////
		//returns assigned value of this variable
		//aborts if value is not assigned
		bool GetValue() const;
		////////////////////////////////////////////////////////////
		//remove assigned flag
		void UnAssign();
		////////////////////////////////////////////////////////////
		bool Satisfiable() const;
		////////////////////////////////////////////////////////////
		Bool SatisfiableAux() const;
		std::vector<BoolVariable*> GetVars() const;
		////////////////////////////////////////////////////////////
		void Print (std::ostream& os) const;
		////////////////////////////////////////////////////////////
		const BoolVariable* clone() const;
};
std::ostream& operator<<(std::ostream& os, const BoolVariable& v);

#ifdef INLINE_BOOLEAN_VARIABLE
	#include "boolean.variable.inl"
#endif
#undef INLINE 

#endif
