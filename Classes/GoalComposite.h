#ifndef __GOAL_COMPOSITE_H__
#define __GOAL_COMPOSITE_H__

#include <list>
#include <assert.h>
#include "Goal.h"

using namespace std;

/**
* 组合目标类，该目标由多个子目标组成 
*/
template <class entity_type>
class GoalComposite : public Goal<entity_type>
{
public:
    /**
    * 是否拥有子目标 
    */
    bool hasSubgoal()
    {
        return m_subgoalList.size() > 0;
    }

    /**
    * 处理消息 
    */
    virtual bool handleMessage(Telegram& msg) override
    {
        return forwardMessageToFrontMostSubgoal(msg);
    }

    /**
    *	返回当前最前面的目标描述
    */
    virtual string getGoalDescribe() override
    {
        if (m_subgoalList.size() > 0)
        {
            return m_subgoalList.front()->getGoalDescribe();
        }
        else
        {
            return getCompositeGoalDescribe();
        }
    }

protected:
    GoalComposite(entity_type* owner):Goal<entity_type>(owner)
    {

    }

    virtual ~GoalComposite()
    {
        removeAllSubgoals();
    }

    virtual void activate() = 0;

    /**
    * 对于组合的情况，默认的process就只是处理子目标 
    */
    virtual GoalStateEnum process()
    {
        this->activateIfInactive();

        this->m_goalState = processSubgoals();

        // 默认情况下如果失败，就删除所有的子目标
        if (this->m_goalState == failed)
        {
            removeAllSubgoals();
        }
        return this->m_goalState;
    }

protected:
    /**
    * 在后面追加目标对象 
    */
    virtual void addSubgoal(Goal<entity_type>* goal) override
    {
        assert(goal != nullptr && goal != this);

        m_subgoalList.push_back(goal);
    }

    /**
    * 从前面追加目标对象
    */
    virtual void pushSubgoal(Goal<entity_type>* goal) override
    {
        assert(goal != nullptr && goal != this);

        m_subgoalList.push_front(goal);
    }

    /**
    * 移除该目标的所有子目标
    */
    void removeAllSubgoals()
    {
        auto tmpIterator = m_subgoalList.begin();
        for (; tmpIterator != m_subgoalList.end(); tmpIterator++)
        {
            (*tmpIterator)->terminate();

            // 如果该目标也是组合目标，就会在构造函数中去结束掉它的所有子目标
            delete *tmpIterator;
        }

        m_subgoalList.clear();
    }

    /**
    * 处理子目标，并返回子目标的处理结果，比如所有子目标处理完成，就会返回completed，如果有子目标失败
    * 就会返回失败，如果某个子目标完成，但是还剩下子目标，就会返回activate表示子目标仍然活跃
    */
    GoalStateEnum processSubgoals()
    {
        // 删除已经完成的子目标，@_@ 之所以会这样，是因为可以在子目标内部逻辑中通过将自己设置为completed来结束自己
        // 这里再添加一个就是也删除失败的目标，因为可能会在子目标失败后，父目标重新规划子目标的时候忘记了removeAllSubgoals
        while (m_subgoalList.size() > 0 && m_subgoalList.front()->isComplete())
        {
            m_subgoalList.front()->terminate();
            delete m_subgoalList.front();
            m_subgoalList.pop_front();
        }

        // 如果当前子目标没有了，那么就是已经完成
        if (m_subgoalList.empty())
        {
            return completed;
        }

        // 处理第一个子目标
        GoalStateEnum tmpGoalState = m_subgoalList.front()->process();
        if (tmpGoalState == completed)
        {
            // 如果该子目标执行成功，就会删除
            m_subgoalList.front()->terminate();
            delete m_subgoalList.front();
            m_subgoalList.pop_front();

            if (m_subgoalList.empty())
            {
                // 如果所有的子目标已经解决完毕
                return completed;
            }
            else
            {
                // 表示该目标仍然在活跃中
                return active;
            }
        }

        return tmpGoalState;
    }

    /**
    * 将消息分派给最前面的子目标 
    */
    bool forwardMessageToFrontMostSubgoal(Telegram& msg)
    {
        if (!m_subgoalList.empty())
        {
            return m_subgoalList.front()->handleMessage(msg);
        }
        return false;
    }

protected:
    virtual string getCompositeGoalDescribe()
    {
        return "";
    }
    typedef list<Goal<entity_type>*>    SubgoalList;

    SubgoalList     m_subgoalList;                          // 子目标
};

#endif