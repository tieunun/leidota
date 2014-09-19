#ifndef __GOAL_TEAM_THINK_H__
#define __GOAL_TEAM_THINK_H__

#include "GoalComposite.h"
#include "GoalTeamAdvance.h"
#include "GoalTeamAttackTargetTeam.h"
#include "TeamManager.h"

class GameTeam;

/**
* 属于队伍级别的最高目标，在此处会对环境进行判断，然后设置当前队伍的合适目标 
*/
class GoalTeamThink : public GoalComposite<GameTeam>
{
public:
    GoalTeamThink(GameTeam* owner):GoalComposite<GameTeam>(owner){}

    // @_@ 临时使用的，方便外部设置目标
    void setGoal(Goal* aGoal)
    {
        removeAllSubgoals();
        addSubgoal(aGoal);
    }

    virtual void activate() override
    {
        //@_@ 临时这样写，一开始就给队伍一个集体前进的目标
        // addSubgoal(new GoalTeamAdvance(m_pOwner));
        // @_@ 临时这样写，进攻
        // addSubgoal(new GoalTeamAttackTargetTeam(m_pOwner, TeamMgr->getTeamFromId(1)));
    }

    virtual GoalStateEnum process() override
    {
        return GoalComposite<GameTeam>::process();
    }
};

#endif