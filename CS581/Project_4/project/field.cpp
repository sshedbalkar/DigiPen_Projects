#include "field.h"
#include <cstring>
#include <iostream>

MSfieldPart1::MSfieldPart1(const char * filename) : x_max(0), y_max(0), data(NULL), bust_counter(0) {
	const int MAX_LINE_SIZE =255;
	std::ifstream field_file(filename,std::ifstream::in);

	/* read - dimensions*/
	/* figure_out x_max */
	field_file.ignore(MAX_LINE_SIZE,'+');//skip till +
	char  s[MAX_LINE_SIZE];
	field_file.getline(s,MAX_LINE_SIZE,'+');
	x_max = std::strlen(s);
	//std::cout << "x_max = " << x_max << std::endl;
	field_file.ignore(MAX_LINE_SIZE,'\n');//skip till new line

	std::vector< char* > raw_data; //temporary storage for rows
	/* figure_out y_max */
	field_file.ignore(MAX_LINE_SIZE,'|');//skip to past "|"
	while ( !field_file.eof() ) {
		raw_data.push_back( new char[x_max+1] );
		field_file.getline(raw_data.back(),x_max+1,'|');
		field_file.ignore(MAX_LINE_SIZE,'|');//skip to next row
	}
	field_file.close();
	y_max = raw_data.size();
	//std::cout << "y_max = " << y_max << std::endl;

	//allocate real storage
	data = new int* [x_max];
	for (int x=0;x<x_max;++x) {
		data[x] = new int[y_max]; 
		for (int y=0;y<y_max;++y) data[x][y] = -10;  /*init*/
	}

	//read data from temporary storage into real
	for (int y=0;y<y_max;++y) { /*rows*/
		for (int x=0;x<x_max;++x) {
			switch (raw_data[y][x]) {
				case 'm': /*known mine*/         
					data[x][y] = -1; 
					break;
				case 'M': /*mine unknown to AI*/ 
					data[x][y] = -5; 
					break;
				case ' ': /*unknown content*/    
					data[x][y] = -10; 
					break;
				default : /* should be a 0-8 character */
					data[x][y] = raw_data[y][x] - 48;
					if ( data[x][y] <0 || data[x][y]>8) {
						std::cerr << "Illegal character in input " 
							<< raw_data[y][x] << " changing to unknown\n";
						data[x][y] = -10;
					}
					break;
			}
		}
	}
	std::vector< char*>::iterator b = raw_data.begin();
	std::vector< char*>::iterator e = raw_data.end();
	for ( ; b != e; ++b) { delete [] *b; }
}

////////////////////////////////////////////////////////////
MSfieldPart1::~MSfieldPart1() {
	for (int x=0;x<x_max;++x) { delete [] data[x]; }
	delete [] data;
}

////////////////////////////////////////////////////////////
MSfieldPart1::MSfieldPart1(const MSfieldPart1& rhs) 
	: x_max(rhs.x_max), y_max(rhs.y_max),data(new int* [rhs.x_max]),bust_counter(rhs.bust_counter) 
{
	for (int x=0;x<x_max;++x) {
		data[x] = new int[y_max]; 
		for (int y=0;y<y_max;++y) data[x][y] = rhs.data[x][y];  /*init*/
	}
}

////////////////////////////////////////////////////////////
MSfieldPart1& MSfieldPart1::operator=(const MSfieldPart1& rhs) {
	if (this == &rhs) return *this;

	for (int x=0;x<x_max;++x) { delete [] data[x]; }
	delete [] data;

	x_max = rhs.x_max;
	y_max = rhs.y_max;
	bust_counter = rhs.bust_counter;

	data = new int* [x_max];
	for (int x=0;x<x_max;++x) {
		data[x] = new int[y_max]; 
		for (int y=0;y<y_max;++y) data[x][y] = rhs.data[x][y];  /*init*/
	}
	return *this;
}

////////////////////////////////////////////////////////////
int MSfieldPart1::GetBustCounter() const { return bust_counter; }

////////////////////////////////////////////////////////////
int MSfieldPart1::GetMaxX() const { return x_max; }

////////////////////////////////////////////////////////////
int MSfieldPart1::GetMaxY() const { return y_max; }

////////////////////////////////////////////////////////////
bool MSfieldPart1::IsKnownMine(int x, int y) const throw (MSexception) { 
	if ( !CheckIsInside(x,y) )
		throw MSexception("IsKnownMine - location is outside the grid");
	return data[x][y] == -1; 
}

////////////////////////////////////////////////////////////
//this function may be used by class and derivatives ONLY
//return true if there is a mine (even if it's unknown to AI)
bool MSfieldPart1::IsMine(int x, int y) const throw (MSexception) { 
	if ( !CheckIsInside(x,y) )
		throw MSexception("IsKnownMine - location is outside the grid");
	return data[x][y] == -1 || data[x][y] == -5; 
}

////////////////////////////////////////////////////////////
int MSfieldPart1::OpenLocation_aux(int x, int y) throw (MSexception) {
	//std::cout << "OpenLocation_aux (" << x << "," << y << ") = " << data[x][y] << std::endl;
	if ( !CheckIsInside(x,y) )
		throw MSexception("OpenLocation - location is outside the grid");
	if ( IsMine(x,y) ) { //mine
		++bust_counter;
		return -100; //busted
	}
	int count = 0;
	for (int i=-1;i<2;++i) {
		for (int j=-1;j<2;++j) { //8 neighbors around x,y
			if ( (i || j) && CheckIsInside(x+i,y+j) ) {
				//std::cout << "open looks at " << x << " " << y << "  ";
				if ( IsMine(x+i,y+j ) ) { 
					++count; 
					//std::cout << " MINE" << std::endl;
				} else {
					//std::cout << " SAFE" << std::endl;
				}
			}
		}
		data[x][y] = count;
	}
	return count;
}

////////////////////////////////////////////////////////////
//false iff location is open or marked as mine
//true otherwise
bool MSfieldPart1::IsKnown(int x, int y) const throw (MSexception) { 
	//std::cout << "IsKnown (" << x << "," << y << ") = " << data[x][y] << std::endl;
	if ( !CheckIsInside(x,y) )
		throw MSexception("IsKnown - location is outside the grid");
	return data[x][y] >= -2; 
}

////////////////////////////////////////////////////////////
bool MSfieldPart1::IsVisited(int x, int y) const throw (MSexception) { 
	//std::cout << "IsVisited (" << x << "," << y << ") = " << data[x][y] << std::endl;
	if ( !CheckIsInside(x,y) )
		throw MSexception("IsVisited - location is outside the grid");
	return data[x][y] >= -1; 
}

////////////////////////////////////////////////////////////
bool MSfieldPart1::IsHasValue(int x, int y) const throw (MSexception) { 
	//std::cout << "IsSafe (" << x << "," << y << ") = " << data[x][y] << std::endl;
	if ( !CheckIsInside(x,y) )
		throw MSexception("IsHasValue - location is outside the grid");
	return data[x][y] >= 0; 
}

////////////////////////////////////////////////////////////
int MSfieldPart1::GetMineCount(int x, int y) const throw (MSexception) {
	//std::cout << "in GetMineCount (" << x << "," << y << ") = " << data[x][y] << std::endl;
	if ( !CheckIsInside(x,y) )
		throw MSexception("GetMineCount - location is outside the grid");
	if ( data[x][y] >= 0 ) return data[x][y];
	else { //this square wasn't open or is a mine, 
		//so this is an illegal question
		throw 
			MSexception("GetMineCount - no mine count for unknown and/or mine locations");
	}
}

////////////////////////////////////////////////////////////
std::set<std::pair<int,int> > MSfieldPart1::UnKnownLocations(int x, int y) const throw (MSexception) {
	if ( !CheckIsInside(x,y) )
		throw 
			MSexception("UnKnownLocations - location is outside the grid");
	std::set<std::pair<int,int> > result;
	for (int i=-1;i<2;++i) {
		for (int j=-1;j<2;++j) {
			if ( (i || j) && x+i<x_max && x+i>-1 && y+j<y_max && y+j>-1 ) {
				if ( !IsKnown( x+i,y+j ) ) {
					result.insert (std::make_pair<int,int>(x+i,y+j));
				}
			}
		} 
	}
	return result;
}


////////////////////////////////////////////////////////////
bool MSfieldPart1::CheckIsInside(int x,int y) const throw (MSexception) {
	return ( x<x_max && x>-1 && y<y_max && y>-1 );
}

////////////////////////////////////////////////////////////
int MSfieldPart1::KnownMines(int x, int y) const throw (MSexception) {
	if ( !CheckIsInside(x,y) )
		throw MSexception("KnownMines - location is outside the grid");
	int count = 0;
	for (int i=-1;i<2;++i) 
	{
		for (int j=-1;j<2;++j) 
		{ //8 neighbors around x,y
			if ( (i || j) 
					&& x+i<x_max && x+i>-1 
					&& y+j<y_max && y+j>-1 ) 
			{
				if ( IsKnownMine( x+i,y+j ) ) { ++count; }
			}
		} 
	}
	return count;
}


////////////////////////////////////////////////////////////
void MSfieldPart1::MarkAsSafe(int x, int y) throw (MSexception) { //mark safe but do not open the location
	//std::cout << "Mark as Safe (" << x << "," << y << ")\n";
	if ( !CheckIsInside(x,y) )
		throw MSexception("MarkAsSafe - location is outside the grid");
	//do not override values,
	//that is - if location is already marked as mine, do nothing
	if (  data[x][y] == -10  || data[x][y] == -5 ) data[x][y] = -2;
	//DebugPrint(std::cout);
}

////////////////////////////////////////////////////////////
void MSfieldPart1::MarkAsMine(int x, int y) throw (MSexception) {
	//std::cout << "Mark as Mine (" << x << "," << y << ")\n";
	if ( !CheckIsInside(x,y) )
		throw MSexception("MarkAsMine - location is outside the grid");
	//do not override values,
	//that is - if location is already marked as safe, do nothing
	if (  data[x][y] == -10 || data[x][y] == -5 ) data[x][y] = -1;
	//DebugPrint(std::cout);
}

////////////////////////////////////////////////////////////
//prints a little bit more info compared to operator<<
void MSfieldPart1::DebugPrint(std::ostream& os) const {
	os << "Print in debug mode" << std::endl;
	os << "+";
	for (int x=0;x<4*x_max-1;++x) os << "-";
	os << "+\n";

	for (int y=0;y<y_max;++y) { /*rows*/
		for (int x=0;x<x_max;++x) {
			switch (data[x][y]) {
				case -1:  os << "| m "; break;
				case -2:  os << "| s "; break;
				case -5:  os << "| M "; break; 
				case -10: os << "|   "; break;
				default:  os << "| "<< data[x][y] << " "; break;
			}
		}
		os << "|\n";
	}
	os << "+";
	for (int x=0;x<4*x_max-1;++x) os << "-";
	os << "+\n";
}

////////////////////////////////////////////////////////////
int MSfieldPart1::OpenLocation(int x, int y) throw (MSexception) {
	if ( !CheckIsInside(x,y) )
		throw MSexception("OpenLocation - location is outside the grid");
	throw MSexception("OpenLocation - illegal to use in part 1");
}


////////////////////////////////////////////////////////////
std::ostream & operator<<(std::ostream& os, const MSfieldPart1& msf) {
	os << "+";
	for (int x=0;x<4*msf.x_max-1;++x) os << "-";
	os << "+\n";

	for (int y=0;y<msf.y_max;++y) { /*rows*/
		for (int x=0;x<msf.x_max;++x) {
			switch (msf.data[x][y]) {
				case -1:  os << "| m "; break;
				case -2:  os << "| s "; break;
				case -5:  os << "|   "; break; 
				case -10: os << "|   "; break;
				default:  os << "| "<< msf.data[x][y] << " "; break;
			}
		}
		os << "|\n";
	}
	os << "+";
	for (int x=0;x<4*msf.x_max-1;++x) os << "-";
	os << "+\n";
	return os;
}


////////////////////////////////////////////////////////////
MSfieldPart2::MSfieldPart2(const char * filename) : MSfieldPart1(filename) { }

////////////////////////////////////////////////////////////
int MSfieldPart2::OpenLocation(int x, int y) throw (MSexception) {
	return OpenLocation_aux(x,y);
}
