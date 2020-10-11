#ifndef ASTAR
#define ASTAR

#include <vector>
#include <iostream>
#include <string>

template <typename GraphType, typename Heuristic > 
class Astar {
	public:
		Astar(const GraphType & _graph) : graph(_graph) {}
		std::vector<typename GraphType::Edge> search(size_t start_id, size_t goal_id) const {
			//code in this method doesn't make any sense
			//just to show what you can do here

			//creating a solution
			std::vector<typename GraphType::Edge> solution;
			solution.push_back( typename GraphType::Edge(0,2,1) );
			solution.push_back( typename GraphType::Edge(2,3,1) );

			//working with heuristic
			Heuristic h;
			std::cout << "Testing heuristic\n";

			typename GraphType::Vertex goal = graph.GetVertex(goal_id);
			typename GraphType::Vertex v = graph.GetVertex(start_id);
			std::cout << "Heuristic at start " << 
				h( graph,v,goal) << std::endl;
			v = graph.GetVertex(1);
			std::cout << "Heuristic at 1 " << 
				h( graph,v,goal) << std::endl;

			//working neighbors
			std::cout << "Testing neighbors\n";
			std::vector<typename GraphType::Edge> outedges = graph.GetOutEdges(graph.GetVertex(goal_id));
			size_t outedges_size = outedges.size();
			for (size_t i = 0; i < outedges_size; ++i) {
				std::cout << "goal has a neighbor " << outedges[i].GetID2() << " at distance " << outedges[i].GetWeight() << std::endl;
			}
			return solution;
		}
	private:
		const GraphType & graph;
};

#endif
