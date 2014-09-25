#ifndef __GOAL_TEAM_RECOVER_FORMATION_H__
#define __GOAL_TEAM_RECOVER_FORMATION_H__

#include "Goal.h"
#include "GameTeam.h"
#include "Telegram.h"
#include "GameCharacter.h"
#include "TimeTool.h"

/**
* 队伍恢复当前阵型的目标 
*/
class GoalTeamRecoverFormation : public Goal<GameTeam>
{
public:
    GoalTeamRecoverFormation(GameTeam* owner, float timeOut = 4):Goal<GameTeam>(owner)
    {
        m_timeOut   =   timeOut;
    }

protected:
    virtual void activate() override
    {
        // 直接把队员安排在正确的位置上
        auto tmpFormation   =   m_pOwner->getTeamFormation();
        auto tmpMemebers    =   m_pOwner->getMembers();
        for (auto tmpIterator = tmpMemebers.begin(); tmpIterator != tmpMemebers.end(); tmpIterator++)
        {
            GameCharacter* tmpCharacter        =   *tmpIterator;
            MovingEntity& tmpMovingEntity      =   tmpCharacter->getMovingEntity();
            tmpMovingEntity.setPosition(tmpFormation.getPositionByPosId(tmpMovingEntity.getFormationPosId()));
        }
    }

    virtual GoalStateEnum process() override
    {
        activateIfInactive();
        return completed;
    }

private:
    double          m_activeTime;
    float           m_timeOut;
};

#endif