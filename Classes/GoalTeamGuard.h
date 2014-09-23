#ifndef __GOAL_TEAM_GUARD_H__
#define __GOAL_TEAM_GUARD_H__

#include "GoalComposite.h"
#include "GameTeam.h"
#include "GoalTeamAttackTargetTeam.h"

/**
*	警戒目标，当玩家目标靠近一定距离后就发起攻击 
*/
class GoalTeamGuard : public GoalComposite<GameTeam>
{
public:
    GoalTeamGuard(GameTeam* owner):GoalComposite<GameTeam>(owner), m_guardDistance(700)
    {

    }

protected:
    virtual void activate() override
    {

    }

    virtual GoalStateEnum process() override
    {
        activateIfInactive();

        auto tmpTarget  =   getTeamInGuradDistance();
        if (tmpTarget != nullptr && m_subgoalList.size() == 0)
        {
            addSubgoal(new GoalTeamAttackTargetTeam(m_pOwner, tmpTarget));
        }

        processSubgoals();
        return active;
    }

private:
    // 如果有玩家队伍在他的警戒范围内，就返回，否则返回null
    GameTeam* getTeamInGuradDistance()
    {
        // @_@ 这里暂时简单假设玩家队伍是0
        GameTeam* tmpPlayerTeam =   TeamMgr->getTeamFromId(0);
        if (tmpPlayerTeam != nullptr)
        {
            auto tmpDistance    =   m_pOwner->getTeamFormation().getFormationAnchor().x - 
                tmpPlayerTeam->getTeamFormation().getFormationAnchor().x;
            if (tmpDistance <= m_guardDistance)
            {
                return tmpPlayerTeam;
            }
        }

        return nullptr;
    }

    const float m_guardDistance;            // 警戒距离
};

#endif