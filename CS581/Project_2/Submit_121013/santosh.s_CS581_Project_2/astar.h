#ifndef ASTAR
#define ASTAR
//
#include <vector>
#include <iostream>
#include <string>
#include <map>
//
class ASNode
{
public:
	ASNode()
		:parent( NULL ),
		costG( 999999999999.99 ),
		costTotal( 999999999999.99 ),
		onClosed( false )
	{
		//
	}
	//
	ASNode( size_t n, ASNode* p )
		:id( n ),
		parent( p ),
		costG( 999999999999.99 ),
		costTotal( 999999999999.99 ),
		onClosed( false )
	{
		//
	}
	//
	~ASNode()
	{
		//
	}
	//
	ASNode*		parent;		// parent of this node
	float		costG;		// total cost of this node to get it from the start node
	float		costTotal;	// sum of costG and heuristic
	bool		onClosed;	// if on closed list
	size_t		id;			// id associated with this node
};
//
template <typename GraphType, typename Heuristic > 
class Astar 
{
private:
	typedef std::vector< ASNode* >					NodeList;
	typedef std::map< size_t, ASNode* >				NodeMap;
	//
public:
	Astar(const GraphType & _graph) : graph(_graph) {}
	std::vector<typename GraphType::Edge> search(size_t start_id, size_t goal_id) const
	{
		std::vector< typename GraphType::Edge >		solution;
		float										costT, costG, costH;
		ASNode										*n, *curr;
		Heuristic									h;
		size_t										id2;
		NodeMap										closedList;
		NodeList									openList;
		//
		typename GraphType::Vertex goal = graph.GetVertex( goal_id );
		typename GraphType::Vertex v = graph.GetVertex( start_id );
		//
		curr = new ASNode( start_id, NULL );
		v = graph.GetVertex( start_id );
		costH = h( graph, v, goal) ;
		costT = costH;		// total cost of the start node = costH
		curr->costG = 0.0f;
		curr->costTotal = costT;
		openList.push_back( curr );	// push the start node into the open list
		//
		while( (curr = GetCheapestNode(openList)) != NULL )
		{
			if( curr->id == goal_id )	// we are at goal
			{
				std::cout<<closedList.size()<<std::endl;
				while( curr->parent != NULL )	// populate the solution vector
				{
					solution.push_back( typename GraphType::Edge(curr->id,curr->parent->id,curr->costG - curr->parent->costG) );
					curr = curr->parent;
				}
				//std::reverse( solution.begin(), solution.end() );
				return solution;
			}
			else
			{
				// push the node on to the closed list and remove it from the open list
				curr->onClosed = true;
				closedList[ curr->id ] = curr;
				openList.pop_back();
				//
				std::vector<typename GraphType::Edge> outedges = graph.GetOutEdges( graph.GetVertex(curr->id) );
				size_t outedges_size = outedges.size();
				//
				// do the following on all neighbors of this node
				for( size_t i = 0; i < outedges_size; ++i )
				{
					id2 = outedges[i].GetID2();
					v = graph.GetVertex( id2 );
					//
					costG = outedges[i].GetWeight() + curr->costG;	//cost is the cost of this node + it's parent's cost
					costH = h( graph, v, goal );
					costT = costG + costH;
					//
					if( (n = FindInMap(&closedList, id2) ) == NULL )
					{
						n = new ASNode( id2, curr );
						n->costG = costG;
						n->costTotal = costT;
						openList.push_back(n);
					}
					else
					{
						if( costT < n->costTotal )	//if it's on closed list but the new cost we found is cheaper
						{
							n->costG = costG;
							n->costTotal = costT;
							n->parent = curr;
							n->onClosed = false;
							//
							openList.push_back(n);
							closedList.erase( id2 );
						}
					}
				}
			}
		}
		// clear the open and closed lists
		//ClearMap( &closedList );
		//ClearList( &openList );
		//
		return solution;
	}
	//
private:
	void ClearList( NodeList* list ) const
	{
		for( NodeList::iterator it1 = list->begin(), it2 = list->end(); it1 != it2; ++it1 )
		{
			delete *it1;
		}
	}
	void ClearMap( NodeMap* map ) const
	{
		for( NodeMap::iterator it1 = map->begin(), it2 = map->end(); it1 != it2; ++it1 )
		{
			delete it1->second;
		}
	}
	//
	ASNode* GetCheapestNode( NodeList& list ) const
	{
		if( list.empty() ){ return NULL; }
		int index = -1;
		ASNode* cheap = list.back();
		float cost = cheap->costTotal;
		//
		for ( std::vector<ASNode*>::iterator it1 = list.begin(), it2 = list.end(), it3 = it1; it1 != it2; ++it1 )
		{
			if( (*it1)->costTotal < cost )
			{
				index = it1 - it3;
				cheap = *it1;
				cost = cheap->costTotal;
			}
		}
		//
		// switch this node with the last node so that it can be popped
		if ( index != -1)
		{
			list[ index ] = list.back();
			list.back() = cheap;
		}
		return cheap;
	}
	//
	ASNode* FindInMap( NodeMap* list, size_t id ) const
	{
		NodeMap::iterator it = list->find( id );
		if( it != list->end() )
		{
			return it->second;
		}
		return NULL;
	}
	//
private:
	const GraphType&			graph;
	//
};
#endif
