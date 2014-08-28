#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include "State.h"

/**
	 状态机，每一个游戏实体都会有一个状态机，状态机负责管理该游戏实体的状态更新、转换等
*/
template <class entity_type>
class StateMachine : public cocos2d::Ref
{
protected:
    entity_type *_owner;                        // 该状态机关联的实体
    State<entity_type>* _globalState;           // 全局状态
    State<entity_type>* _currentState;          // 当前的状态
    State<entity_type>* _previousState;         // 之前的状态，用来翻转的

protected:
    StateMachine(entity_type *owner)
    {
        _owner          =   owner;
        _currentState   =   nullptr;
        _globalState    =   nullptr;
        _previousState  =   nullptr;
    }

    ~StateMachine()
    {
        CC_SAFE_RELEASE_NULL(_currentState);
        CC_SAFE_RELEASE_NULL(_globalState);
        CC_SAFE_RELEASE_NULL(_previousState);
    }

public:
    State<entity_type>* getCurrentState()
    {
        return _currentState;
    }

    /**
    	 更新
    */
    void update(float dm)
    {
        if (_globalState != nullptr)
        {
            _globalState->update(_owner, dm);
        }

        if (_currentState != nullptr)
        {
            _currentState->update(_owner, dm);
        }
    }

    /**
    	 更改当前状态
    */
    void changeState(State<entity_type> *state)
    {
        CC_SAFE_RELEASE_NULL(_previousState);

        if (_currentState != nullptr)
        {
            _currentState->onExit(_owner);
        }
        
        _previousState  =   _currentState;
        _currentState   =   state;

        _currentState->retain();
        _currentState->onEnter(_owner);
    }

    /**
    	 切换到之前的状态
    */
    void reverseState()
    {
        if (_previousState == nullptr)
        {
            return;
        }

        _previousState->retain();
        changeState(_previousState);
    }

    /**
    	 处理消息
    */
    bool handleMessage(Telegram& msg)
    {
        if (_currentState != nullptr && _currentState->onMessage(_owner, msg))
        {
            return true;
        }

        if (_globalState != nullptr && _globalState->onMessage(_owner, msg))
        {
            return true;
        }

        return false;
    }

    /**
    	
    */
    void setGlobalState(State<entity_type>* globalState)
    {
        _globalState    =   globalState;
        _globalState->retain();
    }

    /**
    	 使用该函数来创建状态机
    */
    static StateMachine<entity_type> *create(entity_type *entity)
    {
        StateMachine<entity_type> *pRet = new StateMachine<entity_type>(entity);
        if (pRet != nullptr)
        {
            pRet->autorelease();
        }

        return pRet;
    }
};

#endif