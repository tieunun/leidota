#include "GoalAttackTarget.h"
#include "GameCharacter.h"
#include "EntityManager.h"

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
