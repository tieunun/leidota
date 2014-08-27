#ifndef __SPARSE_GRAPH_H__
#define __SPARSE_GRAPH_H__

#include "NavGraphNode.h"
#include "NavGraphEdge.h"

#include <map>
#include <list>
#include <assert.h>

using namespace std;

/**
	 保存稀疏图的数据结构
*/
class SparseGraph
{
protected:
    typedef map<int, NavGraphNode>  NodeVector;
    typedef list<NavGraphEdge>      EdgeList;
    typedef map<int, EdgeList>      EdgeListVector;

    /**
    	 按照节点的index下标保存在这个里面
    */
    NodeVector      m_nodes;
    /**
    	 下标是节点下标，对应一个从该节点出发的所有边
    */
    EdgeListVector  m_edges;

    bool            m_directed;             // 是否有向

public:
    SparseGraph(bool directed = false)
    {
        m_directed  =   directed;
    }

    ~SparseGraph()
    {
        
    }

    void addNode(NavGraphNode node)
    {
        assert(m_nodes.find(node.index()) == m_nodes.end() && "add node multi");

        m_nodes.insert(make_pair(node.index(), node));
        if (m_edges.find(node.index()) == m_edges.end())
        {
            m_edges.insert(make_pair(node.index(), EdgeList()));
        }
    }

    void addEdge(NavGraphEdge edge)
    {
        assert(m_nodes.find(edge.from()) != m_nodes.end() && m_nodes.find(edge.to()) != m_nodes.end()
            && "invalid edge from or to");
        EdgeList* tmpList   =   &(*m_edges.find(edge.from())).second;
        tmpList->push_back(edge);

        if (!m_directed)
        {
            // 如果是无方向的，还需要创建一个相反的路径
            NavGraphEdge newEdge(edge);
            newEdge.setFrom(edge.to());
            newEdge.setTo(edge.from());
            tmpList =   &(*m_edges.find(newEdge.from())).second;
            tmpList->push_back(newEdge);
        }
    }

    int getNodeNumber()
    {
        return m_nodes.size();
    }

    NavGraphNode& getNodeByIndex(int index)
    {
        assert(m_nodes.find(index) != m_nodes.end());
        return (*m_nodes.find(index)).second;
    }

    /**
    	 用来修改某个节点的可通过属性
    */
    void setNodePassable(int nodeIndex, bool passable)
    {
        // 修改相应节点
        m_nodes[nodeIndex].setPassable(passable);
    }

    /**
    	 设置所有的格子都可以通过
    */
    void resetAllNodePassable()
    {
        for (auto tmpIterator = m_nodes.begin(); tmpIterator != m_nodes.end(); tmpIterator++)
        {
            tmpIterator->second.setPassable(true);
        }
    }

    /**
    	 用来遍历从该节点出发的所有边，只有该边可以使用，并且该节点以及对应的to节点可以使用的才会
         返回
    */
    class EdgeIterator
    {
    protected:
        typedef EdgeList::iterator  CurEdge;

        int             m_nodeIndex;
        SparseGraph*    m_graph;
        CurEdge         m_iterator;
        EdgeList*       m_list;

    public:
        EdgeIterator(SparseGraph* graph, int nodeIndex)
        {
            // 要求必须该节点存在，而且必须是可以使用的节点
            assert(graph->m_edges.find(nodeIndex) != graph->m_edges.end() && "invalid nodeindex");

            m_graph     =   graph;
            m_nodeIndex =   nodeIndex;
            m_list      =   &((*m_graph->m_edges.find(nodeIndex)).second);
            m_iterator  =   m_list->begin();
        }

        NavGraphEdge* begin()
        {
            m_iterator  =   m_list->begin();

            // 这里需要返回可以使用的边，并且改变的to指向的节点也可以使用
            while ((!end()) && (!m_iterator->passable() || !m_graph->getNodeByIndex(m_iterator->to()).passable()))
            {
                m_iterator++;
            }

            if (end())
            {
                return nullptr;
            }
            return &(*m_iterator);
        }

        NavGraphEdge* next()
        {
            m_iterator++;

            // 也需要判断，必须返回可以使用的
            while ((!end()) && (!m_iterator->passable() || !m_graph->getNodeByIndex(m_iterator->to()).passable()))
            {
                m_iterator++;
            }

            if (end())
            {
                return nullptr;
            }

            return &(*m_iterator);
        }

        bool end()
        {
            auto tmpIterator    =   m_iterator;
            for (; tmpIterator != m_list->end(); tmpIterator++)
            {
                if (tmpIterator->passable() && m_graph->getNodeByIndex(tmpIterator->to()).passable())
                {
                    return false;
                }
            }

            return true;
        }
    };

    friend class EdgeIterator;

    /**
    	 用来遍历某一个节点周围的其他节点
    */
    class NodeIterator
    {
    protected:
        typedef EdgeList::iterator  CurEdge;

        int             m_nodeIndex;
        SparseGraph*    m_graph;
        CurEdge         m_iterator;
        EdgeList*       m_list;

    public:
        NodeIterator(SparseGraph* graph, int nodeIndex)
        {
            // 该节点必须可以使用
            assert(graph->m_edges.find(nodeIndex) != graph->m_edges.end() && "invalid nodeindex");

            m_graph     =   graph;
            m_nodeIndex =   nodeIndex;
            m_list      =   &((*m_graph->m_edges.find(nodeIndex)).second);
            m_iterator  =   m_list->begin();
        }

        NavGraphNode* begin()
        {
            m_iterator  =   m_list->begin();

            while ((!end()) && (!m_iterator->passable() || !m_graph->getNodeByIndex(m_iterator->to()).passable()))
            {
                m_iterator++;
            }

            if (end())
            {
                return nullptr;
            }
            return &(*m_graph->m_nodes.find((*m_iterator).to())).second;
        }

        NavGraphNode* next()
        {
            m_iterator++;

            while ((!end()) && (!m_iterator->passable() || !m_graph->getNodeByIndex(m_iterator->to()).passable()))
            {
                m_iterator++;
            }

            if (end())
            {
                return nullptr;
            }
            return &(*m_graph->m_nodes.find((*m_iterator).to())).second;
        }

        bool end()
        {
            auto tmpIterator    =   m_iterator;
            for (; tmpIterator != m_list->end(); tmpIterator++)
            {
                if (tmpIterator->passable() && m_graph->getNodeByIndex(tmpIterator->to()).passable())
                {
                    return false;
                }
            }

            return true;
        }
    };

    friend class NodeIterator;
};

#endif