#ifndef SHORTPATH_H
#define SHORTPATH_H
#include"matrixgraph.h"
#include"adjlistgraph.h"
#include<vector>
#include<string>
#include<stack>
using namespace std;
void dijkstra(int num, int v, vector<int> & p, vector<int> & d, const vector<vector<int>> &g);
void Floyd(int num, vector<vector<int>> &p, vector<vector<int>>& d);
void MinispanTreeprim(int num, vector<int>& lowcost, vector<int>& adjvex, vector<pair<int, int>> &finaledge, const vector<vector<int>>&g);
void MinispanTreekruskal(int num, int numedge, const vector<Edgeinf<string, int>> & edgeInf, vector<Edgeinf<string, int>> &finalpath);
bool TopolgicalSort(AdjlistGraph<string, int> a, int num, vector<int>&etv, stack<int>& Stack2);
bool TopolgicalSort(AdjlistGraph<string, int> a, int num);
void CriticalPath(AdjlistGraph<string, int> a, int num, vector<int>&etv, stack<int>& Stack2);
#endif
