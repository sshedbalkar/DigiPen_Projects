#ifndef FIELD_H
#define FIELD_H

#include <fstream>
#include <iostream>
#include <set>
#include <vector>

class MSexception : public std::exception {
	std::string msg;
	public:
	MSexception(const std::string & _msg) : msg(_msg) {}
	const char * what() const throw () { return msg.c_str(); }
	virtual ~MSexception() throw () {}
};
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

/* 
 * -10 - unknown square 
 * -5 - mine in this square, but unknown to the AI
 * -2 - safe location, but not open yet
 * -1 - mine in this square
 * 0,...,8 - open and visited
 *  */

/* Definition:
 * Safe location - you know there is no mine, but you haven't clicked on it yet
 */

/*
 * Part 1 is a static analyzer, so you may mark safe and mine, but not allowed to 
 * open locations (OpenLocation(...) will throw if you call it)
 */
class MSfieldPart1 {
	int x_max, y_max;
	int ** data;
	int bust_counter; //number of times user steps on a mine (part2)
	public:

	//the usual stuff
	MSfieldPart1(const char * filename);
	virtual ~MSfieldPart1();
	MSfieldPart1(const MSfieldPart1& rhs);
	MSfieldPart1& operator=(const MSfieldPart1& rhs);

	//simple getters
	int GetBustCounter() const;
	int GetMaxX() const;
	int GetMaxY() const;


	public:
	//IS-methods - for if-statements
	/* Terminology:
	 * Known    - contains a mine OR a non-negative number OR safe (-2,-1,0,1,2,3,4,5,6,7,8)
	 * Visited  - contains a mine OR a non-negative number (-1,0,1,2,3,4,5,6,7,8)
	 * HasValue - non-negative number (0,1,2,3,4,5,6,7,8)
	 * */
	bool IsKnown(int x, int y) const throw (MSexception);
	bool IsVisited(int x, int y) const throw (MSexception);
	bool IsHasValue(int x, int y) const throw (MSexception);
	bool IsKnownMine(int x, int y) const throw (MSexception);

	//these are methods needed by Rules
	//Val(x)
	int 							GetMineCount(int x, int y) const throw (MSexception);
	//UL(X)
	std::set<std::pair<int,int> > 	UnKnownLocations(int x, int y) const throw (MSexception);
	//KM(X)
	int 							KnownMines(int x, int y) const throw (MSexception);
	bool 							CheckIsInside(int x,int y) const throw (MSexception);

	//methods to modify data
	void MarkAsSafe(int x, int y) throw (MSexception);
	void MarkAsMine(int x, int y) throw (MSexception);

	//prints a little bit more info compared to operator<<
	void DebugPrint(std::ostream& os) const;

	//required only for part 2
	virtual int OpenLocation(int x, int y) throw (MSexception);

	friend 
		std::ostream & operator<<(std::ostream& os, const MSfieldPart1& msf);

	protected: //MOFIFIED FOR GRADING
	//this function may be used by class and derivatives ONLY
	//return true if there is a mine (even if it's unknown to AI)
	bool IsMine(int x, int y) const throw (MSexception);
	int OpenLocation_aux(int x, int y) throw (MSexception);

};

std::ostream & operator<<(std::ostream& os, const MSfieldPart1& msf);

class MSfieldPart2 : public MSfieldPart1 {
	private:
		//MSfieldPart2 is copiable/assignable
		//no cheating!!!
		MSfieldPart2(const MSfieldPart2 & rhs);
		MSfieldPart2& operator=(const MSfieldPart2 & rhs);
	public:
		MSfieldPart2(const char * filename);
		//returns a number of adjacent mines
		int OpenLocation(int x, int y) throw (MSexception);
};

#endif
