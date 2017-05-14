#include"path.h"
#include"graph.h"
#include"matrixgraph.h"
#include"adjlistgraph.h"
#define INFI 65536;
//这里都是利用邻接矩阵找的最短路径和最小生成树
//num指顶点的数目，v指出发的顶点，g表示邻接矩阵，p矩阵为到某顶点的前驱，d为v到各个顶点的最小权值
//时间复杂度为O(n^2)
void dijkstra(int num, int v, vector<int>&p, vector<int>&d,const vector<vector<int>>& g)
{
	int min,k;
	vector<bool> final(num,false);
	for (int i = 0; i < num; i++)
		d[i] = g[v][i];
	d[v] = 0;   //表示v到v路径长度为0
	final[v] = true;   //表示v到v路径不需要求
	for (int i = 0; i < num; ++i)
	{
		min = INFI;
			for (int j = 0; j < num; ++j)
			{
				if (!final[j] && d[j] < min)
				{
					k = j;
					min = d[j];
				}

			}
			final[k] = true;   //找到v最近的顶点
			for (int w = 0; w< num; w++)//v具体k最近，如果v到k的距离加k到w的距离小于v到w的距离，则调整d使得为最短的距离
			{
				if (!final[w] && (min + g[k][w]) < d[w])
				{
					d[w] = min + g[k][w];
					p[w] = k;     //p矩阵为到某顶点的前驱
				}
			}
	}
}
//这里p为存放途径的下标，d为某顶点到某顶点的最短路径
//时间复杂度为O(n^3)
void Floyd(int num,vector<vector<int>> &p, vector<vector<int>>& d)
{
	for (int i = 0; i < num; ++i)   //初始化p
	{
		for (int j = 0; j < num; ++j)
		{
			//d[i][j] = g[i][j];
			p[i][j] = j;
		}
	}
	for (int i = 0; i < num; ++i)   //初始化d和p
	{
		for (int j = 0; j < num; ++j)
		{
			for (int k=0; k < num; k++)
			{
				if (d[j][k] > d[j][i] + d[i][k])
				{
					d[j][k] =d[j][i] + d[i][k];
					p[j][k] = p[j][i];
				}
			}
		}
	 }
}
//lowcost为计算过程中保存的最短路径，当lowcost全为零时说明已经生成最小生成树，adjvex保存顶点的下标,finaledge为最终找到最小生成树的各个边
//时间复杂度为O(n^2)
void MinispanTreeprim(int num,vector<int>& lowcost, vector<int>& adjvex,vector<pair<int,int>> &finaledge,const vector<vector<int>>&g)
{
	int min;
	lowcost[0] = 0;  //表示已经将第一个顶点加入树中
	adjvex[0] = 0;
	for (int i = 1; i < num; i++)
	{
		min = INFI;
		int j = 1, k = 0;
		while (j < num)  //k为最小值min的下标
		{
			if (lowcost[j] != 0 && lowcost[j] < min)
			{
				min = lowcost[j];
				k = j;
			}
			++j;
		}
		pair<int, int> edge(adjvex[k], k);
		finaledge.push_back(edge);
		lowcost[k] = 0;//将当前顶点设置为0，说明此顶点已经加入树中
		for (j = 1; j < num; j++)
		{
			if (lowcost[j] != 0 && g[k][j] < lowcost[j])
			{
				lowcost[j] = g[k][j];//较小的路径存入lowcost中
				adjvex[j] = k;  //关联这几个边的顶点k存入adjvex中
			}
		}
	}
}

int Find(vector<int> parent, int f)  //找到所连顶点的尾部下标
{
	while (parent[f] > 0)
		f = parent[f];
	return f;
}
// edgeInf为按边权值由小到大存储的每条边（包括起点，终点，权值），finalpath为最终找到的最小生成树，numedge为边数
//总时间复杂度为0（eloge）,适合边少的图（也就是比较稀疏的图）。
void MinispanTreekruskal(int num, int numedge, const vector<Edgeinf<string, int>> & edgeInf, vector<Edgeinf<string, int>> &finalpath)
{
	int n, m;
	vector<int> parent(num, 0);//判断边是否构成回路
	for (int i = 0; i < numedge; i++)
	{
		n = Find(parent, edgeInf[i].begin);  //Find函数时间复杂度为O（loge）
		m = Find(parent, edgeInf[i].end);
		if (n != m)//说明没有构成环路
		{
			parent[n] = m;
			finalpath.push_back(edgeInf[i]);
		}
	}
}
//将入度为零的顶点入栈的时间复杂度为O（n）,while循环入度减一，和边数有关时间复杂度为O（e）,所以总时间复杂度为O（n+e）
//拓扑排序,使用邻接表,用于计算关键路径,
//这里a为建立图的邻接表的临时对象，临时对象的入度改变，原有的入度并未改变。a使用的是构造拷贝构造函数，深度拷贝，这样才能只改变拷贝构造的对象的入度，原有的不改变
//num为顶点数目，etv为顶点发生的最早时间，Stack2将拓扑排序的顶点入栈。
bool TopolgicalSort(AdjlistGraph<string, int> a, int num, vector<int>&etv, stack<int>& Stack2)  
{
	int count = 0, gettop;
	stack<int> Stack;
	for (int i = 0; i < num; i++)
		if (a.getin(i) == 0)
			Stack.push(i);    //将入度为0的顶点的下标入栈
	while (!Stack.empty())
	{
		gettop = Stack.top();
		Stack.pop();
		cout << a.getvertex(gettop) << " ";
		count++;     //统计输出的顶点数
		Stack2.push(gettop);
		for (int k = a.FirstAdjVex(gettop); k >= 0; k = a.NextAdjVex(gettop, k))
		{
			if (!(--a.getin(k))) //将邻接顶点的入度减一，当入度为零时，入栈..注意此处一旦运行一次，图中的各个顶点的入度都为0了，意味着删除了该顶点
				 Stack.push(k);
			if (etv[gettop] + a.getweight(gettop,k)>etv[k])
				etv[k] = etv[gettop] + a.getweight(gettop, k);
		}
	}
	if (count < num)//如果count小于顶点数，说明存在环，不能进行拓扑排序
	 return false;				
	return true;
}
bool TopolgicalSort(AdjlistGraph<string, int> a, int num)  //重载函数TopolgicalSort，只是为了判断返回值bool是否能进行拓扑排序
{
	int count = 0, gettop;
	stack<int> Stack;
	for (int i = 0; i < num; i++)
		if (a.getin(i) == 0)
			Stack.push(i);    //将入度为0的顶点的下标入栈
	while (!Stack.empty())
	{
		gettop = Stack.top();
		Stack.pop();
		count++;     //统计输出的顶点数	
		for (int k = a.FirstAdjVex(gettop); k >= 0; k = a.NextAdjVex(gettop, k))
		{
			if (!(--a.getin(k))) //将邻接顶点的入度减一，当入度为零时，入栈..注意此处一旦运行一次，图中的各个顶点的入度都为0了，意味着删除了该顶点
				Stack.push(k);
		}
	}
	if (count < num)//如果count小于顶点数，说明存在环，不能进行拓扑排序
		return false;
	return true;
}
//关键路径，利用拓扑排序的结果计算关键路径
//etv为顶点最早发生的时间，ltv为顶点最晚发生的时间，
//通过分析，总的时间复杂度为O（n+e）
void CriticalPath(AdjlistGraph<string, int> a,int num, vector<int>&etv, stack<int>& Stack2)
{
	int gettop,ete,lte; //ete为弧最早发生的时间，lte为弧最晚发生的时间
	vector<int> ltv(num, etv[num - 1]);
	while (!Stack2.empty())   //时间复杂度为O（n+e）,且求etv和Stack2时间复杂度为与拓扑排序的时间复杂度O（n+e）
	{
		gettop = Stack2.top();
		Stack2.pop();
		for (int k = a.FirstAdjVex(gettop); k >= 0; k = a.NextAdjVex(gettop, k))
		{
			if (ltv[k] - a.getweight(gettop, k) < ltv[gettop]) //求ltv为顶点最晚发生的时间
				ltv[gettop] = ltv[k] - a.getweight(gettop, k);
		}
	}
	for (int i = 0; i < num;i++)    //时间复杂度为O（n+e）
		for (int j = a.FirstAdjVex(i); j >= 0; j = a.NextAdjVex(i, j))
		{
		ete = etv[i];
		lte = ltv[j] - a.getweight(i, j);
		if (ete == lte)
			cout << "("<<a.getvertex(i) << "->" << a.getvertex(j) << "," << a.getweight(i, j)<<") ";
		}
}
