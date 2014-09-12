#ifndef __PATH_PLANNER_H__
#define __PATH_PLANNER_H__

#include <vector>
#include <list>
#include "NavGraphEdge.h"

using namespace std;

class GameCharacter;
class MapGrid;

/**
* 路径规划类，角色通过该类的对象来搜索地图，返回要想达到目的需要走的路线 
*/
class PathPlanner
{
public:
    typedef vector<int> NodeIndexPath;          // 以节点序号构成的路径
    typedef list<NavGraphEdge> EdgePath;        // 以边构成的路径

    PathPlanner(GameCharacter* owner);
    ~PathPlanner();

    /**
    * 请求到某个指定位置的路径
    */
    bool requestPathToGridIndex(int gridIndex);

    /**
    * 返回以节点序号构成的路径
    */
    const NodeIndexPath& getNodeIndexPath();

    /**
    *  返回以边构成的路径
    */
    const EdgePath& getEdgeListPath();

private:
    GameCharacter*  m_pOwner;                   // 该路径规划的拥有者
    MapGrid*        m_graph;                    // 所在的地图
    
    // 最近一次请求路径的结果，这里是为了以后把路径请求与获取路径规划结果分离
    NodeIndexPath   m_currentIndexPath;
    EdgePath        m_currentEdgePath;
};

#endif