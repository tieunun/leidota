#include "GoalAttackTarget.h"
#include "GameCharacter.h"
#include "PathPlanner.h"
#include "EntityManager.h"
#include "GoalMoveToGrid.h"

GoalAttackTarget::GoalAttackTarget( GameCharacter* owner, int targetId )
    :GoalComposite<GameCharacter>(owner)
{
    m_targetId      =   targetId;
}

GoalAttackTarget::~GoalAttackTarget()
{

}

void GoalAttackTarget::activate()
{
    
}

GoalStateEnum GoalAttackTarget::process()
{
    activateIfInactive();

    do 
    {
        // 查看对手是否还存在
        if (EntityMgr->getEntityFromID(m_targetId) == nullptr)
        {
            m_goalState =   completed;
            break;
        }

        // 如果在攻击距离内
        if (isInAttackRange())
        {
            // 可以直接攻击
            m_pOwner->getWeaponControlSystem()->takeWeaponAndAttack(getTarget());
        }
        else
        {
            // 增加一个前进到指定格子的子目标
            addSubgoal(new GoalMoveToGrid(m_pOwner, getAdvanceGridIndex()));
        }
    } while (0);
    
    return m_goalState;
}

bool GoalAttackTarget::isInAttackRange()
{
    // @_@ 先这样写吧
    return true;
}

int GoalAttackTarget::getAdvanceGridIndex()
{
    // @_@ 先这样写吧
    return 1;
}

GameCharacter* GoalAttackTarget::getTarget()
{
    return dynamic_cast<GameCharacter*>(EntityMgr->getEntityFromID(m_targetId));
}
