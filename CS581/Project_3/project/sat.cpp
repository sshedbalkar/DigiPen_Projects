#include <iostream>
#include <fstream>
#include <cmath>
#include <set>
#include <string>
#include <cassert>
#include "boolean.variable.h"
#include "boolean.constraint.h"
#include "contraints.graph.h"
#include "csp.h"


int main(void) {
	BoolVariable x("x"); //x.Assign(false);
	BoolVariable y("y"); //y.Assign(true);
	BoolVariable z("z"); //z.Assign(true);

	ConstraintGraph<Constraint<BoolVariable> > cg;
	cg.InsertVariable( x );
	cg.InsertVariable( y );
	cg.InsertVariable( z );
	cg.InsertConstraint( x && y || z );
	cg.InsertConstraint( x || z );
	cg.InsertConstraint( !y );
	cg.InsertConstraint( x && z );

	cg.Print();
	std::cout << std::endl;
	cg.PreProcess();

	CSP<ConstraintGraph<Constraint<BoolVariable> > > csp( cg );
	if ( csp.SolveDFS(0) ) {
		cg.Print();
		std::cout << std::endl;
	} 
	else std::cout << "No solution found\n";
}
