#ifndef AGENT_H
#define AGENT_H
#include "definitions.h"
#include <map>
////////////////////////////////
struct NodeCoord
{
	int x;
	int y;
};
//
struct Node
{
	NodeCoord	Coord;
	Node*		Parent;
};
//
// Compares 2 coords
class FnCompCoords
{
public:
	bool operator()( NodeCoord first, NodeCoord second )
	{
		if( first.x > second.x )
		{
			return true;
		}
		else if( first.x < second.x )
		{
			return false;
		}
		else
		{
			return( first.y - second.y ) > 0;
		}
	}
};
//
class Agent
{
public:
				Agent( int random_seed );
				~Agent();
	Action		GetAction( Percept p );
	//
private:
	Node*		CreateNode( Node* parentNode );
	void		MarkAsVisited( NodeCoord c );
	bool		CheckIfVisited( NodeCoord& c );
	bool		FindUnvisitedChild();
	Action		MoveToTarget();
	Action		NextAction();
	//
private:
	enum State
	{
		ST_INIT,
		ST_MOVE,
		ST_TURN
	};
	//
	typedef std::map<NodeCoord, bool, FnCompCoords> CoordsMap;
	State		m_state;
	Node*		m_curentNode;
	NodeCoord	m_tagetCoord;
	CoordsMap	m_visitedMap;
	Heading		m_currentDir;
};
//
#endif
