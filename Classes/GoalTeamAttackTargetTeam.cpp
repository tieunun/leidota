#include "GoalTeamAttackTargetTeam.h"
#include "GoalTeamAdvance.h"
#include "GameCharacter.h"
#include "MessageDispatcher.h"
#include "TeamManager.h"
#include "MathTool.h"

GoalTeamAttackTargetTeam::GoalTeamAttackTargetTeam( GameTeam* owner, GameTeam* target ) 
    :GoalComposite<GameTeam>(owner), m_attDistance(800)
{
    m_targetTeam    =   target;
    m_targetId      =   target->getTeamId();
    
    // 0号位
    m_targetRule.push_back(vector<int>());
    m_targetRule[0].push_back(0);
    m_targetRule[0].push_back(1);
    m_targetRule[0].push_back(2);
    m_targetRule[0].push_back(3);
    m_targetRule[0].push_back(4);
    m_targetRule[0].push_back(5);
    m_targetRule[0].push_back(6);
    m_targetRule[0].push_back(7);
    m_targetRule[0].push_back(8);

    // 1号位
    m_targetRule.push_back(vector<int>());
    m_targetRule[1].push_back(1);
    m_targetRule[1].push_back(0);
    m_targetRule[1].push_back(2);
    m_targetRule[1].push_back(4);
    m_targetRule[1].push_back(3);
    m_targetRule[1].push_back(5);
    m_targetRule[1].push_back(7);
    m_targetRule[1].push_back(6);
    m_targetRule[1].push_back(8);

    // 2号位
    m_targetRule.push_back(vector<int>());
    m_targetRule[2].push_back(2);
    m_targetRule[2].push_back(1);
    m_targetRule[2].push_back(0);
    m_targetRule[2].push_back(5);
    m_targetRule[2].push_back(4);
    m_targetRule[2].push_back(3);
    m_targetRule[2].push_back(8);
    m_targetRule[2].push_back(7);
    m_targetRule[2].push_back(6);

    // 3号位
    m_targetRule.push_back(vector<int>());
    m_targetRule[3].push_back(0);
    m_targetRule[3].push_back(1);
    m_targetRule[3].push_back(2);
    m_targetRule[3].push_back(3);
    m_targetRule[3].push_back(4);
    m_targetRule[3].push_back(5);
    m_targetRule[3].push_back(6);
    m_targetRule[3].push_back(7);
    m_targetRule[3].push_back(8);

    // 4号位
    m_targetRule.push_back(vector<int>());
    m_targetRule[4].push_back(1);
    m_targetRule[4].push_back(0);
    m_targetRule[4].push_back(2);
    m_targetRule[4].push_back(4);
    m_targetRule[4].push_back(3);
    m_targetRule[4].push_back(5);
    m_targetRule[4].push_back(7);
    m_targetRule[4].push_back(6);
    m_targetRule[4].push_back(8);

    // 5号位
    m_targetRule.push_back(vector<int>());
    m_targetRule[5].push_back(2);
    m_targetRule[5].push_back(1);
    m_targetRule[5].push_back(0);
    m_targetRule[5].push_back(5);
    m_targetRule[5].push_back(4);
    m_targetRule[5].push_back(3);
    m_targetRule[5].push_back(8);
    m_targetRule[5].push_back(7);
    m_targetRule[5].push_back(6);

    // 6号位
    m_targetRule.push_back(vector<int>());
    m_targetRule[6].push_back(0);
    m_targetRule[6].push_back(1);
    m_targetRule[6].push_back(2);
    m_targetRule[6].push_back(3);
    m_targetRule[6].push_back(4);
    m_targetRule[6].push_back(5);
    m_targetRule[6].push_back(6);
    m_targetRule[6].push_back(8);

    // 7号位
    m_targetRule.push_back(vector<int>());
    m_targetRule[7].push_back(1);
    m_targetRule[7].push_back(0);
    m_targetRule[7].push_back(2);
    m_targetRule[7].push_back(4);
    m_targetRule[7].push_back(3);
    m_targetRule[7].push_back(5);
    m_targetRule[7].push_back(7);
    m_targetRule[7].push_back(6);
    m_targetRule[7].push_back(8);

    // 8号位
    m_targetRule.push_back(vector<int>());
    m_targetRule[8].push_back(2);
    m_targetRule[8].push_back(1);
    m_targetRule[8].push_back(0);
    m_targetRule[8].push_back(5);
    m_targetRule[8].push_back(4);
    m_targetRule[8].push_back(3);
    m_targetRule[8].push_back(8);
    m_targetRule[8].push_back(7);
    m_targetRule[8].push_back(6);
}

void GoalTeamAttackTargetTeam::activate()
{
    // 判断距离，如果两个部队距离太远，就让该部队起步走到离地方一定距离的地方
    auto tmpOwnerXPos   =   m_pOwner->getTeamFormation().getFormationAnchor().x;
    auto tmpTargetXPos  =   m_targetTeam->getTeamFormation().getFormationAnchor().x;
    if (abs(tmpTargetXPos - tmpOwnerXPos) > m_attDistance)
    {
        if (m_pOwner->getTeamFormation().getFormationType() == Formation::FORMATION_TYPE_RIGHT)
        {
            addSubgoal(new GoalTeamAdvance(m_pOwner, tmpTargetXPos - m_attDistance));
        }
        else if (m_pOwner->getTeamFormation().getFormationType() == Formation::FORMATION_TYPE_LEFT)
        {
            addSubgoal(new GoalTeamAdvance(m_pOwner, tmpTargetXPos + m_attDistance));
        }
    }
}

GoalStateEnum GoalTeamAttackTargetTeam::process()
{
    // 这里做的应该是纵观大局，随时给首先分配任务，直到对方队伍消灭才改为completed
    activateIfInactive();
    
    // 当子目标结束后才会开战
    if (processSubgoals() == completed)
    {
        inspectTeamMembers();
    }

    return m_goalState;
}

void GoalTeamAttackTargetTeam::inspectTeamMembers()
{
    // 判断是否有队伍失败了
    if (isWin())
    {
        // 目标结束
        m_goalState =   completed;
        return;
    }

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

bool GoalTeamAttackTargetTeam::isWin()
{
    // 敌方队伍消失
    return TeamMgr->getTeamFromId(m_targetId) == nullptr;
}
