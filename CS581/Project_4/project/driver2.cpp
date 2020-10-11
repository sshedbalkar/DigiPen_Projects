#include <iostream>
#include <set>
#include <ctime>
#include <cstdlib>
#include "field.h"
#include "analyzer.h"

int main() try {
	std::srand ( time (NULL) ) ;
	//part2
	MSfieldPart2 f4_2("field4_part2");
	std::cout << f4_2 << std::endl;
	Solver solver4_2(f4_2);
	solver4_2.Solve();
	std::cout << f4_2 << std::endl;
	std::cout << "GetBustCounter " << f4_2.GetBustCounter() << std::endl;

	MSfieldPart2 f2_2("field2_part2");
	std::cout << f2_2 << std::endl;
	Solver solver2_2(f2_2);
	solver2_2.Solve();
	std::cout << f2_2 << std::endl;
	std::cout << "GetBustCounter " << f2_2.GetBustCounter() << std::endl;

	MSfieldPart2 f6_2("field6_part2");
	std::cout << f6_2 << std::endl;
	Solver solver6_2(f6_2);
	solver6_2.Solve();
	std::cout << f6_2 << std::endl;
	std::cout << "GetBustCounter " << f6_2.GetBustCounter() << std::endl;
		
	//I'll add code to compare your solution to mine to test whether you found
	//all possible mines/safe places.
} catch ( MSexception& e) {
	std::cerr << e.what() << std::endl;
}
