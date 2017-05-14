/**************************************这里类模板的声明和实现放一块*****************************************************/
#ifndef MATRIXGRAPH_H_
#define MATRIXGRAPH_H_
#include<iostream>
#include<string>
#include<fstream>
#include<queue>
#include<iomanip>
#include"graph.h"
#include<algorithm>
using namespace std;
#define edgecost 1;

template<typename T, typename E>
struct Edgeinf
{
	int begin;
	int end;
	E edgeweight;
	bool operator< (const Edgeinf &rhs)
	{
		return this->edgeweight<rhs.edgeweight;
	}
};
template<typename T, typename E>
class MatrixGraph :public Graph<T, E>
{
private:
	T *VertexesList;                                            //顶点表  
	E * *Edge;                                                  //邻接矩阵 
public:
	static const E cost = edgecost;
	MatrixGraph(int size = 20);
	~MatrixGraph()
	{
		delete[]VertexesList;
		delete[]Edge;
	}
	void CreateGraph();
	void InputfileGraph(string filename);
	void display();  //显示顶点表和邻接矩阵
	void display1(); //详细的显示信息
	bool InsertVertex(const T& vertex);
	bool InsertEdge(const T vertex1, const T vertex2, E weight);
	bool removeEdge(const T vertex1, const T vertex2);
	bool removeVertex(const T vertex);
	int getVertexPos(T vertex);  //返回顶点所在的位置
	T  getvertex(int v); //与getVertexPos（）相反，返回顶点表中某位置的顶点
	E  getweight(int v1, int v2);  //返回顶点表中两个顶点（在顶点表中的位置）的边的权值
	int FirstAdjVex(int v);
	int NextAdjVex(int v, int w);
	vector<vector<E>> getedgematrix();//将邻接矩阵存在vector<vector<E>>中
	void DFSTraverse();
	void DFS(int i, vector<bool>&visited);
	void visit(int i);
	void BFSTraverse();
	vector<Edgeinf<T, E>> getedgeinfo();
};

template<typename T, typename E>
vector<Edgeinf<T, E>> MatrixGraph<T, E>::getedgeinfo()   //获得边集数组
{
	vector<Edgeinf<T, E>> edgeInf;
	Edgeinf<T, E> sedge;
	if (!Directed)
	{
		for (int i = 0; i < numVertexes; i++)
		 for (int j = i + 1; j < numVertexes; j++)
		{
			if (Edge[i][j] != 0 && Edge[i][j] != maxWeight)
			{
				sedge.begin = i;
				sedge.end = j;
				sedge.edgeweight = Edge[i][j];
				edgeInf.push_back(sedge);
			}

		}
	}	
	else
	{
		for (int i = 0; i < numVertexes; i++)
			for (int j = 0; j < numVertexes; j++)
		{
			if (Edge[i][j] != 0 && Edge[i][j] != maxWeight)
			{
				sedge.begin = i;
				sedge.end = j;
				sedge.edgeweight = Edge[i][j];
				edgeInf.push_back(sedge);
			}

		}
	}
	sort(edgeInf.begin(), edgeInf.end());
	return edgeInf;
}

template<typename T, typename E>   //构造函数
MatrixGraph<T, E>::MatrixGraph(int size = 20)
{
	maxVertexes = size;//分配的内存大小（对顶点表和邻接矩阵）
	numVertexes = 0;
	numEdges = 0;
	VertexesList = new T[maxVertexes];
	Edge = new E *[maxVertexes];
	for (int i = 0; i<maxVertexes; i++)
		Edge[i] = new E[maxVertexes];
}
template<typename T,typename E>
vector<vector<E>> MatrixGraph<T, E>::getedgematrix()
{
	vector<vector<int>> g(numVertexes);
	for (int i = 0; i < numVertexes;i++)
		for (int j = 0; j < numVertexes; j++)
		  g[i].push_back(Edge[i][j]);
	return g;
}

template <typename T, typename E>
void MatrixGraph<T, E>::CreateGraph()
{
	int n, m, i, j;
	T vertex, vertex1, vertex2;
	E weight;
	cout << "请输入顶点数和边数：";
	cin >> n >> m;
	cout << "请依次输入顶点：";
	for (int i = 0; i != n; i++)
	{
		cin >> vertex;
		this->InsertVertex(vertex);
	}
	cout << "如果是有权图输入1，否则输入2：";
	cin >> i;
	cout << "如果是有向图输入1，否则输入2：";
	cin >> j;
	if (i == 1)
		Weight = true;
	else
		Weight = false;
	if (j == 1)
		Directed = true;
	else
		Directed = false;
	if (Weight)                      //当为带权图
	{
		for (int i = 0; i<maxVertexes; i++)    //初始化邻接矩阵
			for (int j = 0; j<maxVertexes; j++)
				Edge[i][j] = (i == j) ? 0 : maxWeight;
		cout << "如果有方向则按头尾方式输入,如果无方向输入边的两个顶点" << endl;
		cout << "请以这样的方式输入边(顶点(头)，顶点(尾)，权值)：vertex1 vertex2 weight:";
	}
	else                           //当为无权图
	{
		for (int i = 0; i<maxVertexes; i++)    //初始化邻接矩阵
			for (int j = 0; j<maxVertexes; j++)
				Edge[i][j] = 0;
		cout << "如果有方向则按头尾方式输入,如果无方向输入边的两个顶点" << endl;
		cout << "请以这样的方式输入边(顶点(头),顶点(尾))：vertex1 vertex2:";
	}
	for (int i = 0; i<m; i++)
	{
		if (Weight)       //有权
			cin >> vertex1 >> vertex2 >> weight;
		else              //无权
		{
			cin >> vertex1 >> vertex2;
			weight = cost;
		}
		InsertEdge(vertex1, vertex2, weight);
	}
}
//插入顶点
template<typename T, typename E>
bool MatrixGraph<T, E>::InsertVertex(const T& vertex)   //此处只插入顶点，没有插入顶点所在的边
{
	int i = getVertexPos(vertex);
	if (i != -1)
	{
		cerr << "顶点已经存在，添加失败！" << endl;
		return false;
	}
	if (numVertexes == maxVertexes)
		return false;
	VertexesList[numVertexes++] = vertex;
	return true;
}

template<typename T, typename E>
bool MatrixGraph<T, E>::InsertEdge(const T vertex1, const T vertex2, E weight)
{
	int i = getVertexPos(vertex1);
	int j = getVertexPos(vertex2);
	if (i != -1 && j != -1)
	{
		if (Edge[i][j] == maxWeight || Edge[i][j] == 0)
		{
			if (!Directed)        //无向图,邻接矩阵为对称矩阵
				Edge[i][j] = Edge[j][i] = weight;
			else                  //有向图
				Edge[i][j] = weight;
			numEdges++;
		}
		else
		{
			cerr << "该边已经存在,添加失败！" << endl;
			return false;
		}
		return true;
	}
	return false;
}

//显示函数(显示顶点表和邻接矩阵)
template<typename T, typename E>
void MatrixGraph<T, E>::display()
{
	string s1, s2;
	if (Directed) s1 = "有向";
	else         s1 = "无向";
	if (Weight)   s2 = "有权";
	else         s2 = "无权";
	cout << "此图是" << numVertexes << "个顶点的" << s1 << s2 << "图，顶点分别是：" << endl;
	for (int i = 0; i != numVertexes; i++)
		cout << VertexesList[i] << " ";
	cout << endl;
	cout << "邻接矩阵为：" << endl;
	for (int i = 0; i<numVertexes; i++)
	{
		for (int j = 0; j < numVertexes; j++)
			if (Edge[i][j] == maxWeight)
				cout << setw(7)<< "∞" ;
			else
				cout << setw(7)<<Edge[i][j] ;
		cout << endl;
	}
}
template<typename T, typename E>
void MatrixGraph<T, E>::display1()      //具体显示边的信息（权值）
{
	T vertex1, vertex2;
	E weight;
	cout << "此图顶点数为：" << numVertexes << "边数为" << numEdges << "具体的边的信息为：" << endl;
	if (!Directed)//如果无向
	{
		for (int i = 0; i < numVertexes; i++)
		{
			for (int j = i + 1; j < numVertexes; j++)
			{
				weight = this->getweight(i, j);
				if (weight != 0 && weight != maxWeight)  //边存在的情况下
				{
					vertex1 = this->getvertex(i);
					vertex2 = this->getvertex(j);
					if (Weight)
						cout << "(" << vertex1 << "," << vertex2 << "," << weight << ")" << endl;
					else
						cout << "(" << vertex1 << "," << vertex2 << ")" << endl;
				}
			}
		}
	}
	else         //如果有向
	{
		for (int i = 0; i < numVertexes; i++)
		{
			for (int j = 0; j < numVertexes; j++)
			{
				weight = this->getweight(i, j);
				if (weight != 0 && weight != maxWeight)  //边存在的情况下
				{
					vertex1 = this->getvertex(i);
					vertex2 = this->getvertex(j);
					if (Weight)
						cout << "(" << vertex1 << "->" << vertex2 << "," << weight << ")" << endl;
					else
						cout << "(" << vertex1 << "->" << vertex2 << ")" << endl;
				}
			}
		}
	}
}

template<class T, class E>
bool MatrixGraph<T, E>::removeEdge(const T vertex1, const T vertex2)     //删除两个顶点之间的边 ,顶点不变 
{
	int i = getVertexPos(vertex1);
	int j = getVertexPos(vertex2);
	if (i != -1 && j != -1 && Edge[i][j] != 0 && Edge[i][j] != maxWeight)
	{
		if (!Directed && Weight)
			Edge[i][j] = Edge[j][i] = maxWeight;
		else if (!Directed && !Weight)
			Edge[i][j] = Edge[j][i] = 0;
		else if (Directed && Weight)
			Edge[i][j] = maxWeight;
		else
			Edge[i][j] = 0;
		numEdges--;
		return true;
	}
	return false;
};

template<class T, class E>
bool  MatrixGraph<T, E>::removeVertex(const T vertex)               //删除一个顶点 ,则删除了它所连接的边 
{
	int v = getVertexPos(vertex);
	if (v == -1 || v>numVertexes)
		return false;
	if (numVertexes == 1)
		return false;               //只剩一个顶点，不删除  

	VertexesList[v] = VertexesList[numVertexes - 1];    //用顶点表中最后一个顶点代替要删除的顶点 

	for (int i = 0; i<numVertexes; i++)                      //这里删除的边仅仅是指向待删除结点的情况 
		if (Edge[i][v]>0 && Edge[i][v] != maxWeight)
			numEdges--;
	if (Directed)                       //这里删除的边是有向情况下待删除结点指向别的顶点的情况 
	{
		for (int i = 0; i<numVertexes; i++)
			if (Edge[v][i]>0 && Edge[v][i] != maxWeight)
				numEdges--;
	}
	for (int i = 0; i<numVertexes; i++)                 //与顶点表相对应，调整邻接矩阵
		Edge[i][v] = Edge[i][numVertexes - 1];
	numVertexes--;
	for (int j = 0; j<numVertexes; j++)
		Edge[v][j] = Edge[numVertexes][j];
	return true;
}

template<typename T, typename E>
int MatrixGraph<T, E>::getVertexPos(T vertex)  //返回顶点在顶点表的位置
{
	for (int i = 0; i<numVertexes; i++)
		if (VertexesList[i] == vertex)
			return i;
	return -1;
}
template<typename T, typename E>      //与getVertexPos（）相反，返回顶点表中某位置的顶点
T MatrixGraph<T, E>::getvertex(int v)
{
	if (v >= 0 && v<numVertexes)
		return VertexesList[v];
	return 0;
}
template<typename T, typename E>
E MatrixGraph<T, E>::getweight(int v1, int v2)  //返回顶点表中两个顶点（在顶点表中的位置）的边的权值
{
	if (v1 != -1 && v2 != -1)
		return Edge[v1][v2];
	else
		return 0;
}


template<typename T, typename E>
int MatrixGraph<T, E>::FirstAdjVex(int v) //返回v的第一个邻接顶点的位置
{
	if (v != -1)
	{
		for (int i = 0; i<numVertexes; i++)
			if (Edge[v][i] != 0 && Edge[v][i] != maxWeight)
				return i;
	}
	return -1;
}

template<typename T, typename E>
int MatrixGraph<T, E>::NextAdjVex(int v, int w) //返回v的邻接顶点w的下一个邻接顶点  
{
	if (v != -1 && w != -1)
	{
		for (int i = w + 1; i<numVertexes; i++)
		{
			if (Edge[v][i] != 0 && Edge[v][i] != maxWeight)
				return i;
		}
	}
	return -1;
}
template<typename T, typename E>
void MatrixGraph<T, E>::InputfileGraph(string filename)
{
	int n, m, i, j;
	T vertex, vertex1, vertex2;
	ifstream fin(filename);
	E weight;
	//cout << "请输入顶点数和边数：";
	fin >> n >> m;
	//cout << "请依次输入顶点：";
	for (int i = 0; i != n; i++)
	{
		fin >> vertex;
		this->InsertVertex(vertex);
	}
	//cout << "如果是有权图输入1，否则输入2：";
	fin >> i;
	//cout << "如果是有向图输入1，否则输入2：";
	fin >> j;
	if (i == 1)
		Weight = true;
	else
		Weight = false;
	if (j == 1)
		Directed = true;
	else
		Directed = false;
	if (Weight)                      //当为带权图
	{
		for (int i = 0; i<maxVertexes; i++)    //初始化邻接矩阵
			for (int j = 0; j<maxVertexes; j++)
				Edge[i][j] = (i == j) ? 0 : maxWeight;
	}
	else                           //当为无权图
	{
		for (int i = 0; i<maxVertexes; i++)    //初始化邻接矩阵
			for (int j = 0; j<maxVertexes; j++)
				Edge[i][j] = 0;
	}
	for (int i = 0; i<m; i++)
	{
		if (Weight)       //有权
			fin >> vertex1 >> vertex2 >> weight;
		else              //无权
		{
			fin >> vertex1 >> vertex2;
			weight = cost;
		}
		InsertEdge(vertex1, vertex2, weight);
	}
}
template<typename T, typename E>
void MatrixGraph<T, E>::DFSTraverse()   //深度优先遍历,递归方式处理，当顶点多，边少时，选择邻接表，时间复杂度为O（n+e）
{
	vector<bool>visited(numVertexes, false);
	//for (int i = 0; i < numVertexes; i++)
	//visited[i] = false;    //先初始化所有顶点为未访问状态
	for (int j = 0; j < numVertexes; j++)
		if (!visited[j])
			DFS(j, visited);
}
template<typename T, typename E>
void MatrixGraph<T, E>::DFS(int i, vector<bool> &visited)
{
	visited[i] = true;
	visit(i);
	for (int j = 0; j < numVertexes; j++)
		if (Edge[i][j] != 0 && Edge[i][j] != maxWeight && !visited[j])
			DFS(j, visited);
}
template<typename T, typename E>
void MatrixGraph<T, E>::visit(int i)
{
	cout << VertexesList[i] << "  ";
}
template<typename T, typename E>
void MatrixGraph<T, E>::BFSTraverse()   //广度优先遍历,利用队列，类似与层序遍历
{
	vector<bool>visited(numVertexes, false);
	//for (int i = 0; i < numVertexes; i++)
	//visited[i] = false;    //先初始化所有顶点为未访问状态
	queue<int> q;  //存储顶点序号
	for (int i = 0; i < numVertexes; i++)
	{
		if (!visited[i])
		{
			q.push(i);
			visited[i] = true;
			visit(i);
			while (!q.empty())
			{
				int j = q.front();
				q.pop();
				for (int k = FirstAdjVex(j); k >= 0; k = NextAdjVex(j, k))
					if (!visited[k])
					{
					q.push(k);
					visited[k] = true;
					visit(k);
					}
			}
		}
	}
}
#endif MATRIXGRAPH_H_

