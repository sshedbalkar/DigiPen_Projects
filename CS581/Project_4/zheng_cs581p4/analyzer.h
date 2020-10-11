#ifndef ANALYZER_H
#define ANALYZER_H
#include "field.h"

class Analyzer 
{
protected:
	/*
		* real field - in part1 used only to create an MSfieldPart1 copy (temp = field;) for your "proof"
		* 
		* in part 2 you will also call field.OpenLocation(...) and field.MarkAsMine(...)
		* From my driver you may notice that "field" is actually a reference to MSfieldPart2,
		* so OpenLocation(...) will work on field (but not on temp)
		* also you are not allowed to create MSfieldPart2 copies of field 
		* (let me know if you figure out how to do this)
		*/
	MSfieldPart1 & field; 

	//temporary field - used for proofs
	MSfieldPart1 temp;    
public:
	Analyzer(MSfieldPart1 & field);
	bool IsMine(int x,int y);
	bool IsSafe(int x, int y);
protected:
	bool ApplyRule1and2( int x, int y );
	bool ApplyRule3( int x, int y );

	void MemorizeNeighbors( int x, int y );
	void StoreAllValues();
	bool CheckValidity();

	typedef std::pair<int, int> Location;
	typedef std::set< Location > LocationSet;
	typedef LocationSet::iterator LocationIterator;

	LocationSet locs;
	LocationSet values;

	void Difference( LocationSet& result, LocationSet& set0, LocationSet& set1 );
};

class Solver : public Analyzer 
{
public:
	Solver(MSfieldPart2 & field);
	void Solve();
private:
	LocationSet unknowns;
};

#endif
