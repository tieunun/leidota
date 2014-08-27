#ifndef __GRAPH_SEARCH_A_STAR_H__
#define __GRAPH_SEARCH_A_STAR_H__

#include <vector>
#include "SparseGraph.h"
#include "PriorityQLow.h"

using namespace std;

/**
	 用来计算启发因子的回调函数
*/ 
typedef float (*HeuristicFunc)(SparseGraph& grap, int sourceIndex, int targetIndex);

/**
	 A星算法搜索路径
*/
class GraphSearchAStar
{
protected:
    static int m_count;                                 // 统计循环次数
    static vector<float> m_GCosts;                      // 到该节点的累计开销
    static vector<float> m_FCosts;                      // 通过计算m_GCosts的每个节点与启发因子相加的结果
    static vector<NavGraphEdge*> m_shortestPathTree;    // 最小路径树
    static vector<NavGraphEdge*> m_searchFrontier;      // 搜索用的

public:
    static vector<int> search(SparseGraph& grap, int sourceIndex, int targetIndex, HeuristicFunc heuristicFunc)
    {
        if (sourceIndex == targetIndex)
        {
            return vector<int>(0);
        }

        if (m_GCosts.size() != grap.getNodeNumber())
        {
            m_GCosts    =   vector<float>(grap.getNodeNumber(), 0);
        }
        else
        {
            for (int n = 0; n < m_GCosts.size(); n++)
            {
                m_GCosts[n] =   0;
            }
        }

        if (m_FCosts.size() != grap.getNodeNumber())
        {
            m_FCosts    =   vector<float>(grap.getNodeNumber(), 0);
        }
        else
        {
            for (int n = 0; n < m_FCosts.size(); n++)
            {
                m_FCosts[n] =   0;
            }
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

        if (m_searchFrontier.size() != grap.getNodeNumber())
        {
            m_searchFrontier    =   vector<NavGraphEdge*>(grap.getNodeNumber(), nullptr);
        }
        else
        {
            for (int n = 0; n < m_searchFrontier.size(); n++)
            {
                m_searchFrontier[n] =   nullptr;
            }
        }

        // 优先级队列
        PriorityQLow prioQ(m_FCosts);

        // 将自己加入
        prioQ.insert(sourceIndex);

        m_count =   0;
        while (!prioQ.empty())
        {
            m_count++;

            int tmpNodeIndex    =   prioQ.pop();

            // 加入最小路径树中
            m_shortestPathTree[tmpNodeIndex]    =   m_searchFrontier[tmpNodeIndex];

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
                // 从源节点到该节点的消耗
                float tmpGCost  =   m_GCosts[tmpNodeIndex] + tmpEdge->cost();
                // 这里就是计算启发因子
                float tmpHCost  =   heuristicFunc(grap, tmpEdge->to(), targetIndex);

                if (m_searchFrontier[tmpEdge->to()] == nullptr)
                {
                    m_searchFrontier[tmpEdge->to()] =   tmpEdge;
                    m_GCosts[tmpEdge->to()] =   tmpGCost;
                    m_FCosts[tmpEdge->to()] =   tmpGCost + tmpHCost;
                    prioQ.insert(tmpEdge->to());
                }
                else if (tmpGCost < m_GCosts[tmpEdge->to()])
                {
                    // 需要松变
                    m_searchFrontier[tmpEdge->to()] =   tmpEdge;
                    m_GCosts[tmpEdge->to()]         =   tmpGCost;
                    m_FCosts[tmpEdge->to()]         =   tmpGCost + tmpHCost;
                    prioQ.reOrder();
                }
            }
        }

        return vector<int>(0);
    }
};

#endif