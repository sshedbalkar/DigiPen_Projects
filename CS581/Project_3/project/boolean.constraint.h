/*
 * Boolean constraints based on expression templates
 * Expression templates allow client to use syntax like
 * cg.InsertConstraint( x && y || z );
 * also "!" (not)
 */
#ifndef BOOLEAN_CONSTRAINT_H
#define BOOLEAN_CONSTRAINT_H
#include <set>
#include <string>
#include "contraints.h"

#ifdef INLINE_BOOLEAN_CONSTRAINT
	#define INLINE inline
#else   
	#define INLINE 
#endif

//binary constraint
template <typename E1, typename E2, typename Op>
class BinaryOp : public Constraint<BoolVariable> {
	public:
		const E1& e1;
		const E2& e2;
		BinaryOp(const E1& _e1, const E2& _e2);
		bool Satisfiable() const;
		Bool SatisfiableAux() const;
		void Print (std::ostream& os) const;
		BinaryOp<E1,E2,Op>* clone () const;
};

template <typename E1, typename E2, typename Op>
std::ostream& operator<<(std::ostream& os, const BinaryOp<E1,E2,Op>& binop);

//unary constraint
template <typename E1, typename Op>
class UnaryOp : public Constraint<BoolVariable> {
	public:
		const E1& e1;
		UnaryOp(const E1& _e1);
		bool Satisfiable() const;
		Bool SatisfiableAux() const;
		void Print (std::ostream& os) const;
		UnaryOp<E1,Op>* clone () const;
		//operator bool () const { return true; }
};
template <typename E1, typename Op>
std::ostream& operator<<(std::ostream& os, const UnaryOp<E1,Op>& unop);

//actual operators
//operator AND
struct And {
  inline static Bool doit(const Bool& x, const Bool& y) { return x && y; }
  static const std::string name;
};
const std::string And::name = "^";

//operator OR
struct Or {
  inline static Bool doit(const Bool& x, const Bool& y) { return x || y; }
  static const std::string name;
};
const std::string Or::name = "V";

//operator NOT
struct Not {
	inline static Bool doit(const Bool& x) { return !x; }
	static const std::string name;
};
const std::string Not::name = "!";

//global operators
//syntax helper
template <typename E1, typename E2>
BinaryOp<E1, E2, And> operator&&(const E1& e1, const E2& e2) {
  return BinaryOp<E1, E2, And>(e1, e2);
}

//syntax helper
template <typename E1, typename E2>
BinaryOp<E1, E2, Or> operator||(const E1& e1, const E2& e2) {
  return BinaryOp<E1, E2, Or>(e1, e2);
}

//syntax helper
template <typename E1>
UnaryOp<E1, Not> operator!(const E1& e1) {
	return UnaryOp<E1, Not>(e1);
}

#include "boolean.constraint.cpp"
#undef INLINE 
#endif
