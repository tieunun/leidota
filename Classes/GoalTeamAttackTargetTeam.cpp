#include "GoalTeamAttackTargetTeam.h"
#include "GameCharacter.h"
#include "MessageDispatcher.h"

GoalTeamAttackTargetTeam::GoalTeamAttackTargetTeam( GameTeam* owner, GameTeam* target ) :GoalComposite<GameTeam>(owner)
{
    m_targetTeam    =   target;
}

void GoalTeamAttackTargetTeam::activate()
{

}

GoalStateEnum GoalTeamAttackTargetTeam::process()
{
    // 这里做的应该是纵观大局，随时给首先分配任务，直到对方队伍消灭才改为completed
    activateIfInactive();
    inspectTeamMembers();
    return m_goalState;
}

void GoalTeamAttackTargetTeam::inspectTeamMembers()
{
    // @_@ 先写一个简单的规则
    /**
    GameCharacter* tmpOwnMem    =   *m_pOwner->getMembers().begin();
    GameCharacter* tmpTarget    =   *m_targetTeam->getMembers().begin();
    if (!tmpOwnMem->hasGoal())
    {
        // 给这个角色发送一个消息，就是指定它去攻击目标
        auto tmpMsg = Telegram::create(0, tmpOwnMem->getId(), TELEGRAM_ENUM_TEAM_ATTACK_SPECIFIED_TARGET, 0, (void*)tmpTarget->getId());
        m_pOwner->sendMessageToOneMember(*tmpMsg, tmpOwnMem);
    }
    if (m_pOwner->getMembers().size() > 1)
    {
        auto tmpIterator            =   m_pOwner->getMembers().begin();
        tmpIterator++;
        GameCharacter* tmpSecMem    =   *tmpIterator;
        if (!tmpSecMem->hasGoal())
        {
            auto tmpMsg = Telegram::create(0, tmpSecMem->getId(), TELEGRAM_ENUM_TEAM_ATTACK_SPECIFIED_TARGET, 0, (void*)tmpTarget->getId());
            m_pOwner->sendMessageToOneMember(*tmpMsg, tmpSecMem);
        }
    }
    */
    /**
    * 接下来按照攻击顺序依次给队员设置攻击目标 
    */
    auto tmpOwnMembers      =   m_pOwner->getMembers();
    auto tmpTargetMembers   =   m_targetTeam->getMembers();
    // 开始遍历我方角色
    for (auto tmpIterator = tmpOwnMembers.begin(); tmpIterator != tmpOwnMembers.end(); tmpIterator++)
    {
        auto tmpCharacter = dynamic_cast<GameCharacter*>(*tmpIterator);
        if (tmpCharacter->getMovingEntity().getFormationPosId() == 0)
        {

        }
    }
}