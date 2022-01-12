// https://dev.to/nokha_debbarma/implement-kruskal-s-algorithm-in-c-29cn
// Should be public domain, author does not state any copyright or license

#include <iostream>
#include <algorithm>
#include <vector>
#include "kruskal.hpp"

// class for an edge in a graph
Edge::Edge(int ss, int dd, int ww)
{ // constructor
    this->ss = ss;
    this->dd = dd;
    this->ww = ww;
}

bool Edge::operator==(const Edge& other) {
    return (this->ss == other.ss && this->dd == other.dd) || (this->ss == other.dd && this->dd == other.ss);
}

// class for a graph
// member function to add an edge to the undirected graph
void Graph::addEdge(int s, int d, int w)
{
    Edge obj(s, d, w);        // create one edge
    All_edges.push_back(obj); // push one edge to edge container
}

// class for finding MST in the graph
// constructor
Kruskal::Kruskal(int totalVertices)
{
    this->totalVertices = totalVertices;

    subsets.resize(totalVertices);
    // resize subsets equal to total vertices

    for (int i = 0; i < totalVertices; ++i)
    {
        subsets[i].first = i;  // set parent value equal to respective index
        subsets[i].second = 0; // set rank value equal to zero
    }
}

void Kruskal::createMST(Graph &graph)
{
    // sort the edges of graph in increasing order of their weights
    sort(graph.All_edges.begin(), graph.All_edges.end(), comparator);

    int i = 0, e = 0;
    // i =  variable to keep track of total vertices
    // e = variable to keep track of total edges in MST formed so far
    // total edges in MST  == (total vertices - 1)

    // iterate through list of edges in a graph
    while (e < (totalVertices - 1) && i < graph.All_edges.size())
    {
        // store current edge
        Edge currEdge = graph.All_edges[i++];

        // find absolute parent
        // to detect if current edge form a cycle with MST formed so far
        int x = _find(currEdge.ss); // pass current source vertex
        int y = _find(currEdge.dd); // pass current destination vertex

        if (x != y) // is they don't form a cycle
        {
            // push current edge to MST
            mst.push_back(currEdge);
            // then make that two vertex Union
            //  in other words to create edge between two vertices
            makeUnion(x, y);
        }
    }
}

int Kruskal::_find(int i)
{
    if (subsets[i].first != i) // if index is not equal to parent value
    {
        // recursively call _find()
        //  and pass current parent value
        subsets[i].first = _find(subsets[i].first);
    }

    return subsets[i].first;
}

void Kruskal::makeUnion(int x, int y)
{
    int xroot = _find(x);
    int yroot = _find(y);

    // if-else for rank comparison & update parent-rank values
    if (subsets[xroot].second < subsets[yroot].second)
    {
        subsets[xroot].first = yroot;
    }
    else if (subsets[xroot].second > subsets[yroot].second)
    {
        subsets[yroot].first = xroot;
    }
    else
    {
        subsets[xroot].first = yroot;
        subsets[yroot].second++;
    }
}
