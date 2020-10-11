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
					Analyzer( MSfieldPart1& field);
	virtual			~Analyzer();
	//
public:
	bool			IsMine( int x, int y );
	bool			IsSafe( int x, int y );
	//
protected:
	typedef std::pair<int, int>			Position;
	typedef std::set< Position >		PositionList;
	typedef PositionList::iterator		PositionItr;
	//
	PositionList	m_positionList;
	PositionList	m_valueList;
	//
protected:
	bool			Rule1( int x, int y );
	bool			Rule2( int x, int y );
	bool			Rule3( int x, int y );
	//
	void			ListNeighbors( int x, int y );
	void			PopulateValues();
	void			SetDifference( PositionList& set1, PositionList& set2, PositionList& result );
};

class Solver: public Analyzer 
{
public:
					Solver( MSfieldPart2& field );
					~Solver();
	//
	void			Solve();
	//
private:
	PositionList	m_unknowns;
};

#endif
