#ifndef SUBSETS_H
#define SUBSETS_H
#include <vector>
#include <iostream>

////////////////////////////////////////////////////////////
class Subsets {
	public:
		Subsets(unsigned int _size) : size(_size), subset_id(0),stop(false) {
		}

		bool next(std::vector<unsigned int> & subset ) {
			if (stop) { return false; }

			//initialize subset according to the subset_id bits
			subset.clear();
			for (unsigned int i=0;i<size;++i) {
				if ( subset_id & (1<<i) ) { //i'th bit is set in subset_id, then
					//i is in the subset
					subset.push_back(i);
				}
			}

			//PREPARE FOR THE NEXT ITERATION
			//increment the subset_id
			//stop criteria - stop if subset_id == (2^size)
			if ( subset_id++ == 1<<size ) { return false; }
			return true;
		}
	private:
		unsigned int size; //up to 32
		unsigned int subset_id;
		bool stop;
};

#ifdef TEST_SUBSETS_H
//compile 
//g++ main-test-subsets.cpp
//run dvolper@main: learning $ ./a.out 
//{}
//{0 }
//{1 }
//{0 1 }
//{2 }
//{0 2 }
//{1 2 }
//{0 1 2 }
//{3 }
//{0 3 }
//{1 3 }
//{0 1 3 }
//{2 3 }
//{0 2 3 }
//{1 2 3 }
//{0 1 2 3 }
//{4 }
//{0 4 }
//{1 4 }
//{0 1 4 }
//{2 4 }
//{0 2 4 }
//{1 2 4 }
//{0 1 2 4 }
//{3 4 }
//{0 3 4 }
//{1 3 4 }
//{0 1 3 4 }
//{2 3 4 }
//{0 2 3 4 }
//{1 2 3 4 }
//{0 1 2 3 4 }

int main () {
	int size =4;
	Subsets subsets_generator(size);
	std::vector<unsigned int> subset(size);

	do {
		std::cout << "{";
		for (unsigned int i=0; i<subset.size(); ++i) {
			std::cout << subset[i] << " ";
		}
		std::cout << "}" << std::endl;
	} while ( subsets_generator.next(subsets) );
}
#endif

#endif
