#ifndef __GOAL_TEAM_ADVANCE_H__
#define __GOAL_TEAM_ADVANCE_H__

#include "Goal.h"
#include "GameTeam.h"

/**
* 队伍集体向前移动的目标
* @_@ 这里只管队伍推进就OK了，不需要管什么当前的环境
*/
class GoalTeamAdvance : public Goal<GameTeam>
{
public:
    GoalTeamAdvance( GameTeam* owner ):Goal<GameTeam>(owner){}

protected:
    virtual void activate() override
    {
        m_pOwner->collectiveForwardStart();
    }

    virtual GoalStateEnum process() override
    {
        activateIfInactive();
        return m_goalState;
    }

    virtual void terminate() override
    {
        m_pOwner->collectiveForwardEnd();
    }
};

#endif