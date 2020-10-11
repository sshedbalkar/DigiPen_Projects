#include <iostream>
#include <sstream>
#include <cstdlib>
#include "vector.h"

int main() {
	CS225::SparseVector v1;
	for (int i=0;i<16;++i) v1[i] = i%2;

	const CS225::SparseVector v2(v1);
	
	CS225::SparseVector v3(v2); //check copy construction from const vector
	v3=v2; //check assignment from const vector
	return 0;
}
//add overwrite
//check assignment from const
