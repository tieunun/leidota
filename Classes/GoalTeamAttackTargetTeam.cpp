#include "GoalTeamAttackTargetTeam.h"
#include "GameCharacter.h"
#include "MessageDispatcher.h"

GoalTeamAttackTargetTeam::GoalTeamAttackTargetTeam( GameTeam* owner, GameTeam* target ) :GoalComposite<GameTeam>(owner)
{
    m_targetTeam    =   target;
    
    // 0号位
    m_targetRule.push_back(vector<int>());
    m_targetRule[0].push_back(0);
    m_targetRule[0].push_back(1);
    m_targetRule[0].push_back(2);
    m_targetRule[0].push_back(3);
    m_targetRule[0].push_back(4);
    m_targetRule[0].push_back(5);

    // 1号位
    m_targetRule.push_back(vector<int>());
    m_targetRule[1].push_back(1);
    m_targetRule[1].push_back(0);
    m_targetRule[1].push_back(3);
    m_targetRule[1].push_back(2);
    m_targetRule[1].push_back(5);
    m_targetRule[1].push_back(4);

    // 2号位
    m_targetRule.push_back(vector<int>());
    m_targetRule[2].push_back(2);
    m_targetRule[2].push_back(1);
    m_targetRule[2].push_back(0);
    m_targetRule[2].push_back(3);
    m_targetRule[2].push_back(4);
    m_targetRule[2].push_back(5);

    // 3号位
    m_targetRule.push_back(vector<int>());
    m_targetRule[3].push_back(3);
    m_targetRule[3].push_back(1);
    m_targetRule[3].push_back(0);
    m_targetRule[3].push_back(2);
    m_targetRule[3].push_back(5);
    m_targetRule[3].push_back(4);

    // 4号位
    m_targetRule.push_back(vector<int>());
    m_targetRule[4].push_back(2);
    m_targetRule[4].push_back(3);
    m_targetRule[4].push_back(0);
    m_targetRule[4].push_back(2);
    m_targetRule[4].push_back(5);
    m_targetRule[4].push_back(4);

    // 5号位
    m_targetRule.push_back(vector<int>());
    m_targetRule[5].push_back(5);
    m_targetRule[5].push_back(1);
    m_targetRule[5].push_back(0);
    m_targetRule[5].push_back(2);
    m_targetRule[5].push_back(3);
    m_targetRule[5].push_back(4);
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
    /**
    * 接下来按照攻击顺序依次给队员设置攻击目标 
    */
    auto tmpOwnMembers      =   m_pOwner->getMembers();
    auto tmpTargetMembers   =   m_targetTeam->getMembers();
    // 开始遍历我方角色
    for (auto tmpIterator = tmpOwnMembers.begin(); tmpIterator != tmpOwnMembers.end(); tmpIterator++)
    {
        auto tmpOwnCharacter    =   dynamic_cast<GameCharacter*>(*tmpIterator);
        if (tmpOwnCharacter->hasGoal())
        {
            // 当前有目标的队员就不用管
            continue;
        }
        auto tmpRule            =   m_targetRule[tmpOwnCharacter->getMovingEntity().getFormationPosId()];
        GameCharacter* tmpTarget=   nullptr;
        for (auto tmpPosIterator = tmpRule.begin(); tmpPosIterator != tmpRule.end(); tmpPosIterator++)
        {
            // 从规则最前面寻找第一个坑位有人的
            tmpTarget   =   m_targetTeam->getMemberIdFromFormation(*tmpPosIterator);
            if (tmpTarget != nullptr)
            {
                break;
            }
        }

        if (tmpTarget != nullptr)
        {
            // 发送消息告知攻击
            auto tmpMsg = Telegram::create(0, tmpOwnCharacter->getId(),
                TELEGRAM_ENUM_TEAM_ATTACK_SPECIFIED_TARGET, 0, (void*)tmpTarget->getId());
            m_pOwner->sendMessageToOneMember(*tmpMsg, tmpOwnCharacter);
        }
    }
}