#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <ctime>
#include "graph.h"
#include "astar.h"

    struct VertexTest {
        VertexTest (size_t _id=0, float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) 
            : id(_id),x(_x),y(_y),z(_z)   {  
                //std::cout << "VertexTest("<<id<<") called\n";
            }
        size_t ID() const { return id; }
        float X() const { return x; }
        float Y() const { return y; }
        float Z() const { return z; }
        bool operator< ( const VertexTest& rhs) const {
            return id<rhs.id;
        }
        private:
        size_t id;
        float x,y,z;
    };

struct EdgeTest {
    EdgeTest(size_t id1, size_t id2, float weight) : 
        id1(id1),id2(id2),weight(weight) {}
    size_t GetID1() const { return id1; }
    size_t GetID2() const { return id2; }
    float  GetWeight() const { return weight; }
    private:
    size_t id1;
    size_t id2;
    float weight;
};

template <typename GraphType>
struct TestHeuristic {
    typedef float ReturnType;
    float operator () (const GraphType &, 
            const typename GraphType::Vertex & v1,  
            const typename GraphType::Vertex &) const 
    {
        float h = 0.0f;
        switch ( v1.ID() ) {
            case 1: h = 2.0f; break;
            case 2: h = 1.0f; break;
        };
        return h;
    }
};

template <typename GraphType>
struct TestHeuristic2 {
    typedef float ReturnType;
    float operator () (const GraphType &, 
            const typename GraphType::Vertex & v1,  
            const typename GraphType::Vertex &) const {
        float h = 0.0f;
        switch ( v1.ID() ) {
            case 1: h = 10.0f; break;
        };
        return h;
    }
};

//heuristic to use when working with euclidian coordinates 
template <typename GraphType>
struct HeuristicEuclidian {
    typedef float ReturnType;
    ReturnType operator () (const GraphType & graph, 
            const typename GraphType::Vertex & v1,  
            const typename GraphType::Vertex & v2) const {
        float lat1 = v1.X();
        float lng1 = v1.Y();
        float lat2 = v2.X();
        float lng2 = v2.Y();
        std::cout << "Heuristic " << lat1 << " " << lng1 << " "
            << lat2 << " " << lng2 << " "<< std::endl;
        return std::sqrt((lat1-lat2)*(lat1-lat2)+(lng1-lng2)*(lng1-lng2));
    }
};

struct EdgeGrid {
    EdgeGrid(size_t id1, size_t id2, float weight) : id1(id1),id2(id2),weight(weight) {}
    size_t GetID1() const { return id1; }
    size_t GetID2() const { return id2; }
    float  GetWeight() const { return weight; }
    private:
    size_t id1;
    size_t id2;
    float weight; //miles
};

struct VertexGrid {
    VertexGrid (size_t _id=0, float _x=0.0f, float _y=0.0f, float _z = 0.0f) : id(_id),x(_x),y(_y),z(_z)   {  }
    size_t ID() const { return id; }
    bool operator< ( const VertexGrid& rhs) const { return id<rhs.id; }
    float X() const { return x; }
    float Y() const { return y; }
    float Z() const { return z; }
    private:
    size_t id;
    float x,y,z;
};

template <typename GraphType>
struct ManhattanHeuristic {
    typedef float ReturnType;
    ReturnType operator () (const GraphType &, 
            const typename GraphType::Vertex & v1,  
            const typename GraphType::Vertex & v2) const {
        //Manhattan distance
        return std::abs (v1.X()-v2.X()) + std::abs (v1.Y()-v2.Y());
    }
};

////////////////////////////////////////////////////////////
void test0() {
    typedef Graph<VertexTest,EdgeTest> Graph;
    Graph graph;
    {
        /*       (B)
                 /     \ 
                 1       1
                 /         \
                 (A)-1-(C)-2-(E)--1--(F)
                 \         /
                 1       3
                 \     /
                 (D)

                 h(B)=2
                 h(C)=1
                 h(all other nodes)=0
         */
    }

    enum V {A,B,C,D,E,F};

    //make sure algorithm works with all permutation of edges AB AC AD
    //that is correctness doesn't depend on tie-breaking
    std::vector<int> order;
    order.push_back(1);
    order.push_back(2);
    order.push_back(3);
    float worst_solution = 0;
    //long worst_time = 0;
    do {
        //std::cout << order[0] << " "  << order[1] << " " << order[2] << std::endl;
        {
            graph.InsertVertex( Graph::Vertex(A) );
            graph.InsertVertex( Graph::Vertex(B) );
            graph.InsertVertex( Graph::Vertex(C) );
            graph.InsertVertex( Graph::Vertex(D) );
            graph.InsertVertex( Graph::Vertex(E) );
            graph.InsertVertex( Graph::Vertex(F) );
            graph.InsertEdge( Graph::Edge(A,static_cast<V>(order[0]),1));
            graph.InsertEdge( Graph::Edge(static_cast<V>(order[0]),A,1));
            graph.InsertEdge( Graph::Edge(A,static_cast<V>(order[1]),1));
            graph.InsertEdge( Graph::Edge(static_cast<V>(order[1]),A,1));
            graph.InsertEdge( Graph::Edge(A,static_cast<V>(order[2]),1));
            graph.InsertEdge( Graph::Edge(static_cast<V>(order[2]),A,1));
            graph.InsertEdge( Graph::Edge(B,E,1) );
            graph.InsertEdge( Graph::Edge(E,B,1) );
            graph.InsertEdge( Graph::Edge(C,E,2) );
            graph.InsertEdge( Graph::Edge(E,C,2) );
            graph.InsertEdge( Graph::Edge(D,E,3) );
            graph.InsertEdge( Graph::Edge(E,D,3) );
            graph.InsertEdge( Graph::Edge(E,F,1) );
            graph.InsertEdge( Graph::Edge(F,E,1) );
        }

        Astar<Graph,TestHeuristic<Graph> > astar(graph);
        //clock_t start = std::clock();
        std::vector<EdgeTest> solution = astar.search(A,F);
        //clock_t finish = std::clock();
        //if (worst_time< (finish-start)) worst_time = finish-start;
        float cost = 0.0f;
        std::vector<Graph::Edge>::reverse_iterator rit;
        for(rit=solution.rbegin();rit!=solution.rend();++rit) {
            cost += rit->GetWeight();
        }
        if (worst_solution < cost) worst_solution = cost;
    } while (next_permutation(order.begin(),order.end()));
    //std::cout << "search time " << worst_time << std::endl;
    std::cout << "solution cost " << worst_solution << std::endl;
}

void test1() {
    /*      (B)
            /     \
            2       1
            /         \
            (A)         (D)-1-(E)--10--(F)
            \         /
            1       4
            \     /
            (C)
            h(B)=10
            h(all other nodes)=0 
     */
    typedef Graph<VertexTest,EdgeTest> Graph;
    Graph graph;

    enum V {A,B,C,D,E,F};

    {
        graph.InsertVertex( Graph::Vertex(A) );
        graph.InsertVertex( Graph::Vertex(B) );
        graph.InsertVertex( Graph::Vertex(C) );
        graph.InsertVertex( Graph::Vertex(D) );
        graph.InsertVertex( Graph::Vertex(E) );
        graph.InsertVertex( Graph::Vertex(F) );
        graph.InsertEdge( Graph::Edge(A,B,2));
        graph.InsertEdge( Graph::Edge(B,A,2));
        graph.InsertEdge( Graph::Edge(A,C,1));
        graph.InsertEdge( Graph::Edge(C,A,1));
        graph.InsertEdge( Graph::Edge(B,D,1));
        graph.InsertEdge( Graph::Edge(D,B,1));
        graph.InsertEdge( Graph::Edge(C,D,4) );
        graph.InsertEdge( Graph::Edge(D,C,4) );
        graph.InsertEdge( Graph::Edge(D,E,1) );
        graph.InsertEdge( Graph::Edge(E,D,1) );
        graph.InsertEdge( Graph::Edge(E,F,10) );
        graph.InsertEdge( Graph::Edge(F,E,10) );
    }

    Astar<Graph,TestHeuristic2<Graph> > astar(graph);
    //clock_t start = std::clock();
    std::vector<EdgeTest> solution = astar.search(A,F);
    //clock_t finish = std::clock();
    //std::cout << "search time " << (finish-start) << std::endl;
    float cost = 0.0f;
    std::vector<Graph::Edge>::reverse_iterator rit;
    for(rit=solution.rbegin();rit!=solution.rend();++rit) {
        cost += rit->GetWeight();
    }
    std::cout << "solution cost " << cost << std::endl;
}

void test2(size_t s) {
    typedef Graph<VertexGrid,EdgeGrid> Grid;
    Grid graph;

    size_t max_x = 1000;
    size_t max_y = 1000;
    for (size_t x=0;x<max_x;++x) {
        for(size_t y=0;y<max_y;++y) {
            graph.InsertVertex( Grid::Vertex(max_x * y+x ,x,y,0.0f) );
        }
    }
    for (size_t x=0;x<max_x;++x) {
        for(size_t y=0;y<max_y;++y) {
            //insert in both directions
            graph.InsertEdge( Grid::Edge(max_x * y+x,max_x * y + ((x+1)%max_x),1.0) );
            graph.InsertEdge( Grid::Edge(max_x * y + ((x+1)%max_x),max_x * y+x,1.0) );
            graph.InsertEdge( Grid::Edge(max_x * y+x,max_x * y + ((x+max_x-1)%max_x),1.0) );
            graph.InsertEdge( Grid::Edge(max_x * y + ((x+max_x-1)%max_x),max_x * y+x,1.0) );
            graph.InsertEdge( Grid::Edge(max_x * y+x,max_x * ((y+1)%max_y) + x,1.0) );
            graph.InsertEdge( Grid::Edge(max_x * ((y+1)%max_y) + x,max_x * y+x,1.0) );
            graph.InsertEdge( Grid::Edge(max_x * y+x,max_x * ((y+max_y-1)%max_y) + x,1.0) );
            graph.InsertEdge( Grid::Edge(max_x * ((y+max_y-1)%max_y) + x,max_x * y+x,1.0) );
            //std::cout << max_x * y+x << " -> " << max_x * y + ((x+1)%max_x) << std::endl;
            //std::cout << max_x * y+x << " -> " << max_x * y + ((x+max_x-1)%max_x) << std::endl;
            //std::cout << max_x * y+x << " -> " << max_x * ((y+1)%max_y) + x << std::endl;
            //std::cout << max_x * y+x << " -> " << max_x * ((y+max_y-1)%max_y) + x << std::endl;
        }
    }


    Astar<Grid,ManhattanHeuristic<Grid> > astar(graph);
    size_t start_id = 0, goal_id=s*max_x+s;
    //clock_t start = std::clock();
    std::vector<Grid::Edge> solution = astar.search(start_id,goal_id);
    //clock_t finish = std::clock();
    //std::cout << "search time " << (finish-start) << std::endl;
    float cost = 0.0f;

    std::vector<Grid::Edge>::reverse_iterator rit;
    for(rit=solution.rbegin();rit!=solution.rend();++rit) {
        cost += rit->GetWeight();
    }
    std::cout << "solution cost " << cost << std::endl;
}


int main(int argc, char **argv) try {
    if (argc!=2) return 1;
    else {
        int t = std::atoi(argv[1]);
        switch (t) {
            case 0: test0(); break;
            case 1: test1(); break;
            case 2: test2(50); break; //0,0 -> 50,50 on a square grid
            case 3: test2(100); break; //0,0 -> 100,100 on a square grid
            case 4: test2(250); break; //0,0 -> 250,250 on a square grid
        }
    }
    return 0;
}
catch (const char * msg) {
    std::cout << msg << std::endl;
}
