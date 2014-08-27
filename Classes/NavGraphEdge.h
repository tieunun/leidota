#ifndef __NAV_GRAPH_EDGE_H__
#define __NAV_GRAPH_EDGE_H__

#include "NavGraphNode.h"

/**
	 有向边
*/
class NavGraphEdge
{
protected:
    int     m_iFrom;
    int     m_iTo;
    float   m_fCost;
    bool    m_bPassable;                // 该边是否可以通过

public:
    NavGraphEdge(int from, int to, float cost = 1, bool passable = true)
    {
        m_iFrom     =   from;
        m_iTo       =   to;
        m_fCost     =   cost;
        m_bPassable =   passable;
    }

    NavGraphEdge()
    {
        NavGraphEdge(INVALID_NODE_INDEX, INVALID_NODE_INDEX);
    }

    int from(){return m_iFrom;}
    void setFrom(int from) {m_iFrom = from;}

    int to() {return m_iTo;}
    void setTo(int to) {m_iTo = to;}

    float cost() {return m_fCost;}
    void setCost(float cost) {m_fCost = cost;}

    void setPassable(bool passable) {m_bPassable = passable;}
    bool passable() {return m_bPassable;}
};

#endif