#include "GoalAttackSpecifiedTarget.h"
#include "EntityManager.h"
#include "GoalPursuitTarget.h"

GoalAttackSpecifiedTarget::GoalAttackSpecifiedTarget( GameCharacter* owner, int targetId )
    :GoalComposite<GameCharacter>(owner)
{
    m_targetId  =   targetId;
}

void GoalAttackSpecifiedTarget::activate()
{
    
}

GoalStateEnum GoalAttackSpecifiedTarget::process()
{
    do 
    {
        // 如果设定目标已经不在了，就是该目标已经完成
        if (!isTargetAlive())
        {
            m_goalState = completed;
            break;
        }

        // 如果在攻击范围内，就直接使用武器系统发动攻击
        if (isInAttackDistance())
        {
            removeAllSubgoals();
            // 使用当前武器系统攻击
            auto tmpCharacter   =   dynamic_cast<GameCharacter*>(EntityMgr->getEntityFromID(m_targetId));
            m_pOwner->getWeaponControlSystem()->takeWeaponAndAttack(tmpCharacter);
        }
        else
        {
            // 否则就设定先追击该目标
            if (m_subgoalList.empty())
            {
                addSubgoal(new GoalPursuitTarget(m_pOwner, m_targetId));
            }
        }

        // 执行子目标
        m_goalState =   processSubgoals();

    } while (0);

    return m_goalState;
}

bool GoalAttackSpecifiedTarget::isTargetAlive()
{
    auto tmpCharacter   =   dynamic_cast<GameCharacter*>(EntityMgr->getEntityFromID(m_targetId));
    return tmpCharacter != nullptr && tmpCharacter->isAlive();
}

bool GoalAttackSpecifiedTarget::isInAttackDistance()
{
    // 这里需要根据当前的武器来判断是否在攻击范围内
    auto tmpCharacter   =   dynamic_cast<GameCharacter*>(EntityMgr->getEntityFromID(m_targetId));
    return m_pOwner->getWeaponControlSystem()->isInAttackRange(tmpCharacter);
}
