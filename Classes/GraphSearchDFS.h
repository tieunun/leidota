#ifndef __GRAPH_SEARCH_DFS_H__
#define __GRAPH_SEARCH_DFS_H__

#include "SparseGraph.h"
#include <vector>
#include <stack>

using namespace std;

#define UNVISITED   0
#define VISITED     1

#define NOCHILD    -1

/**
	 使用深度优先搜索方式来搜索一个稀疏图
*/
class GraphSearchDFS
{
protected:
    static int m_count;

public:
    /**
    	 在制定图上搜索从下标为sourceIndex节点到targetIndex节点的一条路径
         @return    如果找到就返回从sourceIndex开始的路径的节点下标，否则返回一个空的
    */
    static vector<int> search(SparseGraph& grap, int sourceIndex, int targetIndex)
    {
        assert(sourceIndex < grap.getNodeNumber() && targetIndex < grap.getNodeNumber()
            && "invalid sourceIndex or targetIndex");

        m_count =   0;

        // 用来保存路径的
        vector<int> route;

        // 用来保存每个节点被访问的情况
        vector<int> nodeVisited(grap.getNodeNumber());

        // 用来分支节点的index
        stack<int>  nodeStack;
        while (!nodeStack.empty())
        {
            nodeStack.pop();
        }

        nodeStack.push(sourceIndex);
        nodeVisited[sourceIndex]    =   VISITED;

        while (!nodeStack.empty())
        {
            m_count++;

            int tmpNewIndex =   nodeStack.top();
            nodeStack.pop();

            // 标记为已经访问
            nodeVisited[tmpNewIndex]    =   VISITED;

            if (route.empty() || route[route.size() - 1] != tmpNewIndex)
            {
                route.push_back(tmpNewIndex);
            }
            
            if (tmpNewIndex == targetIndex)
            {
                return route;
            }

            // 将周围还未访问的节点压栈
            SparseGraph::NodeIterator tmpIterator(&grap, tmpNewIndex);
            NavGraphNode* tmpNode = tmpIterator.begin();
            // 表示是否压入了栈
            bool tmpFlag = false;
            for (; !tmpIterator.end(); tmpNode = tmpIterator.next())
            {
                if (nodeVisited[tmpNode->index()] == UNVISITED)
                {
                    if (!tmpFlag)
                    {
                        nodeStack.push(tmpNewIndex);
                        tmpFlag =   true;
                    }
                    nodeStack.push(tmpNode->index());
                }
            }

            if (!tmpFlag)
            {
                route.pop_back();
            }
        }

        route.clear();
        return route;
    }

    /**
    	 采用另外一种方法，只是这里的栈里保存的是边
    */
    static vector<int> search2(SparseGraph& grap, int sourceIndex, int targetIndex)
    {
        assert(sourceIndex < grap.getNodeNumber() && targetIndex < grap.getNodeNumber()
            && "invalid sourceIndex or targetIndex");

        m_count =   0;

        // 这里保存的是路径，不过采用另一种方式
        vector<int> route(grap.getNodeNumber(), NOCHILD);

        // 用来临时保存边的栈
        stack<NavGraphEdge*> edgeStack;

        // 构造一个自己的边
        NavGraphEdge dummy(sourceIndex, sourceIndex);
        edgeStack.push(&dummy);

        while (!edgeStack.empty())
        {
            m_count++;

            NavGraphEdge* tmpEdge   =   edgeStack.top();
            edgeStack.pop();

            if (tmpEdge != &dummy)
            {
                route[tmpEdge->from()]    =   tmpEdge->to();
            }

            if (tmpEdge->to() == targetIndex)
            {
                // 表示找到了
                vector<int> retRoute;
                int tmpNext   =   route[sourceIndex];
                retRoute.push_back(sourceIndex);
                while (tmpNext != targetIndex)
                {
                    retRoute.push_back(tmpNext);
                    tmpNext =   route[tmpNext];
                }
                retRoute.push_back(targetIndex);

                return retRoute;
            }

            // 遍历从该节点出发的周围的边
            SparseGraph::EdgeIterator tmpIterator(&grap, tmpEdge->to());
            tmpEdge =   tmpIterator.begin();
            for (; !tmpIterator.end(); tmpEdge = tmpIterator.next())
            {
                if (route[tmpEdge->to()] == NOCHILD)
                {
                    edgeStack.push(tmpEdge);
                }
            }
        }

        route.clear();
        return route;
    }
};

int GraphSearchDFS::m_count =   0;

#endif