#include "GraphSearchDijkstra.h"
#include "GraphSearchAStar.h"

/**
	 这里保存搜素算法的一些静态成员的初始化
*/
int GraphSearchDijkstra::m_count = 0;
vector<NavGraphEdge*> GraphSearchDijkstra::m_shortestPathTree = vector<NavGraphEdge*>(0);
vector<float> GraphSearchDijkstra::m_costToThisNode = vector<float>(0);
vector<NavGraphEdge*> GraphSearchDijkstra::m_searchFroniter = vector<NavGraphEdge*>(0);

int GraphSearchAStar::m_count   =   0;
vector<float> GraphSearchAStar::m_GCosts = vector<float>(0);
vector<float> GraphSearchAStar::m_FCosts = vector<float>(0);
vector<NavGraphEdge*> GraphSearchAStar::m_shortestPathTree = vector<NavGraphEdge*>(0);
vector<NavGraphEdge*> GraphSearchAStar::m_searchFrontier = vector<NavGraphEdge*>(0);