#ifndef __GOAL_TRAVERSE_EDGE_H__
#define __GOAL_TRAVERSE_EDGE_H__

#include "Goal.h"
#include "NavGraphEdge.h"

class GameCharacter;

/**
*  用来沿着某一条边移动的原子目标
*/
class GoalTraverseEdge : public Goal<GameCharacter>
{
public:
    GoalTraverseEdge(GameCharacter* owner, const NavGraphEdge& edge);
    ~GoalTraverseEdge();

protected:
    virtual void activate() override;

    virtual GoalStateEnum process() override;

private:
    /**
    * 判断该角色是否已经到达该边的目标 
    */
    bool isReach();

    /**
    * 判断是否卡住了 
    */
    bool isStuck();

    NavGraphEdge    m_edge;                 // 该目标要实现的沿某条边移动
    
    /**
    * 因为可能会出现之前先规划了一条路线，但是最后因为意外无法继续了，这里
    * 就通过限定完成该目标的时间来限定，如果在规定的时间内没有到达目标，就
    * 判定为该目标执行失败
    */
    float           m_activeTime;           // 调用activate的当前时间
    float           m_timeExpected;         // 期望的到达时间

    const float     m_marginOfError;        // 给一些意外的时间
};

#endif