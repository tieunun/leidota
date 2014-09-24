#ifndef __GOAL_TEAM_ADVANCE_H__
#define __GOAL_TEAM_ADVANCE_H__

#include "Goal.h"
#include "GameTeam.h"
#include "MathTool.h"

/**
* 队伍集体向前移动的目标，移动到指定的x位置
* @_@ 这里只管队伍推进就OK了，不需要管什么当前的环境
*/
class GoalTeamAdvance : public Goal<GameTeam>
{
public:
    GoalTeamAdvance( GameTeam* owner, float xPos ):Goal<GameTeam>(owner)
    {
        m_xPos  =   xPos;
    }

protected:
    virtual void activate() override
    {
        m_pOwner->collectiveForwardStart();
    }

    virtual GoalStateEnum process() override
    {
        activateIfInactive();

        // 当前进到位置后并摆好阵后结束该目标
        if (abs(m_pOwner->getTeamFormation().getFormationAnchor().x - m_xPos) <= 30)
        {
            m_goalState =   completed;
        }

        return m_goalState;
    }

    virtual void terminate() override
    {
        m_pOwner->collectiveForwardEnd();
    }

private:
    float       m_xPos;
};

#endif