#ifndef __GOAL_H__
#define __GOAL_H__

#include <assert.h>
#include "Telegram.h"

template <class entity_type>
class GoalComposite;

/**
* 当前目标的状态的枚举
*/
enum GoalStateEnum
{
    active,
    inactive,
    completed,
    failed
};

/**
*  所有目标的基类，这个是原子目标，不能存在子目标，该目标应该有能力去回调实体的接口
*  来帮助实体完成该目标
*/
template <class entity_type>
class Goal
{
public:

    /**
    * 对当前状态的判断 
    */
    bool isComplete()const{return m_goalState == completed;} 
    bool isActive()const{return m_goalState == active;}
    bool isInactive()const{return m_goalState == inactive;}
    bool hasFailed()const{return m_goalState == failed;}

    /**
    * 目标也有处理消息的能力 
    */
    virtual bool handleMessage(Telegram& msg) { return false; }

protected:
    Goal(entity_type* owner)
    {
        m_pOwner    =   owner;
        m_goalState =   inactive;
    }

    virtual ~Goal()
    {

    }

    /**
    * 为了方便的在只有处于未激活状态的时候才调用activate的
    */
    void activateIfInactive()
    {
        if (isInactive())
        {
            m_goalState = active;
            activate();
        }
    }

    /**
    *  激活该目标的逻辑，也就是开始按照该目标的要去引导角色实现该目标
    */
    virtual void activate() = 0;

    /**
    * 定时处理该目标，这里会返回该目标的当前状态，比如该目标 
    */
    virtual GoalStateEnum process() = 0;

    /**
    * 该目标结束的时候会调用的，用来取消当前目标等
    */
    virtual void terminate() {m_goalState = completed;}

    /**
    * 原子目标是无法有子目标的，但是为了保证与GoalComposite统一的接口
    * 在后面追加目标
    */
    virtual void addSubgoal(Goal<entity_type>* goal)
    {
        assert(false && "Cannot add goals to atomic goals");
    }

    /**
    * 从前面给加入子目标对象 
    */
    virtual void pushSubgoal(Goal<entity_type>* goal)
    {
        assert(false && "Cannot push goals to atomic goals");
    }

    friend class GoalComposite<entity_type>;

protected:
    GoalStateEnum       m_goalState;                    // 目标状态
    entity_type*        m_pOwner;                       // 目标的拥有者
};

#endif