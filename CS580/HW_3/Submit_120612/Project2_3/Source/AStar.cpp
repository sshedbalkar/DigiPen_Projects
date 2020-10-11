#include "DXUT.h"
#include "DXUT\DXUT.h"
#include <iostream>
#include "AStar.h"
#include "time.h"
#include "terrain.h"
#include "global.h"
//
#define TIME_SLICE 0.002f
#define SQRT_2 1.414213562373f
//
////////////////////////////////////////////////////
ASNode::ASNode( void )
	:X( -1 ),
	Y( -1 ),
	parent( NULL ),
	cost( FLT_MAX ),
	totalCost( FLT_MAX ),
	onOpen( false ),
	onClosed ( false )
{
	//
}
ASNode::ASNode( int &x, int &y )
	:X( x ),
	Y( y ),
	parent( NULL ),
	cost( FLT_MAX ),
	totalCost( FLT_MAX ),
	onOpen( false ),
	onClosed ( false )
{
	//
}
ASNode::ASNode( int &x, int &y, ASNode *p )
	:X( x ),
	Y( y ),
	parent( p ),
	cost( FLT_MAX ),
	totalCost( FLT_MAX ),
	onOpen( false ),
	onClosed ( false )
{
	//
}
ASNode::~ASNode( void )
{
	//
}
////////////////////////////////////////////////////
bool fncomp ( int lhs, int rhs )
{
	return lhs < rhs;
}
bool( *fn_pt )( int, int ) = fncomp;
float distChebyshev( int &currX, int &currY, int &goalX, int &goalY );
float distManhattan( int &currX, int &currY, int &goalX, int &goalY );
float distEuclidean( int &currX, int &currY, int &goalX, int &goalY );
float distOctile( int &currX, int &currY, int &goalX, int &goalY );
void normalize( D3DXVECTOR3 &v1, D3DXVECTOR3 &vRes );
float squareDist( D3DXVECTOR3 &v );
/////////////////////////////////////////////////////
AStar::AStar( void )
	:m_completed( true ), 
	m_currMapIndex( -1 ), 
	m_waypoints( new WaypointList() ),
	m_stepMode( true ),
	m_openList( new NodeList() ),
	m_closedList( new NodeList() ),
	m_costG( 0.0f ),
	m_weight( 1.0f ),
	m_straightline( false ),
	m_rubberband( false ),
	m_smooth( false )
{
	changeHeuristic( 0 );
}

AStar::~AStar( void )
{
	delete m_waypoints;
	delete m_openList;
	delete m_closedList;
}

void AStar::setMapId( int mid )
{
	m_currMapIndex = mid;
}

bool AStar::calculatePath( int &startX, int &startY, int &goalX, int &goalY, bool newRequest )
{
	//std::cout<<"Time: "<<g_time.GetCurTime()<<", ET: "<<g_time.GetElapsedTime()<<"\n";
	//std::cout<<"Start: "<<startX<<", "<<startY<<" Goal: "<<goalX<<", "<<goalY<<", new: " <<newRequest<<"\n";
	if( m_straightline && straightlineOptimization( startX, startY, goalX, goalY ) )
	{
		m_waypoints->clear();
		m_waypoints->push_back( g_terrain.GetCoordinates( goalX, goalY ) );
		clear();
		m_completed = true;
		return true;
	}
	else
	{
		ASNode *curr = 0, *n;
		if( m_completed )
		{
			m_completed = false;
			curr = new ASNode( startX, startY );
			curr->cost = 0.0f;
			curr->totalCost = m_weight * heuristic( startX, startY, goalX, goalY );
			curr->onOpen = true;
			g_terrain.SetColor( startX, startY, DEBUG_COLOR_BLUE );
			//
			m_openList->push_back( curr );
		}
		//
		int x, y, i, j, width = g_terrain.GetWidth();
		float costT = 0.0f, costE;
		int arr[16];
		int neighbors, count;
		NodeList::iterator it1, it2, it;
		while( (curr = getCheapestNode( it )) != NULL )
		{
			m_openList->erase( it );
			x = curr->X;
			y = curr->Y;
			if( ( x == goalX ) && ( y == goalY ) )
			{
				WayPoints nodes;
				while( curr->parent != NULL )
				{
					nodes.push_back( curr );
					curr = curr->parent;
				}
				//
				if( m_rubberband ) rubberband( nodes );
				//
				m_waypoints->clear();
				//m_waypoints->push_back( g_terrain.GetCoordinates( startX, startY ) );
				while( !nodes.empty() )
				{
					curr = nodes.back();
					m_waypoints->push_back( g_terrain.GetCoordinates( curr->X, curr->Y ) );
					//std::cout<<"x: "<<curr->X<<", y: "<<curr->Y<<"\n";
					nodes.pop_back();
				}
				if( m_smooth ) smoothen( *m_waypoints );
				clear();
				m_completed = true;
				return true;
			}
			else
			{
				neighbors = getNeighbors( arr, x, y, width );
				count = -1;
				m_costG = curr->cost;
				while( neighbors > 0 )
				{
					i = arr[++count];
					j = arr[++count];
					--neighbors;
					//
					costE = ( i == x || j == y ) ? 1.0f : SQRT_2;
					costT = m_costG + ( m_weight * heuristic( i, j, goalX, goalY ) );
					//
					if( ( n = findInOpen( i, j, it1 )) == NULL && ( n = findInClosed( i, j, it2 )) == NULL)
					{
						n = new ASNode( i, j, curr );
						n->cost = m_costG + costE;
						n->totalCost = costT;
						n->onOpen = true;
						m_openList->push_back( n );
						g_terrain.SetColor( i, j, DEBUG_COLOR_BLUE );
					}
					else
					{
						if( costT < n->totalCost )
						{
							if( n->onClosed )
							{
								m_closedList->erase( it2 );
								m_openList->push_back( n );
							}
							n->cost = m_costG + costE;
							n->totalCost = costT;
							n->parent = curr;
							n->onOpen = true;
							n->onClosed = false;
							g_terrain.SetColor( i, j, DEBUG_COLOR_BLUE );
						}
					}
				}
				curr->onOpen = false;
				curr->onClosed = true;
				m_closedList->push_back( curr );
				g_terrain.SetColor( x, y, DEBUG_COLOR_YELLOW );
				//
				if( m_stepMode ) break;
			}
		}
		return false;
	}
}

void AStar::rubberband( WayPoints &nodes )
{
	int s;
	if( ( s = nodes.size() ) < 3 ) return;
	//
	WayPoints res;
	int first = 0, second = 1, third = 2;
	ASNode *n1, *n2;
	//
	do
	{
		n1 = nodes.at( first );
		n2 = nodes.at( third );
		if( straightlineOptimization( n1->X, n1->Y, n2->X, n2->Y ) )
		{
			second = third;
			++third;
		}
		else
		{
			res.push_back( nodes.at( first ) );
			first = second;
			second = third;
			++third;
		}
	}
	while( third < s );
	res.push_back( nodes.at( first ) );
	res.push_back( nodes.at( second ) );
	nodes.swap( res );
}

void AStar::smoothen( WaypointList &nodes )
{
	D3DXVECTOR3 unit = g_terrain.GetCoordinates( 0, 0 ), diff = g_terrain.GetCoordinates( 0, 1 ), v1;
	diff -= unit;
	float unitL = 1.5f * sqrt( diff.x * diff.x + diff.z * diff.z ), len;
	int times, i, s;
	WaypointList res, res2;
	WaypointList::iterator first = nodes.begin(), second = first, last = nodes.end(), third, fourth;
	//
	while( ++second != last )
	{
		res.push_back( ( *first ) );
		diff = ( *second ) - ( *first );
		len = squareDist( diff );
		if( len > unitL )
		{
			times = ( int )( len / unitL );
			++times;
			normalize( diff, unit );
			unit *= unitL;
			v1 = ( *first );
			for( i = 1; i < times; ++i )
			{
				v1 += unit;
				res.push_back( v1 );
			}
		}
		++first;
	}
	res.push_back( ( *first ) );
	//
	if( ( s = res.size() ) > 2 )
	{
		first = res.begin();
		second = first;
		++second;
		third = second;
		++third;
		fourth = third;
		last = res.end();
		--last;
		--last;
		//
		D3DXVECTOR3 p1, p2, p3;
		//
		D3DXVec3CatmullRom( &p1, &( *first ), &( *first ), &( *second ), &( *third ), 0.25f );
		D3DXVec3CatmullRom( &p2, &( *first ), &( *first ), &( *second ), &( *third ), 0.5f );
		D3DXVec3CatmullRom( &p3, &( *first ), &( *first ), &( *second ), &( *third ), 0.75f );
		//
		res2.push_back( ( *first ) );
		res2.push_back( p1 );
		res2.push_back( p2 );
		res2.push_back( p3 );
		//
		if( s > 3 )
		{
			fourth = third;
			++fourth;
			while( second != last )
			{
				D3DXVec3CatmullRom( &p1, &( *first ), &( *second ), &( *third ), &( *fourth ), 0.25f );
				D3DXVec3CatmullRom( &p2, &( *first ), &( *second ), &( *third ), &( *fourth ), 0.5f );
				D3DXVec3CatmullRom( &p3, &( *first ), &( *second ), &( *third ), &( *fourth ), 0.75f );
				//
				res2.push_back( ( *second ) );
				res2.push_back( p1 );
				res2.push_back( p2 );
				res2.push_back( p3 );
				//
				++first;
				++second;
				++third;
				++fourth;
			}
		}
		D3DXVec3CatmullRom( &p1, &( *first ), &( *second ), &( *third ), &( *third ), 0.25f );
		D3DXVec3CatmullRom( &p2, &( *first ), &( *second ), &( *third ), &( *third ), 0.5f );
		D3DXVec3CatmullRom( &p3, &( *first ), &( *second ), &( *third ), &( *third ), 0.75f );
		//
		res2.push_back( ( *second ) );
		res2.push_back( p1 );
		res2.push_back( p2 );
		res2.push_back( p3 );
		res2.push_back( ( *third ) );
	}
	//
	nodes.swap( res2 );
}

void normalize( D3DXVECTOR3 &v1, D3DXVECTOR3 &vRes )
{
	float d = sqrt( v1.x * v1.x + v1.z * v1.z );
	vRes.x = v1.x / d;
	vRes.y = 0.0f;
	vRes.z = v1.z / d;
}

float squareDist( D3DXVECTOR3 &v )
{
	return sqrt( v.x * v.x + v.z * v.z );
	//return v.x * v.x + v.z * v.z;
}

bool AStar::straightlineOptimization( int &startX, int &startY, int &goalX, int &goalY )
{
	int minI = min( startX, goalX );
	int maxI = max( startX, goalX );
	int minJ = min( startY, goalY );
	int maxJ = max( startY, goalY );
	int i, j;
	//
	for( i = minI; i <= maxI; ++i )
	{
		for( j = minJ; j <= maxJ; ++j )
		{
			if( g_terrain.IsWall( i, j ) ) { return false; };
		}
	}
	return true;
}

int AStar::getNeighbors( int *arr, int &x, int &y, int &width )
{
	int c = 0, d = -1, i, j;
	int minI = max( 0, x - 1 );
	int maxI = min( width - 1, x + 1 ) + 1;
	int minJ = max( 0, y - 1 );
	int maxJ = min( width - 1, y + 1 ) + 1;
	//
	for( i = minI; i < maxI; ++i )
	{
		for( j = minJ; j < maxJ; ++j )
		{
			if( i == x && j == y ) continue;
			if( g_terrain.IsWall( i, j ) ) continue;
			if( i == x || j == y )
			{
				//
			}
			else
			{
				if( g_terrain.IsWall( x, j ) ||  g_terrain.IsWall( i, y ) ) continue;
			}
			*(arr + ++d) = i;
			*(arr + ++d) = j;
			++c;
			//std::cout<<"c: "<<c<<", i: "<<i<<", j: "<<j<<"\n";
		}
	}
	//
	return c;
}

float distChebyshev( int &currX, int &currY, int &goalX, int &goalY )
{
	int dx = currX - goalX;
	int dy = currY - goalY;
	dx = ( dx < 0 ) ? -dx : dx;
	dy = ( dy < 0 ) ? -dy : dy;
	//
	return ( float ) max( dx, dy );
}

float distManhattan( int &currX, int &currY, int &goalX, int &goalY )
{
	int dx = currX - goalX;
	int dy = currY - goalY;
	dx = ( dx < 0 ) ? -dx : dx;
	dy = ( dy < 0 ) ? -dy : dy;
	//
	return ( float ) ( dx + dy );
}

float distEuclidean( int &currX, int &currY, int &goalX, int &goalY )
{
	int dx = currX - goalX;
	int dy = currY - goalY;
	dx = ( dx < 0 ) ? -dx : dx;
	dy = ( dy < 0 ) ? -dy : dy;
	//
	dx *= dx;
	dy *= dy;
	//
	return sqrtf( ( float ) ( dx + dy ) );
}

float distOctile( int &currX, int &currY, int &goalX, int &goalY )
{
	int dx = currX - goalX;
	int dy = currY - goalY;
	dx = ( dx < 0 ) ? -dx : dx;
	dy = ( dy < 0 ) ? -dy : dy;
	//
	float mn = ( float ) min( dx, dy );
	float r = mn * SQRT_2;
	r += ( float ) max ( dx, dy );
	r -= mn;
	//
	return r;

}

void AStar::changeHeuristic( int h )
{
	std::cout<<"Heuristic function: "<<h<<"\n";
	switch( h )
	{
	case 0:
		heuristic = distEuclidean;
		break;
	case 1:
		heuristic = distOctile;
		break;
	case 2:
		heuristic = distChebyshev;
		break;
	case 3:
		heuristic = distManhattan;
		break;
	}
}

ASNode* AStar::findInOpen( int &x, int &y, NodeList::iterator &it )
{
	ASNode *n;
	for( NodeList::iterator it1 = m_openList->begin(), it2 = m_openList->end(); it1 != it2; ++it1 )
	{
		if( (n = ( *it1 ))->X == x && n->Y == y )
		{
			it = it1;
			return n;
		}
	}
	return NULL;
}

ASNode* AStar::findInClosed( int &x, int &y, NodeList::iterator &it )
{
	ASNode *n;
	for( NodeList::iterator it1 = m_closedList->begin(), it2 = m_closedList->end(); it1 != it2; ++it1 )
	{
		if( (n = ( *it1 ))->X == x && n->Y == y )
		{
			it = it1;
			return n;
		}
	}
	return NULL;
}

ASNode* AStar::getCheapestNode( NodeList::iterator &it )
{
	if( m_openList->empty() ){ return NULL; }
	ASNode *cheap = m_openList->front(), *n;
	float cost = cheap->totalCost;
	//
	NodeList::iterator it1 = m_openList->begin(), it2 = m_openList->end();
	it = it1;
	while( it1 != it2 )
	{
		if( ( n = ( *it1 ))->totalCost < cost )
		{
			cheap = n;
			cost = cheap->totalCost;
			it = it1;
		}
		++it1;
	}
	//
	return cheap;
}

const WaypointList* AStar::getPathList()
{
	return m_waypoints;
}

void AStar::setWeight(float f)
{
	m_weight = f;
	std::cout<<"Weight = "<<f<<"\n";
}

void AStar::stepMode( bool mode)
{
	m_stepMode = mode;
	std::cout<<"Single step: " <<mode<<"\n";
}

bool AStar::getSingleStep()
{
	return m_stepMode;
}

void AStar::clear()
{
	ASNode *n;
	while( !m_openList->empty() )
	{
		n = m_openList->front();
		delete n;
		m_openList->pop_front();
	}
	while( !m_closedList->empty() )
	{
		n = m_closedList->front();
		delete n;
		m_closedList->pop_front();
	}
}

void AStar::setStraightline( bool b)
{
	m_straightline = b;
	std::cout<<"Straightline: " <<b<<"\n";
}

void AStar::setRubberband( bool b)
{
	m_rubberband = b;
	std::cout<<"Rubberband: " <<b<<"\n";
}

void AStar::setSmoothing( bool b)
{
	m_smooth = b;
	std::cout<<"Smoothing: " <<b<<"\n";
}