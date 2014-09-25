#ifndef __GOAL_PURSUIT_TARGET_H__
#define __GOAL_PURSUIT_TARGET_H__

#include "Goal.h"
#include "GameCharacter.h"
#include "EntityManager.h"

/**
* 追击一个指定目标的原子目标，直到该目标消失
*/
class GoalPursuitTarget : public Goal<GameCharacter>
{
public:
    GoalPursuitTarget(GameCharacter* owner, int targetId):Goal<GameCharacter>(owner)
    {
        m_targetId  =   targetId;
    }

    virtual string getGoalDescribe()
    {
        char tmpStr[40];
        sprintf(tmpStr, "GoalPursuitTarget : targetId = %d", m_targetId);
        return tmpStr;
    }

protected:
    virtual void activate() override
    {
        // @_@ 因为这是角色目标，所有允许直接修改角色的驱动行为
        m_pOwner->getSteeringBehaviros()->setTargetId(m_targetId);
        m_pOwner->getSteeringBehaviros()->pursuitOn();
    }

    virtual GoalStateEnum process() override
    {
        activateIfInactive();

        // 直到角色不存在，就认定该目标失败
        if (!isTargetAlive())
        {
            m_goalState = failed;
        }

        return m_goalState;
    }

    virtual void terminate() override
    {
        m_pOwner->getSteeringBehaviros()->pursuitOff();
    }

private:
    bool isTargetAlive()
    {
        auto tmpCharacter   =   dynamic_cast<GameCharacter*>(EntityMgr->getEntityFromID(m_targetId));
        return tmpCharacter != nullptr && tmpCharacter->isAlive();
    }

    int     m_targetId;                 // 要追击的目标
};

#endif