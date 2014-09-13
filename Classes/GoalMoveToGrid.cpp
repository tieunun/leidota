#include "GoalMoveToGrid.h"
#include "GameCharacter.h"
#include "GoalFollowPath.h"

GoalMoveToGrid::GoalMoveToGrid( GameCharacter* owner, int gridIndex )
    :GoalComposite<GameCharacter>(owner)
{
    m_gridIndex     =   gridIndex;
    m_pathPlanner   =   m_pOwner->getPathPlanner();
}

GoalMoveToGrid::~GoalMoveToGrid()
{

}

void GoalMoveToGrid::activate()
{
    // Ñ°ÕÒÂ·Ïß
    if (!m_pathPlanner->requestPathToGridIndex(m_gridIndex))
    {
        m_goalState =   failed;
    }
    else
    {
        addSubgoal(new GoalFollowPath(m_pOwner, m_pathPlanner->getEdgeListPath()));
    }
}

void GoalMoveToGrid::terminate()
{
    m_goalState = completed;
    m_pOwner->getShape()->playAction(IDLE_ACTION);
}
