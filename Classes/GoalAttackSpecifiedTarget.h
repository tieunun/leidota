#ifndef __GOAL_ATTACK_SPECIFIED_TARGET_H__
#define __GOAL_ATTACK_SPECIFIED_TARGET_H__

#include "GoalComposite.h"
#include "GameCharacter.h"

/**
* 攻击某一个指定角色的目标，会自动完成前进到攻击范围内，并发动攻击，在目标
* 死亡时该目标变为completed，至于使用什么攻击方式，就靠武器系统
*/
class GoalAttackSpecifiedTarget : public GoalComposite<GameCharacter>
{
public:
    GoalAttackSpecifiedTarget(GameCharacter* owner, int targetId);

protected:
    virtual void activate() override;
    virtual GoalStateEnum process() override;

    virtual string getCompositeGoalDescribe() override
    {
        char tmpStr[50];
        sprintf(tmpStr, "GoalAttackSpecifiedTarget : targetId = %d", m_targetId);
        return tmpStr;
    }

private:
    // 判断攻击目标是否还存活
    bool isTargetAlive();

    // 判断目标是否在攻击范围内
    bool isInAttackDistance();

private:
    int m_targetId;                     // 指定的攻击目标
};

#endif