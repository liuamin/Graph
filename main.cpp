#include"graph.h"
#include"matrixgraph.h"
#include"adjlistgraph.h"
#include"path.h"
int main()
{
	string v, v1, v2;
	int num, weight;
	cout << "请输入最多容纳的顶点数：";
	cin >> num;
	MatrixGraph<string, int> a(num);
	a.InputfileGraph("graph.txt");
	
	AdjlistGraph<string, int> b(num);
	b.InputfileGraph("graph3.txt");

	cout << "图的邻接矩阵的表示形式：" << endl;
	a.display();
	a.display1();

	cout << "图的邻接表的表示形式：" << endl;
	b.display();
	b.display1();
	
	int numv = b.getnumVertexes();


	/***********************************************拓扑排序和关键路径（邻接表实现）************************************************************/
	vector<int> etv(numv, 0);
	stack<int> Stack2;
	cout << "拓扑排序为： ";
	if (TopolgicalSort(b, numv))  //这里传进去的b为深度拷贝对象
	{
		TopolgicalSort(b, numv, etv, Stack2);
		cout << endl;
		cout << "关键路径：";
		CriticalPath(b, numv, etv, Stack2);  //这里会用到拓扑排序得到的etv, Stack2。。
		cout << endl;
	}
	else
		cout << "图可能不是无环有向图，不能进行拓扑排序,关键路径也不能获得！！" << endl;

	//cout << "查看某一个顶点的邻接顶点:";
	//cin >> v;
	//v1 = a.getvertex(a.FirstAdjVex(a.getVertexPos(v)));
	//cout << v1 << endl;
	//v2 = a.getvertex(a.NextAdjVex(a.getVertexPos(v), a.getVertexPos(v1)));
	//cout << v << "的邻接顶点" << v1 << "的下一个邻接顶点" << v2 << endl;

	//cout << "插入顶点:";
	//cin >> v1;
	//a.InsertVertex(v1);  //插入点  
	//cout << "插入边：" << endl;
	//cin >> v1 >> v2 >> weight;
	//a.InsertEdge(v1, v2, weight);//插入边  
	//a.display();

	//cout << "删除边：";
	//cin >> v1 >> v2;
	//a.removeEdge(v1, v2); //删除边  
	//a.display();

	//cout << "删除顶点：";
	//cin >> v1;
	//a.removeVertex(v1);
	//a.display();

	cout << "深度优先遍历输出为：";
	a.DFSTraverse();
	cout << endl;
	cout << "广度优先遍历输出为：";
	a.BFSTraverse();
	cout << endl;

	/*************************************************最短路径（邻接矩阵实现）********************************************************/
	vector<vector<int>> g = a.getedgematrix();//获得邻接矩阵
	int numb = a.getnumVertexes();
	vector<int> p(numb, 0);
	vector<int> d(numb);
	string s1,s2;
	cout << "输入要求的路径顶点的起点和终点：";
	cin >> s1>>s2;
	int i = a.getVertexPos(s1);
	int j = a.getVertexPos(s2);
	int end = j;  //j在下面的while循环中发生了改变
	dijkstra(numb, i, p, d, g);
	cout << "dijkstra算法结果为：" << endl; //dijkstra最短路径算法
	cout << s1 << "到" << s2 << "的最短路径长度为:" << d[j] << ",";
	cout << "具体的路线为：";
	vector<int>path;
	while (j!=i)
	{
		path.push_back(p[j]);
		j = p[j];
	}
	for (vector<int>::iterator iter = path.end()-1 ; iter!=path.begin();--iter)
	{
		cout << a.getvertex(*iter) << "->";
	}
	cout << a.getvertex(*path.begin()) << "->" << s2 << endl;;
    
	vector<vector<int>> m=g;  //存储下标
	vector<vector<int>> n=g;  //存储最短路径,初始化
	Floyd(numb, m, n);
	cout << "Floyd算法结果为：" << endl;
	cout << s1 << "到" << s2 << "的最短路径长度为:" << n[i][end] << ",";
	cout << "具体的路线为：";
	cout << s1 << "->";
	int l = m[i][end];
	while (l!=end)
	{
		cout << a.getvertex(l) << "->";
		l = m[l][end];
	}
	cout << s2 << endl;

	/*********************************************最小生成树（邻接矩阵实现）*************************************************************/
	int nume = a.getnumEdges();
	vector<int>lowcost = g[0]; //初始化lowcost表示从第一个顶点v0开始寻找
	vector<int> adjvex(numb, 0);//初始化都为第一个顶点v0的下标
	vector<pair<int, int>> finaledge;
	cout << "prim最小生成树为：";
	MinispanTreeprim(numb, lowcost, adjvex, finaledge, g);
	for (auto iter = finaledge.begin(); iter != finaledge.end(); ++iter)
	{
		cout << "("<<a.getvertex((*iter).first) << "->" << a.getvertex((*iter).second) << "," << a.getweight((*iter).first, (*iter).second)<<")";
	}
	cout << endl;

	cout << "kruskal最小生成树为：";
	vector<Edgeinf<string, int>> edgeInf = a.getedgeinfo();    //边集数组
	vector<Edgeinf<string, int>> finalpath;//找到的最小生成树
	MinispanTreekruskal(numb, nume, edgeInf, finalpath);
	for (auto iter = finalpath.begin(); iter != finalpath.end(); ++iter)
	{
		cout << "(" << a.getvertex((*iter).begin) << "->" << a.getvertex((*iter).end) << "," <<(*iter).edgeweight << ")";
	}
	cout << endl;


	return 0;
}
