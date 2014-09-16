#ifndef __GOAL_ATTACK_TARGET_H__
#define __GOAL_ATTACK_TARGET_H__

#include <vector>
#include "GoalComposite.h"

using namespace std;

class GameCharacter;

/**
* 直接作为GoalThink的目标的类，属于角色的当前目标级别，这里包含了前进到攻击范围内
* 然后发动攻击的一整套逻辑
*/
class GoalAttackTarget : public GoalComposite<GameCharacter>
{
public:
    /**
    *  设置目标的时候就设置了要攻击的目标id，然后该目标就会完全操纵该角色
    *  去攻击该目标，直到目标死掉或者上层目标主动终止了该目标
    */
    GoalAttackTarget(GameCharacter* owner, int targetId);
    ~GoalAttackTarget();

protected:
    virtual void activate() override;

    virtual GoalStateEnum process() override;

private:
    /**
    * 是否在攻击范围内 
    */
    bool isInAttackRange();

    /**
    * 返回为了攻击该角色而需要前进到哪个格子，这里给的是最佳的目标格子
    */
    int getAdvanceGridIndex();

    /**
    * 根据id返回对象指针 
    */
    GameCharacter* getTarget();

    int             m_targetId;                 // 目标id
};

#endif