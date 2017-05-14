/******************************适合顶点多但是边数少的情况，因为用邻接矩阵浪费空间，所以选择用邻接链表**************************************/
#ifndef ADGLISTGRAPH_H
#define ADGLISTGRAPH_H
#include<iostream>
#include<string>
#include<fstream>
#include<queue>
#include<stack>
#include"graph.h"
using namespace std;
template<typename T, typename E>    //边表结点，存放邻接顶点的下标，权值，和下一边表结点的指针
struct EdgeNode
{
	int adjvex;
	E  weight;
	EdgeNode<T, E> *next;

	EdgeNode(){}
	EdgeNode(int num, E cost) :adjvex(num), weight(cost), next(NULL){}
	bool operator != (EdgeNode<T, E> &R)const
	{
		return (adjvex != R.adjvex) ? true : false;
	}
};

template<typename T, typename E >  //顶点表结点，存放顶点和边链表的头指针
struct VertexNode
{
	int in;    //此处加了每个顶点入度的信息
	T data;
	EdgeNode<T, E> *firstedge;    //边链表的头指针  
};

template <typename T, typename E>
class AdjlistGraph :public Graph<T, E>
{
public:
	AdjlistGraph(int size = 20);
	~AdjlistGraph();
	AdjlistGraph(const AdjlistGraph &);
	void display();
	void display1();
	T getvertex(int v);
	E getweight(int v1, int v2);
	int FirstAdjVex(int v);
	int NextAdjVex(int v, int w);
	int getVertexPos(T vertex);
	bool InsertVertex(const T& vertex);
	bool InsertEdge(const T vertex1, const T vertex2, E weight);
	bool removeEdge(const T vertex1, const T vertex2);
	bool removeVertex(const T vertex); //不完善
	void CreateGraph();
	void InputfileGraph(string filename);
	void DFSTraverse();
	void DFS(int i, vector<bool>&visited);
	void visit(int i);
	void BFSTraverse();
	int &  getin(int i);
private:
	VertexNode<T, E> * VertexTable; //顶点表  
};

template<typename T, typename E>
AdjlistGraph<T, E>::AdjlistGraph(const AdjlistGraph<T, E> &rhs)    //拷贝构造函数,拷贝时类的所有成员函数都要拷贝，这里为深度拷贝，把整个邻接表拷贝，并不是共享指针。。
{   
	maxVertexes=rhs.maxVertexes;       
	numVertexes=rhs.numVertexes;
	numEdges=rhs.numEdges;
	Weight=rhs.Weight;
	Directed=rhs.Directed;
	VertexNode<T, E> *p = new VertexNode<T, E>[numVertexes];
	for (int i = 0; i <numVertexes; i++)
	{
		p[i].in = rhs.VertexTable[i].in;
		p[i].data = rhs.VertexTable[i].data;
		p[i].firstedge =NULL;
		EdgeNode<T, E> *p2=rhs.VertexTable[i].firstedge;
		if(p2!= NULL)
		{
			EdgeNode<T, E> *ptr1 = new EdgeNode<T, E>();
			p[i].firstedge = ptr1;
			ptr1->adjvex = p2->adjvex;
			ptr1->weight = p2->weight;
			ptr1->next = NULL;
			while (p2->next!=NULL)
			{
				ptr1->next = new EdgeNode<T, E>();
				ptr1 = ptr1->next;
				p2 = p2->next;
				ptr1->adjvex = p2->adjvex;
				ptr1->weight = p2->weight;
				ptr1->next = NULL;
			}
		}
	}
	VertexTable = p;
}
template<typename T,typename E>
int & AdjlistGraph<T, E>::getin(int i)  //获得某个顶点i的入度
{
	return VertexTable[i].in;
}

template <typename T, typename E>
AdjlistGraph<T, E>::AdjlistGraph(int size=20)
{
	maxVertexes = size;
	numVertexes = 0;
	numEdges = 0;
	VertexTable = new VertexNode<T, E>[maxVertexes];
	if (VertexTable == NULL)
	{
		cerr << "存储分配错！" << endl;
		exit(1);
	}
	for (int i = 0; i<maxVertexes; i++)  //初始化邻接表
	{
		VertexTable[i].firstedge = NULL;
		VertexTable[i].in = 0;  //初始化入度信息
	}
}
template<typename T, typename E>
AdjlistGraph<T, E>::~AdjlistGraph()
{
	for (int i = 0; i<numVertexes; i++)
	{
		EdgeNode<T, E> *p = VertexTable[i].firstedge;
		while (p != NULL)
		{
			VertexTable[i].firstedge = p->next;
			delete p;
			p = VertexTable[i].firstedge;
		}
	}
	delete[]VertexTable;
}
template<typename T, typename E>
void AdjlistGraph<T, E>::CreateGraph()
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
		cout << "如果有方向则按头尾方式输入,如果无方向输入边的两个顶点" << endl;
		cout << "请以这样的方式输入边(顶点(头)，顶点(尾)，权值)：vertex1 vertex2 weight:";
	}
	else                           //当为无权图
	{
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
			weight = 1;
		}
		InsertEdge(vertex1, vertex2, weight);
	}
}
template<typename T, typename E>
bool AdjlistGraph<T, E>::InsertVertex(const T& vertex)
{
	int i = getVertexPos(vertex);
	if (i != -1)
	{
		cerr << "顶点已经存在，添加失败！" << endl;
		return false;
	}
	if (numVertexes == maxVertexes)
		return false;          //图已满，插入失败  
	VertexTable[numVertexes].data = vertex;
	numVertexes++;
	return true;
}
template<typename T, typename E>                //此处使用的是头插法
bool AdjlistGraph<T, E>::InsertEdge(const T vertex1, const T vertex2, E weight)   
{
	int i = getVertexPos(vertex1);
	int j = getVertexPos(vertex2);
	if (i != -1 && j != -1)
	{
		EdgeNode<T, E> *p = VertexTable[i].firstedge;   //先检查该边是否已经存在
		while (p != NULL && p->adjvex != j)
			p = p->next;
		if (p != NULL)//找到该边  
		{
			cout << "该边已经存在，插入失败！" << endl;
			return false;
		}
		if (!Directed)        //无向图,邻接表也是相对称的
		{
			EdgeNode<T, E> *e1;
			e1 = new EdgeNode< T, E >(j, weight);
			e1->next = VertexTable[i].firstedge;
			VertexTable[i].firstedge = e1;

			EdgeNode<T, E> *e2;
			e2 = new EdgeNode< T, E >(i, weight);
			e2->next = VertexTable[j].firstedge;
			VertexTable[j].firstedge = e2;
		}
		else                 //有向图
		{
			EdgeNode<T, E> *e1;
			e1 = new EdgeNode< T, E >(j, weight);
			e1->next = VertexTable[i].firstedge;
			VertexTable[i].firstedge = e1;
			VertexTable[j].in++;     //有向的时候存在入度，无向不存在
		}
		numEdges++;
		return true;
	}
	return false;
}
template<typename T, typename E>
void AdjlistGraph<T, E>::display()
{
	string s1, s2;
	if (Directed) s1 = "有向";
	else         s1 = "无向";
	if (Weight)   s2 = "有权";
	else         s2 = "无权";
	cout << "此图是" << numVertexes << "个顶点的" << s1 << s2 << "图，顶点分别是：" << endl;
	for (int i = 0; i != numVertexes; i++)
		cout << VertexTable[i].data << "  ";
	cout << endl;
	cout << "邻接表为：" << endl;
	for (int i = 0; i<numVertexes; i++)
	{
		if (!Directed)
		   cout << "顶点下标为" << i << " " << VertexTable[i].data << " ->";
		else
			cout << "顶点下标为" << i << " 的入度为：" << VertexTable[i].in << "  " << VertexTable[i].data << " ->";
		EdgeNode<T, E> *p = VertexTable[i].firstedge;
		while (p != NULL)
		{
			cout << "(" << p->adjvex << ", " << p->weight << ")";
			p = p->next;
			if (p != NULL)
				cout << "->";
		}
		cout << endl;
	}
}
template<typename T, typename E>
void AdjlistGraph<T, E>::display1()
{
	T vertex1, vertex2;
	E cost;
	cout << "此图顶点数为：" << numVertexes << "边数为" << numEdges << "具体的边的信息为：" << endl;
	if (!Directed)//如果无向
	{
		for (int i = 0; i < numVertexes; i++)
		{
			for (int j = i + 1; j < numVertexes; j++)
			{
				cost = this->getweight(i, j);
				if (cost != 0)  //边存在的情况下
				{
					vertex1 = this->getvertex(i);
					vertex2 = this->getvertex(j);
					if (Weight)
						cout << "(" << vertex1 << "," << vertex2 << "," << cost << ")" << endl;
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
				cost = this->getweight(i, j);
				if (cost != 0)  //边存在的情况下
				{
					vertex1 = this->getvertex(i);
					vertex2 = this->getvertex(j);
					if (Weight)
						cout << "(" << vertex1 << "->" << vertex2 << "," << cost << ")" << endl;
					else
						cout << "(" << vertex1 << "->" << vertex2 << ")" << endl;
				}
			}
		}
	}
}

template <typename T, typename E>
E AdjlistGraph<T, E>::getweight(int v1, int v2) //返回边（v1，v2）的权重，边不存在则返回0  
{
	if (v1 != -1 && v2 != -1)
	{
		EdgeNode<T, E> *p = VertexTable[v1].firstedge;
		while (p != NULL && p->adjvex != v2)
			p = p->next;
		if (p != NULL)
			return p->weight;
	}
	return 0;
}
template<typename T, typename E>
T AdjlistGraph<T, E>::getvertex(int v)
{
	return (v >= 0 && v<numVertexes) ? VertexTable[v].data : 0;
}
template<class T, class E>
int AdjlistGraph<T, E>::FirstAdjVex(int v) //获得v的第一个邻接顶点的位置 
{
	if (v != -1)
	{
		EdgeNode<T, E> *p = VertexTable[v].firstedge;
		if (p != NULL)
			return p->adjvex;
	}
	return -1;
}
template<typename T, typename E>
int AdjlistGraph<T, E>::NextAdjVex(int v, int w) //获得v的邻接顶点w的下一个邻接顶点  
{
	if (v != -1)
	{
		EdgeNode<T, E> *p = VertexTable[v].firstedge;
		while (p != NULL && p->adjvex != w)       //寻找邻接顶点w  
			p = p->next;
		if (p != NULL && p->next != NULL) //找到w且存在下一个邻接顶点  
			return p->next->adjvex;
	}
	return -1;
}
template<typename T, typename E>
int AdjlistGraph<T, E>::getVertexPos(T vertex)
{
	for (int i = 0; i<numVertexes; i++)
	{
		if (VertexTable[i].data == vertex)
			return i;
	}
	return -1;
}

template<typename T, typename E>
bool AdjlistGraph<T, E>::removeEdge(const T vertex1, const T vertex2)
{
	int i = getVertexPos(vertex1);
	int j = getVertexPos(vertex2);
	if (i != -1 && j != -1)
	{
		if (!Directed)  //如果无向
		{
			EdgeNode<T, E> *p = VertexTable[i].firstedge, *q = NULL, *s = p;
			while (p != NULL && p->adjvex != j)   //先找到该边  
			{
				q = p;
				p = p->next;
			}
			if (p != NULL)  //找到与顶点vertex1相连另一顶点vertex2的边
			{
				if (p == s)//第一个节点就找到  
					VertexTable[i].firstedge = p->next;
				else
					q->next = p->next;
				delete p;
			}
			else
				return false;
			EdgeNode<T, E> *p1 = VertexTable[j].firstedge, *q1 = NULL, *s1 = p1;
			while (p1 != NULL && p1->adjvex != i)   //先找到该边  
			{
				q1 = p1;
				p1 = p1->next;
			}
			if (p1 != NULL)  //找到与顶点vertex2相连另一顶点vertex1的边
			{
				if (p1 == s)//第一个节点就找到  
					VertexTable[j].firstedge = p1->next;
				else
					q1->next = p1->next;
				delete p1;
			}
		}
		else  //如果有向
		{
			EdgeNode<T, E> *p = VertexTable[i].firstedge, *q = NULL, *s = p;
			while (p != NULL && p->adjvex != j)   //先找到该边  
			{
				q = p;
				p = p->next;
			}
			if (p != NULL)  //找到与顶点vertex1相连另一顶点vertex2的边
			{
				if (p == s)//第一个节点就找到  
					VertexTable[i].firstedge = p->next;
				else
					q->next = p->next;
				VertexTable[j].in--;
				delete p;
			}
			else           //没有找到该边，返回false
				return false;
		}
		numEdges--;
		return true;
	}
	return false;
}
template<typename T, typename E>
bool AdjlistGraph<T, E>::removeVertex(const T vertex)   //不完善！！！这里邻接表主要保存了各个顶点的出度，所以适合无向图顶点的删除，如果是有向图，要考虑其他顶点指向此顶点的情况(也就是入度)
{
	int v = getVertexPos(vertex);
	if (v == -1 || v >= numVertexes || numVertexes == 1)
		return false;      //只剩一个顶点，不删除
	EdgeNode<T, E> *p, *s, *t;
	int  k;
	while (VertexTable[v].firstedge != NULL)     //删除该顶点，以及与之邻接的顶点中的记录 （这里只考虑了此顶点指向其他顶点情况，没考虑其他顶点指向此顶点的情况） 
	{
		p = VertexTable[v].firstedge;
		k = p->adjvex;
		s = VertexTable[k].firstedge;
		t = NULL;
		while (s != NULL && s->adjvex != v) //在对称点的邻接表里面找v，删除掉 ,没有找到s为NULL 
		{
			t = s;
			s = s->next;
		}
		if (s != NULL)
		{
			if (t == NULL) //第一个邻接顶点就是v  
				VertexTable[k].firstedge = s->next;
			else
				t->next = s->next;
			delete s;
		}
		VertexTable[v].firstedge = p->next;
		delete p;
		numEdges--;
	}
	numVertexes--;
	VertexTable[v].data = VertexTable[numVertexes].data;     //用顶点表中最后一个顶点代替要删除的顶点 
	p = VertexTable[v].firstedge = VertexTable[numVertexes].firstedge;
	while (p != NULL)      //代替之后要修改边表与最后一个顶点相连接的下标adjvex(无向情况下可以，如果有向的话，这样不行)
	{
		s = VertexTable[p->adjvex].firstedge;
		while (s != NULL)
		{
			if (s->adjvex == numVertexes)
			{
				s->adjvex = v;
				break;
			}
			else
				s = s->next;
		}
		p = p->next;
	}
	return true;
}

template<typename T, typename E>
void AdjlistGraph<T, E>::InputfileGraph(string filename)
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
	for (int i = 0; i<m; i++)
	{
		if (Weight)       //有权
			fin >> vertex1 >> vertex2 >> weight;
		else              //无权
		{
			fin >> vertex1 >> vertex2;
			weight = 1;
		}
		InsertEdge(vertex1, vertex2, weight);
	}
}
template<typename T, typename E>
void AdjlistGraph<T, E>::DFSTraverse()   //深度优先遍历,递归方式处理
{
	vector<bool> visited(numVertexes, false);
	//for (int i = 0; i < numVertexes; i++)
	//visited[i] = false;    //先初始化所有顶点为未访问状态
	for (int j = 0; j < numVertexes; j++)
		if (!visited[j])
			DFS(j, visited);
}
template<typename T, typename E>
void AdjlistGraph<T, E>::DFS(int i, vector<bool> &visited)
{
	visited[i] = true;
	visit(i);
	EdgeNode<T, E> *p = VertexTable[i].firstedge;
	while (p != NULL)
	{
		if (!visited[p->adjvex])
			DFS(p->adjvex, visited);
		p = p->next;
	}
}
template<typename T, typename E>
void AdjlistGraph<T, E>::visit(int i)
{
	cout << VertexTable[i].data << "  ";
}
template<typename T, typename E>
void AdjlistGraph<T, E>::BFSTraverse()   //广度优先遍历,利用队列，类似与层序遍历
{
	vector<bool> visited(numVertexes, false);
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
#endif ADGLISTGRAPH_H


