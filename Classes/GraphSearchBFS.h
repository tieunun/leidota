#ifndef __GRAPH_SEARCH_BFS_H__
#define __GRAPH_SEARCH_BFS_H__

#include "SparseGraph.h"
#include <queue>
#include <vector>

using namespace std;

#define UNVISITED   0
#define VISITED     1

#define NOPARENT    -1

/**
	 采用广度优先搜索的方式
*/
class GraphSearchBFS
{
protected:
    static int m_count;

public:
    static vector<int> search(SparseGraph& grap, int sourceIndex, int targetIndex)
    {
        assert(sourceIndex < grap.getNodeNumber() && targetIndex < grap.getNodeNumber()
            && "invalid sourceIndex or targetIndex");

        // 一个路径，这里就只能用3->9->11，route[9] = 3, route[11] = 9
        vector<int> route(grap.getNodeNumber(), NOPARENT);
        queue<NavGraphEdge*> edgeQueue;

        NavGraphEdge    dummy(sourceIndex, sourceIndex);
        edgeQueue.push(&dummy);

        m_count =   0;

        while (!edgeQueue.empty())
        {
            m_count++;

            NavGraphEdge* tmpEdge   =   edgeQueue.front();
            edgeQueue.pop();

            route[tmpEdge->to()]  =   tmpEdge->from();

            if (tmpEdge->to() == targetIndex)
            {
                // 发现了
                vector<int> retRoute;
                retRoute.push_back(targetIndex);
                int tmpParent = route[targetIndex];
                while (tmpParent != sourceIndex)
                {
                    retRoute.push_back(tmpParent);
                    tmpParent   =   route[tmpParent];
                }
                retRoute.push_back(sourceIndex);

                reverse(retRoute.begin(), retRoute.end());
                return retRoute;
            }

            // 遍历该节点出发的边
            SparseGraph::EdgeIterator   tmpIterator(&grap, tmpEdge->to());
            tmpEdge = tmpIterator.begin();
            for (; !tmpIterator.end(); tmpEdge = tmpIterator.next())
            {
                if (route[tmpEdge->to()] == NOPARENT)
                {
                    edgeQueue.push(tmpEdge);
                }
            }
        }

        route.clear();
        return route;
    }
};

int GraphSearchBFS::m_count =   0;

#endif