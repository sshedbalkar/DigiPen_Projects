#include <vector>
#include <iostream>
#include "subsets.h"

int main () {
	std::vector<unsigned int> subset;
	Subsets subsets_generator(5);

	while ( subsets_generator.next(subset) ) {
		std::cout << "{";
		for (unsigned int i=0; i<subset.size(); ++i) {
			std::cout << subset[i] << " ";
		}
		std::cout << "}" << std::endl;
	}
}

