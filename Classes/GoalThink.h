#ifndef __GOAL_THINK_H__
#define __GOAL_THINK_H__

#include "GoalComposite.h"

class GameCharacter;

/**
* 属于最高级的目标，在此处会根据当前情况更换当前的目标 
*/
class GoalThink : public GoalComposite<GameCharacter>
{
public:
    GoalThink(GameCharacter* owner);
    ~GoalThink();

    virtual GoalStateEnum process() override;

protected:
    virtual void activate() override;
};

#endif