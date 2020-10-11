#include <iostream>
#include <set>
#include <ctime>
#include <cstdlib> //srand
#include "field.h"
#include "analyzer.h"

int main(int argc, char** argv) try {
	if (argc < 2) {
		std::cerr << "Need a single argument\n";
		return -1;
	}
	std::srand ( time (NULL) ) ;
	//part2
	MSfieldPart2 f(argv[1]);
	std::cout << f << std::endl;
	Solver solver(f);
	solver.Solve();
	std::cout << f << std::endl;
	std::cout << "GetBustCounter " << f.GetBustCounter() << std::endl;

} catch ( MSexception& e) {
	std::cerr << e.what() << std::endl;
}
