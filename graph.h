#ifndef GRAPH_H
#define GRAPH_H
#include<iostream>
using namespace std;
#define INF 65536;
template<typename T, typename E>
class Graph
{
protected:
	int  maxVertexes;      //分配内存时的最大的顶点数
	int  numVertexes;
	int  numEdges;
	bool Weight;
	bool Directed;
public:
	static const E maxWeight = INF;
	Graph(int size = 30);
	~Graph(){}
	int getnumVertexes()
	{
		return numVertexes;
	}
	int getnumEdges()
	{
		return numEdges;
	}
};
template<typename T, typename E>
Graph<T, E>::Graph(int size = 30)
{
	maxVertexes = size;
}

#endif GRAPH_H




