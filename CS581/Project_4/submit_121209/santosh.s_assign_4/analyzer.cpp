#include "analyzer.h"
#include <stdlib.h>
//
#define max( a, b ) (((a) > (b)) ? (a) : (b))
//
Analyzer::Analyzer(MSfieldPart1 & field) : field(field),temp(field) 
{
	//
}
//
Analyzer::~Analyzer()
{
	//
}
//
bool Analyzer::IsMine( int x, int y )
{ 
	temp = field;
	temp.MarkAsSafe( x, y );
	this->PopulateValues();
	this->ListNeighbors( x, y );
	//
	PositionItr it;
	int posx, posy;
	while( !m_positionList.empty() )
	{
		it = m_positionList.begin();
		//
		posx = it->first;
		posy = it->second;
		//
		if( !Rule1(posx, posy) || !Rule2(posx, posy) )
		{
			return true;
		}
		Rule3( posx, posy );
		//
		m_positionList.erase( it );
	}
	//
	return false; 
}
//
bool Analyzer::IsSafe( int x, int y )
{ 
	temp = field;
	temp.MarkAsMine( x, y );
	this->PopulateValues();
	this->ListNeighbors( x, y );
	//
	PositionItr it;
	int posx, posy;
	while( !m_positionList.empty() )
	{
		it = m_positionList.begin();
		//
		posx = it->first;
		posy = it->second;
		//
		if( !Rule1(posx, posy) || !Rule2(posx, posy) )
		{
			return true;
		}
		//
		if( !Rule3(posx, posy) )
		{
			return true;
		}
		//
		m_positionList.erase( it );
	}

	return false; 
}
//
bool Analyzer::Rule3( int x, int y )
{
	for ( PositionItr it1 = m_valueList.begin(), it2 = m_valueList.end(); it1 != it2; ++it1 )
	{
		int posx = it1->first;
		int posy = it1->second;
		//
		if( max(abs(posx - x), abs(posy - y)) > 2 )
		{
			continue;
		}
		//
		int val1 = temp.GetMineCount( x, y );
		int val2 = temp.GetMineCount( posx, posy );
		int km1 = temp.KnownMines( x, y );
		int km2 = temp.KnownMines( posx, posy );
		//
		PositionList ul1 = temp.UnKnownLocations( x, y );
		PositionList ul2 = temp.UnKnownLocations( posx, posy );
		//
		PositionList diff1; SetDifference( ul1, ul2, diff1 );
		PositionList diff2; SetDifference( ul2, ul1, diff2 );
		//
		int lhs = val1 - val2 + km2 - km1;
		//
		if( lhs > static_cast<int>(diff1.size()) )
		{
			return false;
		}
		//
		if( -lhs > static_cast<int>(diff2.size()) )
		{
			return false;
		}
		//
		if( lhs == static_cast<int>(diff1.size()) )
		{
			for( PositionItr it1 = diff1.begin(), it2 = diff1.end(); it1 != it2; ++it1 )
			{
				posx = it1->first;
				posy = it1->second;
				//
				temp.MarkAsMine( posx, posy );
				this->ListNeighbors( posx, posy );
			}
			//
			for( PositionItr it1 = diff2.begin(), it2 = diff2.end(); it1 != it2; ++it1 )
			{
				posx = it1->first;
				posy = it1->second;
				//
				temp.MarkAsSafe( posx, posy );
				this->ListNeighbors( posx, posy );
			}
		}
		else if ( -lhs == static_cast<int>(diff2.size()) )
		{
			for( PositionItr it1 = diff1.begin(), it2 = diff1.end(); it1 != it2; ++it1 )
			{
				posx = it1->first;
				posy = it1->second;
				//
				temp.MarkAsSafe( posx, posy );
				this->ListNeighbors( posx, posy );
			}
			//
			for( PositionItr it1 = diff2.begin(), it2 = diff2.end(); it1 != it2; ++it1 )
			{
				posx = it1->first;
				posy = it1->second;
				//
				temp.MarkAsMine( posx, posy );
				this->ListNeighbors( posx, posy );
			}
		}
	}
	//
	return true;
}
//
bool Analyzer::Rule2( int x, int y )
{
	int val = temp.GetMineCount( x, y );
	int km = temp.KnownMines( x, y );
	PositionList uls = temp.UnKnownLocations( x, y );
	//
	//
	if( km > val )
	{
		return false;
	}
	//
	if( val == km )
	{
		int posx, posy;
		for( PositionItr it1 = uls.begin(), it2 = uls.end(); it1 != it2; ++it1 )
		{
			posx = it1->first;
			posy = it1->second;
			//
			temp.MarkAsSafe( posx, posy );
			ListNeighbors( posx, posy );
		}
	}
	//
	return true;
}
//
bool Analyzer::Rule1( int x, int y )
{
	int val = temp.GetMineCount( x, y );
	int km = temp.KnownMines( x, y );
	PositionList uls = temp.UnKnownLocations( x, y );
	int sizeul = uls.size();
	//
	if( val > km + sizeul )
	{
		return false;
	}
	//
	if( val - km == sizeul )
	{
		int posx, posy;
		for( PositionItr it1 = uls.begin(), it2 = uls.end(); it1 != it2; ++it1 )
		{
			posx = it1->first;
			posy = it1->second;
			//
			temp.MarkAsMine( posx, posy );
			ListNeighbors( posx, posy );
		}
	}
	//
	return true;
}
//
void Analyzer::SetDifference( PositionList& set1, PositionList& set2, PositionList& result )
{
	result = set1;
	//
	for( PositionItr it1 = set2.begin(), it2 = set2.end(); it1 != it2; ++it1 )
	{
		result.erase( *it1 );
	}
}
//
const int l_neighbors[8][2] = { {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1} };
//
void Analyzer::ListNeighbors( int x, int y )
{
	for( unsigned int i = 0; i < 8; ++i )
	{
		int xn = x + l_neighbors[i][0];
		int yn = y + l_neighbors[i][1];
		//
		if( temp.CheckIsInside(xn, yn) && temp.IsHasValue(xn, yn) )
		{
			m_positionList.insert( Position(xn, yn) );
		}
	}
}
//
void Analyzer::PopulateValues()
{
	for( int i = 0; i < temp.GetMaxX(); ++i )
	{
		for( int j = 0; j < temp.GetMaxY(); ++j )
		{
			if( temp.CheckIsInside(i, j) && temp.IsHasValue(i, j) )
			{
				m_valueList.insert( Position(i, j) );
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
Solver::Solver(MSfieldPart2 & field) : Analyzer(field)
{
	//
}
//
Solver::~Solver()
{

}
//
void Solver::Solve()
{
	for( int i = 0 ; i < field.GetMaxX(); ++i )
	{
		for( int j = 0; j < field.GetMaxY(); ++j )
		{
			if( field.CheckIsInside(i, j) && !field.IsVisited(i, j) )
			{
				m_unknowns.insert( Position(i, j) );
			}
		}
	}
	//
	bool progress;
	PositionItr it;
	while( !m_unknowns.empty() )
	{
		progress = false;
		it = m_unknowns.begin(); 
		while ( it != m_unknowns.end() )
		{
			int x = it->first;
			int y = it->second;
			//
			if( this->IsMine(x, y) )
			{
				field.MarkAsMine( x, y );
				it = m_unknowns.erase( it );
				progress = true;
			}
			else if( this->IsSafe(x, y) )
			{
				field.OpenLocation( x, y );
				it = m_unknowns.erase( it );
				progress = true;
			}
			else
			{
				++it;
			}
		}
		//
		if( !progress )
		{
			it = m_unknowns.begin();
			if( field.OpenLocation( it->first, it->second ) < 0 )
			{
				field.MarkAsMine( it->first, it->second );
			}
			m_unknowns.erase( it );
		}
	}
}