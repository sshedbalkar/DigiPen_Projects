#include "boolean.constraint.h"
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
//constraint
template <typename E1, typename E2, typename Op>
INLINE BinaryOp<E1,E2,Op>::BinaryOp(const E1& _e1, const E2& _e2) 
	: e1(_e1), e2(_e2) 
{
	//copy - otherwise BoolVariable not supported
	std::vector<Variable*> e1_vars = e1.GetVars(); 
	typename std::vector<Variable*>::const_iterator b = e1_vars.begin();
	typename std::vector<Variable*>::const_iterator e = e1_vars.end();
	for ( ; b!=e; ++b ) { vars.push_back( *b ); }

	//copy - otherwise BoolVariable not supported
	std::vector<Variable*> e2_vars = e2.GetVars(); 

	b = e2_vars.begin();
	e = e2_vars.end();
	for ( ; b!=e; ++b ) { vars.push_back( *b ); }
}

template <typename E1, typename E2, typename Op>
INLINE bool BinaryOp<E1,E2,Op>::Satisfiable() const { 
	return Op::doit( e1.SatisfiableAux(), e2.SatisfiableAux())!=FALSE; 
}

template <typename E1, typename E2, typename Op>
INLINE Bool BinaryOp<E1,E2,Op>::SatisfiableAux() const { 
	return Op::doit( e1.SatisfiableAux(), e2.SatisfiableAux()); 
}

template <typename E1, typename E2, typename Op>
INLINE void BinaryOp<E1,E2,Op>::Print (std::ostream& os) const { 
	os << *this; 
}

template <typename E1, typename E2, typename Op>
INLINE BinaryOp<E1,E2,Op>* 
BinaryOp<E1,E2,Op>::clone () const { 
	return new BinaryOp<E1,E2,Op>(e1,e2); 
}

template <typename E1, typename E2, typename Op>
INLINE std::ostream& 
operator<<(std::ostream& os, const BinaryOp<E1,E2,Op>& binop) {
	return os << "(" << binop.e1 << " " << Op::name << " " << 
		binop.e2 << ")";
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
template <typename E1, typename Op>
INLINE UnaryOp<E1,Op>::UnaryOp(const E1& _e1) : e1(_e1) {
	//copy - otherwise BoolVariable not supported
	std::vector<Variable*> e1_vars = e1.GetVars(); 
	typename std::vector<Variable*>::const_iterator b = e1_vars.begin();
	typename std::vector<Variable*>::const_iterator e = e1_vars.end();
	for ( ; b!=e; ++b ) { vars.push_back( *b ); }
}

template <typename E1, typename Op>
INLINE bool UnaryOp<E1,Op>::Satisfiable() const { 
	return Op::doit(e1.SatisfiableAux())!=FALSE; 
}

template <typename E1, typename Op>
INLINE Bool UnaryOp<E1,Op>::SatisfiableAux() const { 
	return Op::doit(e1.SatisfiableAux()); 
}

template <typename E1, typename Op>
INLINE void UnaryOp<E1,Op>::Print (std::ostream& os) const { 
	os << *this; 
}

template <typename E1, typename Op>
INLINE UnaryOp<E1,Op>* UnaryOp<E1,Op>::clone () const { 
	return new UnaryOp<E1,Op>(e1); 
}

template <typename E1, typename Op>
INLINE std::ostream& operator<<(std::ostream& os, const UnaryOp<E1,Op>& unop) {
	return os << Op::name << unop.e1;
}
