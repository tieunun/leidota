#ifndef __GOAL_TEAM_ATTACK_TARGET_TEAM_H__
#define __GOAL_TEAM_ATTACK_TARGET_TEAM_H__

#include "GoalComposite.h"
#include "GameTeam.h"

/**
*  队伍的高级目标之一，攻击目标队伍，如果队伍距离太远，会自动增加队伍集体前进的子目标，同时
*  如果该目标处于激活状态，就会不断地遍历该队伍的所有成员，依次给这些队员发送消息制定目标
*/
class GoalTeamAttackTargetTeam : public GoalComposite<GameTeam>
{
public:
    GoalTeamAttackTargetTeam(GameTeam* owner, GameTeam* target);

protected:
    // @_@ 暂时没啥用的
    virtual void activate() override;
    
    // @_@ 在这里面要不断地根据我方人员、敌方人员情况给自己队员发送消息
    virtual GoalStateEnum process() override;

private:
    // 检查每一个队员，并在需要的时候给他们发送消息
    void inspectTeamMembers();

    GameTeam*       m_targetTeam;               // 目标队伍

    /**
    *	因为队伍级AI给角色分配的目标是按照固定模式的，所以以二元数组的方式保存
    */
    vector<vector<int>> m_targetRule;
};

#endif