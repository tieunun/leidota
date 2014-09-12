#include "GoalFollowPath.h"
#include "GoalTraverseEdge.h"

GoalFollowPath::GoalFollowPath( GameCharacter* owner, const list<NavGraphEdge>& path )
    :GoalComposite<GameCharacter>(owner)
{
    m_path  =   path;
}

GoalFollowPath::~GoalFollowPath()
{

}

void GoalFollowPath::activate()
{
    // @_@ 这里先一次把所有的目标创建出来
    for (auto tmpIterator = m_path.rbegin(); tmpIterator != m_path.rend(); tmpIterator++)
    {
        addSubgoal(new GoalTraverseEdge(m_pOwner, *tmpIterator));
    }
}

GoalFollowPath::GoalStateEnum GoalFollowPath::process()
{
    /**
    *  这里直接使用默认的，就是只要在角色沿着边移动的过程中出错了，该目标也会报错
    *  然后期望上一级目标放弃本巡线目标
    */ 
    return GoalComposite<GameCharacter>::process();
}