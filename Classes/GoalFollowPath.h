#ifndef __GOAL_FOLLOW_PATH_H__
#define __GOAL_FOLLOW_PATH_H__

#include <list>
#include "GoalComposite.h"
#include "NavGraphEdge.h"

using namespace std;

class GameCharacter;

/**
* 角色的沿着某个路径移动的组合目标，就是由多个GoalTraverseEdge组成的 
*/
class GoalFollowPath : public GoalComposite<GameCharacter>
{
public:
    GoalFollowPath(GameCharacter* owner, const list<NavGraphEdge>& path);
    ~GoalFollowPath();

protected:
    virtual void activate() override;

    virtual GoalStateEnum process() override;

private:
    list<NavGraphEdge>  m_path;                     // 该目标要沿的边路径
};

#endif