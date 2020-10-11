#include "csp.h"
#include <limits>

#ifdef INLINE_CSP
	//#warning "INFO - inlining CSP methods"
	#define INLINE inline
#else   
	//#warning "INFO - NOT inlining CSP methods"
	#define INLINE 
#endif

////////////////////////////////////////////////////////////
//CSP constructor
template <typename T> 
CSP<T>::CSP(T &cg) :
	arc_consistency(),
	cg(cg),
	solution_counter(0),
	recursive_call_counter(0),
	iteration_counter(0) 
{
}

////////////////////////////////////////////////////////////
//CSP solver, brute force - no forward checking
template <typename T> 
bool CSP<T>::SolveDFS( unsigned level )
{
	++recursive_call_counter;
	//
	if( cg.AllVariablesAssigned() )
	{
		return true;
	}
	//
	Variable* var_to_assign = MinRemVal();
	if( !var_to_assign )
	{
		return false;
	}
	//
	const std::set<Value>& values = var_to_assign->GetDomain();
	for ( typename std::set<Value>::iterator it1 = values.begin(), it2 = values.end(); it1 != it2; ++it1 )
	{
		++iteration_counter;
		//
		var_to_assign->Assign( *it1 );
		//
		if( AssignmentIsConsistent(var_to_assign) )
		{
			if( SolveDFS(level + 1) )
			{
				return true;
			}
		}
		var_to_assign->UnAssign();
	}
	//
	return false;
}


////////////////////////////////////////////////////////////
//CSP solver, uses forward checking
template <typename T> 
bool CSP<T>::SolveFC( unsigned level )
{
	++recursive_call_counter;
	//
	if( cg.AllVariablesAssigned() )
	{
		return true;
	}
	//
	Variable* var_to_assign = MinRemVal();
	if( !var_to_assign )
	{
		return false;
	}
	//
	const std::set<Value>& values = var_to_assign->GetDomain();
	std::map< typename CSP<T>::Variable*, std::set<typename CSP<T>::Variable::Value> > state = SaveState( var_to_assign );
	//
	for( typename std::set<Value>::iterator it1 = values.begin(), it2 = values.end(); it1 != it2; ++it1 )
	{
		++iteration_counter;
		var_to_assign->Assign( *it1 );
		//
		if( AssignmentIsConsistent(var_to_assign) && ForwardChecking(var_to_assign) )
		{
			if( SolveFC(level + 1) )
			{
				return true;
			}
		}
		var_to_assign->UnAssign();
		LoadState( state );
	}
	//
	return false;
}
////////////////////////////////////////////////////////////
//CSP solver, uses arc consistency
template <typename T> 
bool CSP<T>::SolveARC( unsigned level )
{
	++recursive_call_counter;
	//
	if( cg.AllVariablesAssigned() )
	{
		return true;
	}
	//
	Variable* var_to_assign = MinRemVal();
	if( !var_to_assign )
	{
		return false;
	}
	//
	const std::set<Value>& values = var_to_assign->GetDomain();
	std::map< typename CSP<T>::Variable*, std::set<typename CSP<T>::Variable::Value> > state = SaveState( var_to_assign );
	//
	for( typename std::set<Value>::iterator it1 = values.begin(), it2 = values.end(); it1 != it2; ++it1 )
	{
		++iteration_counter;
		var_to_assign->Assign( *it1 );
		//
		if( AssignmentIsConsistent(var_to_assign) && CheckArcConsistency(var_to_assign) )
		{
			if( SolveARC(level + 1) )
			{
				return true;
			}
		}
		var_to_assign->UnAssign();
		LoadState( state );
	}
	//
	return false;
}


template <typename T> 
INLINE
bool CSP<T>::ForwardChecking( Variable *x )
{
	const typename std::set<Variable*>& neighbors = cg.GetNeighbors( x );
	std::set<Value> remove;
	//
	typedef typename std::set<Variable*>::iterator SetVarIt;
	typedef typename std::set<Value>::iterator SetValIt;
	//
	for( SetVarIt it1 = neighbors.begin(), it2 = neighbors.end(); it1 != it2; ++it1 )
	{
		if( (*it1)->IsAssigned() )
		{
			continue;
		}
		//
		remove.clear();
		const std::set<Value>& values = (*it1)->GetDomain();
		//
		for( SetValIt it21 = values.begin(), it22 = values.end(); it21 != it22; ++it21 )
		{
			(*it1)->Assign( *it21 );
			if( !AssignmentIsConsistent(*it1) )
			{
				remove.insert( *it21 );
			}
			(*it1)->UnAssign();
		}
		//
		for( SetValIt it21 = remove.begin(), it22 = remove.end(); it21 != it22; ++it21 )
		{
			(*it1)->RemoveValue( *it21 );
		}
		//
		if( (*it1)->IsImpossible() )
		{
			return false;
		}
	}
	return true;
}
////////////////////////////////////////////////////////////
//load states (available values) of all unassigned variables 
template <typename T> 
void CSP<T>::LoadState(
		std::map<Variable*, 
		std::set<typename CSP<T>::Variable::Value> >& saved) const 
{
	typename std::map<Variable*, std::set<typename Variable::Value> >::iterator 
		b_result = saved.begin();
	typename std::map<Variable*, std::set<typename Variable::Value> >::iterator 
		e_result = saved.end();

	for ( ; b_result != e_result; ++b_result ) {
		//std::cout << "loading state for " 
		//<< b_result->first->Name() << std::endl;
		(*b_result).first->SetDomain( (*b_result).second );
	}
}


////////////////////////////////////////////////////////////
//save states (available values) of all unassigned variables 
//except the current
template <typename T> 
INLINE
std::map< typename CSP<T>::Variable*, std::set<typename CSP<T>::Variable::Value> > 
CSP<T>::SaveState(typename CSP<T>::Variable* x) const {
	std::map<Variable*, std::set<typename Variable::Value> > result;

	const std::vector<Variable*>& all_vars = cg.GetAllVariables();
	typename std::vector<Variable*>::const_iterator 
		b_all_vars = all_vars.begin();
	typename std::vector<Variable*>::const_iterator 
		e_all_vars = all_vars.end();
	for ( ; b_all_vars!=e_all_vars; ++b_all_vars) {
		if ( !(*b_all_vars)->IsAssigned() && *b_all_vars!=x ) {
			//std::cout << "saving state for " 
			//<< (*b_all_vars)->Name() << std::endl;
			result[ *b_all_vars ] = (*b_all_vars)->GetDomain();
		}
	}
	return result;
}
////////////////////////////////////////////////////////////
//check the current (incomplete) assignment for satisfiability
template <typename T> 
INLINE
bool CSP<T>::AssignmentIsConsistent( Variable* p_var ) const
{
	typedef typename std::vector<const Constraint*> VecConstraint;
	//
	const VecConstraint& constraints = cg.GetConstraints( p_var );
	//
	for( typename VecConstraint::const_iterator it1 = constraints.begin(), it2 = constraints.end(); it1 != it2; ++it1 )
	{
		if( !(*it1)->Satisfiable() )
		{
			return false;
		}
	}
	return true;
}
////////////////////////////////////////////////////////////
//insert pair 
//(neighbors of the current variable, the current variable)
// for all y~x insert (y,x)
//into arc-consistency queue
template <typename T> 
INLINE
void CSP<T>::InsertAllArcsTo( Variable* cv, Variable *except )
{
	typedef typename std::set<Variable*> SetVar;
	typedef typename std::set<const Constraint*> SetConstraint;
	//
	const SetVar& neighbors = cg.GetNeighbors( cv );
	//
	for( typename SetVar::iterator it1 = neighbors.begin(), it2 = neighbors.end(); it1 != it2; ++it1 )
	{
		if( *it1 == except || (*it1)->IsAssigned() )
		{
			continue;
		}
		//
		SetConstraint& constraints = cg.GetConnectingConstraints( *it1, cv );
		for( typename SetConstraint::iterator it21 = constraints.begin(), it22 = constraints.end(); it21 != it22; ++it21 )
		{
			arc_consistency.insert( Arc<Constraint>( *it1, cv, *it21 ) );
		}
	}
}
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//AIMA p.146 AC-3 algorithm
template <typename T> 
INLINE
bool CSP<T>::CheckArcConsistency( Variable* x ) 
{
	InsertAllArcsTo( x, x );
	//
	while( !arc_consistency.empty() )
	{
		const Arc<Constraint>& arc = *( arc_consistency.begin() );
		//
		if( this->RemoveInconsistentValues(arc.x, arc.y, arc.c) )
		{
			if( arc.x->IsImpossible() )
			{
				return false;
			}
			InsertAllArcsTo( arc.x, x );
		}
		arc_consistency.erase( arc_consistency.begin() );
	}
	return true;
}
////////////////////////////////////////////////////////////
//CHECK that for each value of x there is a value of y 
//which makes all constraints involving x and y satisfiable
template <typename T> 
INLINE
bool CSP<T>::RemoveInconsistentValues( Variable* x, Variable* y, const Constraint* c )
{
	const std::set<Value>& values = x->GetDomain();
	std::set<Value> remove;
	//
	for( typename std::set<Value>::iterator it1 = values.begin(), it2 = values.end(); it1 != it2; ++it1 )
	{
		x->Assign( *it1 );
		if ( !c->Satisfiable() )
		{
			remove.insert( *it1 );
		}
		x->UnAssign();
	}
	//
	for( typename std::set<Value>::iterator it1 = remove.begin(), it2 = remove.end(); it1 != it2; ++it1 )
	{
		x->RemoveValue( *it1 );
	}
	//
	return !remove.empty();
}
////////////////////////////////////////////////////////////
//choose next variable for assignment
//choose the one with minimum remaining values
template <typename T> 
INLINE
typename CSP<T>::Variable* CSP<T>::MinRemVal()
{
	typedef typename std::vector<Variable*> VecVar;
	//
	Variable* min = NULL;
	const VecVar& vlist = this->cg.GetAllVariables();
	int minVal = std::numeric_limits<int>::max();
	//
	for( typename VecVar::const_iterator it1 = vlist.begin(), it2 = vlist.end(); it1 != it2; ++it1 )
	{
		if( (*it1)->IsAssigned() )
		{
			continue;
		}
		if( (*it1)->SizeDomain() < minVal )
		{
			minVal = (*it1)->SizeDomain();
			min = *it1;
		}
	}
	return min;
}
////////////////////////////////////////////////////////////
//choose next variable for assignment
//choose the one with max degree
template <typename T> 
typename CSP<T>::Variable* CSP<T>::MaxDegreeHeuristic() {














}
#undef INLINE
