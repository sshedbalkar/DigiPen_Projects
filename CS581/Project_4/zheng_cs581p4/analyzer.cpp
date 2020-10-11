#include "analyzer.h"

const int neighbors[8][2] = { -1, -1, 0, -1, 1, -1, -1, 0, 1, 0, -1, 1, 0, 1, 1, 1 };
#define MAX(x,y) ((x)>(y)?(x):(y))

//======================================================================================
Analyzer::Analyzer(MSfieldPart1 & field) : field(field),temp(field) 
{
}
//======================================================================================
bool Analyzer::IsMine(int x,int y) 
{ 
	temp = field;
	temp.MarkAsSafe( x, y );
	this->StoreAllValues();
	this->MemorizeNeighbors( x, y );
	
	while( !locs.empty() )
	{
		LocationIterator it = locs.begin();
		if ( !ApplyRule1and2( it->first, it->second ) ) return true;
		ApplyRule3( it->first, it->second );
		locs.erase( it );
	}

	return false; 
}
//======================================================================================
bool Analyzer::IsSafe(int x, int y) 
{ 
	temp = field;
	temp.MarkAsMine( x, y );
	this->StoreAllValues();
	this->MemorizeNeighbors( x, y );
	

	while( !locs.empty() )
	{
		LocationIterator it = locs.begin();
		if ( !ApplyRule1and2( it->first, it->second ) ) return true;
		if ( !ApplyRule3( it->first, it->second ) ) return true;
		locs.erase( it );
	}

	return false; 
}
//======================================================================================
bool Analyzer::ApplyRule1and2( int x, int y )
{
	int val = temp.GetMineCount( x, y );
	int km = temp.KnownMines( x, y );
	LocationSet uls = temp.UnKnownLocations( x, y );
	int sizeul = uls.size();

	if ( val > km + sizeul ) return false;
	if ( km > val ) return false;
	if ( val - km == sizeul ) 
	{
		for ( LocationIterator it = uls.begin(); it != uls.end(); ++it )
		{
			temp.MarkAsMine( it->first, it->second );
			MemorizeNeighbors( it->first, it->second );
		}
	}
	else if ( val == km )
	{
		for ( LocationIterator it = uls.begin(); it != uls.end(); ++it )
		{
			temp.MarkAsSafe( it->first, it->second );
			MemorizeNeighbors( it->first, it->second );
		}
	}
	return true;
}
//======================================================================================
bool Analyzer::ApplyRule3( int x, int y )
{
	for ( LocationIterator it = values.begin(); it != values.end(); ++it )
	{
		int x1 = it->first, y1 = it->second;
		if ( MAX(abs( x1 - x ), abs( y1 - y ) ) > 2 ) continue;
		int val0 = temp.GetMineCount( x, y );
		int val1 = temp.GetMineCount( x1, y1 );
		int km0 = temp.KnownMines( x, y );
		int km1 = temp.KnownMines( x1, y1 );
		LocationSet ul0 = temp.UnKnownLocations( x, y );
		LocationSet ul1 = temp.UnKnownLocations( x1, y1 );

		LocationSet diff0; Difference( diff0, ul0, ul1 );
		LocationSet diff1; Difference( diff1, ul1, ul0 );

		int lhs = val0 - km0;
		lhs -= val1 - km1;

		if ( lhs > (int)diff0.size() ) return false;
		if ( -lhs > (int)diff1.size() ) return false;

		if ( lhs == diff0.size() )
		{
			for ( LocationIterator it1 = diff0.begin(); it1 != diff0.end(); ++it1 )
			{
				temp.MarkAsMine( it1->first, it1->second );
				this->MemorizeNeighbors( it1->first, it1->second );
			}
			for ( LocationIterator it1 = diff1.begin(); it1 != diff1.end(); ++it1 )
			{
				temp.MarkAsSafe( it1->first, it1->second );
				this->MemorizeNeighbors( it1->first, it1->second );
			}
		}
		else if ( -lhs == diff1.size() )
		{
			for ( LocationIterator it1 = diff0.begin(); it1 != diff0.end(); ++it1 )
			{
				temp.MarkAsSafe( it1->first, it1->second );
				this->MemorizeNeighbors( it1->first, it1->second );
			}
			for ( LocationIterator it1 = diff1.begin(); it1 != diff1.end(); ++it1 )
			{
				temp.MarkAsMine( it1->first, it1->second );
				this->MemorizeNeighbors( it1->first, it1->second );
			}
		}

	}
	return true;
}
//======================================================================================
bool Analyzer::CheckValidity()
{
	for ( LocationIterator it = locs.begin(); it != locs.end(); ++it )
	{
		int x = it->first, y = it->second;
		int target = temp.GetMineCount( x, y );
		int available = temp.KnownMines( x, y ) + temp.UnKnownLocations( x, y ).size();
		if ( available < target ) return false;
	}
	return true;
}
//======================================================================================
void Analyzer::MemorizeNeighbors( int x, int y )
{
	for ( unsigned int i = 0; i < 8; ++i )
	{
		int xn = x + neighbors[i][0];
		int yn = y + neighbors[i][1];

		if ( temp.CheckIsInside( xn, yn ) && temp.IsHasValue( xn, yn ) ) locs.insert( Location(xn,yn) );
	}
}
//======================================================================================
void Analyzer::StoreAllValues()
{
	for ( int i = 0 ; i < temp.GetMaxX(); ++i )
		for ( int j = 0; j < temp.GetMaxY(); ++j )
		{
			if ( temp.CheckIsInside( i, j ) && temp.IsHasValue( i, j ) ) values.insert( Location(i, j) );
		}
}
//======================================================================================
void Analyzer::Difference( LocationSet& result, LocationSet& set0, LocationSet& set1 )
{
	result = set0;
	for ( LocationIterator it = set1.begin(); it != set1.end(); ++it )
	{
		result.erase( *it );
	}
}
//====================================================================================== solver
Solver::Solver(MSfieldPart2 & field) : Analyzer(field)
{
}
//======================================================================================
void Solver::Solve()
{
	for ( int i = 0 ; i < field.GetMaxX(); ++i )
	{
		for ( int j = 0; j < field.GetMaxY(); ++j )
		{
			if ( field.CheckIsInside( i, j ) && !field.IsVisited( i, j ) ) unknowns.insert( Location(i, j) );
		}
	}
	while ( !unknowns.empty() )
	{
		bool progress = false;
		LocationIterator it = unknowns.begin(); 
		while ( it != unknowns.end() )
		{
			int x = it->first, y = it->second;
			if ( this->IsMine( x, y ) ) { field.MarkAsMine( x, y ); it = unknowns.erase( it ); progress = true; }
			else if ( this->IsSafe( x, y ) ) { field.OpenLocation( x, y ); it = unknowns.erase( it ); progress = true; }
			else ++it;
		}
		if ( !progress )
		{
			it = unknowns.begin();
			int number = field.OpenLocation( it->first, it->second );
			if ( number < 0 ) field.MarkAsMine( it->first, it->second );
			unknowns.erase( it );
		}
	}
}