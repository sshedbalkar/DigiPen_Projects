#include <ctime> //for random seed
#include <cmath> 
#include <iostream>
#include "agent.h"

Agent::Agent(int random_seed)
{ 
	m_state = ST_INIT;
	m_curentNode = new Node; //root node
	//
	m_curentNode->Coord.x = 0; //initial X Coord
	m_curentNode->Coord.y = 0; //initial Y Coord
	//
	m_curentNode->Parent = 0;
	m_currentDir = NORTH;
	//
	m_tagetCoord.x = -1;
	m_tagetCoord.y = -1;
	//
	MarkAsVisited( m_curentNode->Coord );
	random_seed = random_seed + 1;
}
Agent::~Agent()
{
	//
}
Action Agent::GetAction(Percept p)
{
	if( p.dirt )
	{
		return SUCK;
	}
	if( p.bump )
	{
		MarkAsVisited( m_tagetCoord );
		return NextAction();
	}
	else
	{
		if( m_state == ST_MOVE )
		{
			if( CheckIfVisited(m_tagetCoord) )		//We are at the parent
			{
				m_curentNode = m_curentNode->Parent;
			}
			else
			{
				Node* node = CreateNode( m_curentNode );
				m_curentNode = node;
				MarkAsVisited( m_curentNode->Coord );
			}
			
			return NextAction();
		}
		else if( m_state == ST_TURN )
		{
			return MoveToTarget();
		}
		else
		{
			return NextAction();
		}
	}
}
//
Action Agent::MoveToTarget()
{
	NodeCoord* currentCoords = &( m_curentNode->Coord );
	switch ( m_currentDir )
	{
	case WEST:
		{
			if( (currentCoords->x+1 == m_tagetCoord.x) && (currentCoords->y == m_tagetCoord.y) )
			{
				//no need to turn any more move forward
				m_currentDir = WEST;
				m_state = ST_MOVE;
				return FORWARD;
			}
			if( (currentCoords->x == m_tagetCoord.x) && (currentCoords->y+1 == m_tagetCoord.y) )
			{
				//turn up
				m_currentDir = NORTH;
				m_state = ST_TURN;
				return LEFT;
			}
			if( (currentCoords->x == m_tagetCoord.x) && (currentCoords->y-1 == m_tagetCoord.y) )
			{
				//turn down
				m_currentDir = SOUTH;
				m_state = ST_TURN;
				return RIGHT;
			}
			if( (currentCoords->x-1 == m_tagetCoord.x) && (currentCoords->y == m_tagetCoord.y) )
			{
				//turn up (down would be the same)
				m_currentDir = NORTH;
				m_state = ST_TURN;
				return LEFT;
			}
		}
		break;
	case SOUTH:
		{
			if( (currentCoords->x+1 == m_tagetCoord.x) && (currentCoords->y == m_tagetCoord.y) )
			{
				//turn right
				m_currentDir = WEST;
				m_state = ST_TURN;
				return LEFT;
			}
			if( (currentCoords->x == m_tagetCoord.x) && (currentCoords->y+1 == m_tagetCoord.y) )
			{
				//turn left (right would be the same)
				m_currentDir = WEST;
				m_state = ST_TURN;
				return LEFT;
			}
			if( (currentCoords->x == m_tagetCoord.x) && (currentCoords->y-1 == m_tagetCoord.y) )
			{
				//no need to turn any more move forward
				m_currentDir = SOUTH;
				m_state = ST_MOVE;
				return FORWARD;
			}
			if( (currentCoords->x-1 == m_tagetCoord.x) && (currentCoords->y == m_tagetCoord.y) )
			{
				//turn up 
				m_currentDir = EAST;
				m_state = ST_TURN;
				return RIGHT;
			}
		}
		break;
	case EAST:
		{
			if( (currentCoords->x+1 == m_tagetCoord.x) && (currentCoords->y == m_tagetCoord.y) )
			{
				//turn up (down would be the same)
				m_currentDir = SOUTH;
				m_state = ST_TURN;
				return LEFT;
			}
			if( (currentCoords->x == m_tagetCoord.x) && (currentCoords->y+1 == m_tagetCoord.y) )
			{
				//turn up
				m_currentDir = NORTH;
				m_state = ST_TURN;
				return RIGHT;
			}
			if( (currentCoords->x == m_tagetCoord.x) && (currentCoords->y-1 == m_tagetCoord.y) )
			{
				//turn down
				m_currentDir = SOUTH;
				m_state = ST_TURN;
				return LEFT;
			}
			if( (currentCoords->x-1 == m_tagetCoord.x) && (currentCoords->y == m_tagetCoord.y) )
			{
				//no need to turn any more move forward
				m_currentDir = EAST;
				m_state = ST_MOVE;
				return FORWARD;
			}
		}
		break;
	case NORTH:
		{
			if( (currentCoords->x+1 == m_tagetCoord.x) && (currentCoords->y == m_tagetCoord.y) )
			{
				//turn right
				m_currentDir = WEST;
				m_state = ST_TURN;
				return RIGHT;
			}
			if( (currentCoords->x == m_tagetCoord.x) && (currentCoords->y+1 == m_tagetCoord.y) )
			{
				//no need to turn any more move forward
				m_currentDir = NORTH;
				m_state = ST_MOVE;
				return FORWARD;
			}
			if( (currentCoords->x == m_tagetCoord.x) && (currentCoords->y-1 == m_tagetCoord.y) )
			{
				//turn left (right would be the same)
				m_currentDir = EAST;
				m_state = ST_TURN;
				return LEFT;
			}
			if( (currentCoords->x-1 == m_tagetCoord.x) && (currentCoords->y == m_tagetCoord.y) )
			{
				//turn left
				m_currentDir = EAST;
				m_state = ST_TURN;
				return LEFT;
			}
		}
		break;
	}
	return SHUTOFF;//this will never happen
}
//
Action Agent::NextAction()
{
	if( FindUnvisitedChild() )
	{
		return MoveToTarget();
	}
	else
	{
		if( m_curentNode->Parent != 0)
		{
			m_tagetCoord = m_curentNode->Parent->Coord ;
			//
			return MoveToTarget();
		}
		else
		{
			return SHUTOFF;
		}
	}
}
//
bool Agent::FindUnvisitedChild()
{
	NodeCoord neighbor;
	//
	//check to the front
	neighbor.x = m_curentNode->Coord.x;
	neighbor.y = m_curentNode->Coord.y + 1;
	if( !CheckIfVisited(neighbor) )
	{
		m_tagetCoord = neighbor;
		return true;
	}
	//
	//check to the back
	neighbor.x = m_curentNode->Coord.x;
	neighbor.y = m_curentNode->Coord.y - 1;
	if( !CheckIfVisited(neighbor) )
	{
		m_tagetCoord = neighbor;
		return true;
	}
	//
	//check to the right
	neighbor.x = m_curentNode->Coord.x + 1;
	neighbor.y = m_curentNode->Coord.y;
	if( !CheckIfVisited(neighbor) )
	{
		m_tagetCoord = neighbor;
		return true;
	}
	//
	//check to the left
	neighbor.x = m_curentNode->Coord.x - 1;
	neighbor.y = m_curentNode->Coord.y;
	if( !CheckIfVisited(neighbor) )
	{
		m_tagetCoord = neighbor;
		return true;
	}
	//
	return false;
}
//
Node* Agent::CreateNode( Node* parentNode )
{
	Node* newNode = new Node();		//root node
	newNode->Parent = parentNode;
	newNode->Coord = m_tagetCoord;
	return newNode;
}
//
bool Agent::CheckIfVisited(NodeCoord & c)
{
	CoordsMap::iterator it = m_visitedMap.find( c );
	return it != m_visitedMap.end();
}
//
void Agent::MarkAsVisited( NodeCoord c )
{
	m_visitedMap.insert( std::pair< NodeCoord, bool >(c, true) );//mark this node as visited
}
