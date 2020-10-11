#pragma once
//
#include "DXUT\DXUT.h"
#include <list>
#include <vector>
#include "singleton.h"
#include "movement.h"
//
/////////////////////////////////////////
struct ASNode
{
	ASNode( void );
	ASNode( int &x, int &y );
	ASNode( int &x, int &y, ASNode *p );
	~ASNode( void );
	//
	int X;
	int Y;
	ASNode *parent;
	float cost;
	float totalCost;
	bool onOpen;
	bool onClosed;
	//
};
/////////////////////////////////////////
/////////////////////////////////////////
typedef std::list<ASNode*> NodeList;
typedef std::vector<ASNode*> WayPoints;
class AStar: public Singleton< AStar >
{
public:
	AStar( void );
	~AStar ( void );
	void setMapId( int mid );
	bool calculatePath( int &startX, int &startY, int &goalX, int &goalY, bool newRequest, WaypointList &way );
	const WaypointList* getPathList();
	void stepMode(bool mode);
	void changeHeuristic( int h );
	void setWeight( float f );
	void setStraightline( bool b );
	void setRubberband( bool b );
	void setSmoothing( bool b );
	bool getSingleStep();
	//
private:
	ASNode* getCheapestNode( NodeList::iterator &it );
	ASNode* findInOpen( int &x, int &y, NodeList::iterator &it );
	ASNode* findInClosed( int &x, int &y, NodeList::iterator &it );
	float ( *heuristic )( int&, int&, int&, int& );
	void clear();
	int getNeighbors( int *arr, int &x, int &y, int &width );
	bool straightlineOptimization( int &startX, int &startY, int &goalX, int &goalY );
	void rubberband( WayPoints &nodes );
	void smoothen( WaypointList &nodes );
	//
private:
	bool m_completed;
	int m_currMapIndex;
	WaypointList* m_waypoints;
	NodeList* m_openList;
	NodeList* m_closedList;
	ASNode* m_currNode;
	float m_costG;
	//
	float m_weight;
	bool m_stepMode;
	bool m_straightline;
	bool m_rubberband;
	bool m_smooth;
};
