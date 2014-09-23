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
        // 通知大家保持阵型
        auto tmpMsg = Telegram::create(0, 0, TELEGRAM_ENUM_TEAM_COLLECTIVE_FORWARD);
        m_pOwner->sendMessageToAllMember(*tmpMsg);
        m_activeTime    =   TimeTool::getSecondTime();
    }

    virtual GoalStateEnum process() override
    {
        activateIfInactive();

        // 当所有人的速度变为0的时候就是完成了
        if (m_pOwner->isEveryMemberInPos() || TimeTool::getSecondTime() - m_activeTime >= m_timeOut)
        {
            return completed;
        }

        return active;
    }

    virtual void terminate() override
    {
        auto tmpMsg = Telegram::create(0, 0, TELEGRAM_ENUM_TEAM_CANCEL_COLLECTIVE_FORWARD);
        m_pOwner->sendMessageToAllMember(*tmpMsg);
    }

private:
    double          m_activeTime;
    float           m_timeOut;
};

#endif