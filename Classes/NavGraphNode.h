#ifndef __NAV_GRAPH_NODE_H__
#define __NAV_GRAPH_NODE_H__

#define INVALID_NODE_INDEX  -1

/**
	 节点信息
*/
class NavGraphNode
{
protected:
    int     m_iIndex;           // 索引
    float   x;                  // 网格中心坐标
    float   y;
    bool    m_bPassable;        // 该节点是否可以通过

public:
    NavGraphNode()
    {
        NavGraphNode(INVALID_NODE_INDEX);
    }

    NavGraphNode(int index, int x = 0, int y = 0, bool passable = true)
    {
        m_iIndex        =   index;
        this->x         =   x;
        this->y         =   y;
        m_bPassable     =   passable;
    }

    int index() {return m_iIndex;}

    void setPassable(bool passable) {m_bPassable = passable;}
    bool passable() {return m_bPassable;}

    float getX() {return x;}
    float getY() {return y;}
};

#endif