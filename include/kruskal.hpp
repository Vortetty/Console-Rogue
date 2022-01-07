#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

// class for an edge in a graph
class Edge
{
public:
    int ss, dd, ww;
    // ss = source edge
    // dd = destination edge
    // ww = weight of an edge
    Edge(int ss, int dd, int ww);
};

// class for a graph
class Graph
{
public:
    vector<Edge> All_edges; // to hold list of all edges of the graph

    // member function to add an edge to the undirected graph
    void addEdge(int s, int d, int w);
};

// declare a displayMST function to define it later
void displayMST(const vector<Edge> &);

// class for finding MST in the graph
class Kruskal
{

public:
    int totalVertices; // total vertices

    vector<pair<int, int>> subsets;
    // subsets will hold list of [parent - rank] pairs
    // which we will use in Union Find
    // by path compression algorithm

    vector<Edge> mst; // declare a container to store edges of MST

    // constructor
    Kruskal(int totalVertices);

    static bool comparator(Edge &a, Edge &b);

    void createMST(Graph &graph);

    int _find(int i);

    void makeUnion(int x, int y);
};
