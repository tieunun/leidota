#include "GoalTraverseEdge.h"
#include "GameCharacter.h"
#include "TimeTool.h"
#include "MathTool.h"

GoalTraverseEdge::GoalTraverseEdge( GameCharacter* owner, const NavGraphEdge& edge)
    :Goal<GameCharacter>(owner), m_marginOfError(0.5), m_fuzzyReachGap(10)
{
    m_edge  =   edge;
}

GoalTraverseEdge::~GoalTraverseEdge()
{

}

void GoalTraverseEdge::activate()
{
    /**
    *  对于该目标，只需要让角色从该边的from到to，这里只需要要求角色向目标
    *  移动
    */ 
    auto tmpRate = m_pOwner->getAttribute().getRate();
    m_pOwner->moveToGridIndex(m_edge.to(), tmpRate);
    
    auto tmpGridMap =   m_pOwner->getMapGrid();
    m_activeTime    =   TimeTool::getSecondTime();
    m_timeExpected  =   m_activeTime + m_marginOfError
        + tmpGridMap->getDistance(m_edge.from(), m_edge.to()) / tmpRate;
}

GoalTraverseEdge::GoalStateEnum GoalTraverseEdge::process()
{
    activateIfInactive();

    // 是否到达目标
    if (isReach())
    {
        terminate();
    }
    else if (isStuck())
    {
        // 是否该角色被阻塞住了
        m_goalState = failed;
    }

    return m_goalState;
}

bool GoalTraverseEdge::isStuck()
{
    return m_timeExpected < TimeTool::getSecondTime();
}

bool GoalTraverseEdge::isReach()
{
    auto tmpGridMap =   m_pOwner->getMapGrid();
    auto tmpPos     =   m_pOwner->getShape()->getPosition();
    auto tmpGrid    =   tmpGridMap->getNodeByIndex(m_edge.to());
    //return (tmpPos.x == tmpGrid.getX()) && (tmpPos.y == tmpGrid.getY());
    // @_@ 这里改为只要和目标有几个个像素以内就算到了，这样可以让动作看起来连贯
    return abs(tmpPos.x - tmpGrid.getX()) <= m_fuzzyReachGap && abs(tmpPos.y - tmpGrid.getY()) <= m_fuzzyReachGap;
}
