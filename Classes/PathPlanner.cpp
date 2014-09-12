#include "PathPlanner.h"
#include "GameCharacter.h"
#include "GraphSearchAStar.h"

PathPlanner::PathPlanner( GameCharacter* owner )
{
    m_pOwner    =   owner;
}

PathPlanner::~PathPlanner()
{

}

bool PathPlanner::requestPathToGridIndex( int gridIndex )
{
    m_graph     =   m_pOwner->getMapGrid();
    assert(gridIndex >= 0 && gridIndex < m_graph->getNodeNumber());

    // 格子序号组成的路径
    auto tmpObjectOnGrid    =   m_pOwner->getObjectOnGrid();
    auto tmpSourceIndex     =   tmpObjectOnGrid->nodeIndex;

    // 搜索路径前先去掉自己所占的坑位
    m_graph->removeObjectFromGrid(tmpObjectOnGrid);
    m_currentIndexPath      =   GraphSearchAStar::search(*m_graph, tmpSourceIndex, 
        gridIndex, MapGrid::calculateH);
    // 恢复自己
    m_graph->addObjectToGrid(tmpObjectOnGrid);

    m_currentEdgePath.clear();
    if (m_currentIndexPath.size() == 0)
    {
        return false;
    }

    // 构造边组成的路径
    for (auto tmpIndex = 0; tmpIndex < m_currentIndexPath.size() - 1; tmpIndex++)
    {
        m_currentEdgePath.push_back(m_graph->getEdgeByFromAndTo(m_currentIndexPath[tmpIndex], 
            m_currentIndexPath[tmpIndex + 1]));
    }

    return true;
}

const PathPlanner::NodeIndexPath& PathPlanner::getNodeIndexPath()
{
    return m_currentIndexPath;
}

const PathPlanner::EdgePath& PathPlanner::getEdgeListPath()
{
    return m_currentEdgePath;
}
