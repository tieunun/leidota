#ifndef __GRAPH_SEARCH_DIJKSTRA_H__
#define __GRAPH_SEARCH_DIJKSTRA_H__

#include <vector>
#include "PriorityQLow.h"
#include "SparseGraph.h"

using namespace std;

/**
	Dijkstra搜索算法
*/
class GraphSearchDijkstra
{
protected:
    static int m_count;                                 // 统计循环次数

    /**
    	 以下是为了防止每次search都要在栈中创建一次
    */
    static vector<NavGraphEdge*> m_shortestPathTree;    // 搜索用的最小路径树
    static vector<float> m_costToThisNode;              // 记录当前到达该节点的最小消耗
    static vector<NavGraphEdge*> m_searchFroniter;      // 临时记录当前到达某个节点的父边

public:
    static vector<int> search(SparseGraph& grap, int sourceIndex, int targetIndex)
    {
        if (sourceIndex == targetIndex)
        {
            return vector<int>();
        }

        if (m_shortestPathTree.size() != grap.getNodeNumber())
        {
            m_shortestPathTree  =   vector<NavGraphEdge*>(grap.getNodeNumber(), nullptr);
        }
        else
        {
            for (int n = 0; n < m_shortestPathTree.size(); n++)
            {
                m_shortestPathTree[n]   =   nullptr;
            }
        }

        if (m_costToThisNode.size() != grap.getNodeNumber())
        {
            m_costToThisNode    =   vector<float>(grap.getNodeNumber(), 0);
        }
        else
        {
            for (int i = 0; i < m_costToThisNode.size(); i++)
            {
                m_costToThisNode[i] =   0;
            }
        }

        if (m_searchFroniter.size() != grap.getNodeNumber())
        {
            m_searchFroniter    =   vector<NavGraphEdge*>(grap.getNodeNumber(), nullptr);
        }
        else
        {
            for (int n = 0; n < m_searchFroniter.size(); n++)
            {
                m_searchFroniter[n] =   nullptr;
            }
        }

        // 一个优先级队列
        PriorityQLow prioQ(m_costToThisNode);

        // 将自己加入
        prioQ.insert(sourceIndex);

        m_count =   0;
        while (!prioQ.empty())
        {
            m_count++;

            int tmpNodeIndex    =   prioQ.pop();

            // 加入最小路径树中
            m_shortestPathTree[tmpNodeIndex]    =   m_searchFroniter[tmpNodeIndex];

            if (tmpNodeIndex == targetIndex)
            {
                // 发现了
                vector<int> tmpRet;
                tmpRet.push_back(targetIndex);
                NavGraphEdge* tmpTreeEdge   =   m_shortestPathTree[targetIndex];
                while (tmpTreeEdge->from() != sourceIndex)
                {
                    tmpRet.push_back(tmpTreeEdge->from());
                    tmpTreeEdge =   m_shortestPathTree[tmpTreeEdge->from()];
                }
                tmpRet.push_back(sourceIndex);

                reverse(tmpRet.begin(), tmpRet.end());
                return tmpRet;
            }

            // 开始搜索该节点周围的边
            SparseGraph::EdgeIterator tmpIterator(&grap, tmpNodeIndex);
            NavGraphEdge* tmpEdge   =   tmpIterator.begin();
            for (; !tmpIterator.end(); tmpEdge = tmpIterator.next())
            {
                float tmpNewCost    =   m_costToThisNode[tmpNodeIndex] + tmpEdge->cost();

                if (m_searchFroniter[tmpEdge->to()] == nullptr)
                {
                    m_searchFroniter[tmpEdge->to()] =   tmpEdge;
                    m_costToThisNode[tmpEdge->to()] =   tmpNewCost;
                    prioQ.insert(tmpEdge->to());
                }
                else if (tmpNewCost < m_costToThisNode[tmpEdge->to()])
                {
                    // 需要松变
                    m_searchFroniter[tmpEdge->to()] =   tmpEdge;
                    m_costToThisNode[tmpEdge->to()] =   tmpNewCost;
                    prioQ.reOrder();
                }
            }
        }

        return vector<int>(0);
    }
};

#endif