#ifndef __GAME_CHARACTER_STATE_H__
#define __GAME_CHARACTER_STATE_H__

#include "State.h"
#include "GameCharacter.h"

/**
	 处于idle状态，这个状态存在于刚开始的时候，以及玩家停止操作若干帧之内
*/
class GameCharacterIdleState : public State<GameCharacter>
{
public:
    virtual void onEnter(GameCharacter* owner) override;
    virtual void update(GameCharacter* owner, float dm) override;
    virtual void onExit(GameCharacter* owner) override;
    virtual bool onMessage(GameCharacter* owner, Telegram &msg) override;

    CREATE_FUNC(GameCharacterIdleState);
};

/**
	 角色处于从一个格子移动到另一个格子的过程中
*/
class GameCharacterMovingState : public State<GameCharacter>
{
public:
    /**
    	 移动方向
    */
    enum MovingDirection
    {
        MOVING_DIRECTION_RIGHT,
        MOVING_DIRECTION_LEFT,
        MOVING_DIRECTION_TOP,
        MOVING_DIRECTION_BOTTOM
    };

    MovingDirection movingDirection;                // 移动方向

    virtual void onEnter(GameCharacter* owner) override;
    virtual void update(GameCharacter* owner, float dm) override;
    virtual void onExit(GameCharacter* owner) override;
    virtual bool onMessage(GameCharacter* owner, Telegram &msg) override;

    CREATE_FUNC(GameCharacterMovingState);
};

/**
	 自动随机作战，寻找最近目标，然后移动过去
*/
class GameCharacterAutoState : public State<GameCharacter>
{
public:
    virtual void onEnter(GameCharacter* owner) override;
    virtual void update(GameCharacter* owner, float dm) override;
    virtual void onExit(GameCharacter* owner) override;
    virtual bool onMessage(GameCharacter* owner, Telegram &msg) override;

    CREATE_FUNC(GameCharacterAutoState);

protected:
    /**
    	 返回一个在攻击范围内的对手，如果没有，就返回nullptr
    */
    GameCharacter* getCharacterInAttackDistance(GameCharacter* owner, vector<GameCharacter*>& targets);

    /**
    	 判断target是否在owner的攻击范围内
    */
    bool isInAttackDistance(GameCharacter* owner, GameCharacter* target);
};

/**
	 普通攻击
*/
class GameCharacterNormalAttack : public State<GameCharacter>
{
protected:
    GameCharacterNormalAttack():realAttackFrame(9)
    {
        _executed   =   false;
    }
    bool _executed;                     // 是否已经执行

public:
    int targetId;                       // 攻击目标id
    const int realAttackFrame;          // 真是攻击作用在第几帧

    virtual void onEnter(GameCharacter* owner) override;
    virtual void update(GameCharacter* owner, float dm) override;
    virtual void onExit(GameCharacter* owner) override;
    virtual bool onMessage(GameCharacter* owner, Telegram &msg) override;

    CREATE_FUNC(GameCharacterNormalAttack);
};

/**
	 全局状态
*/
class GameCharacterGlobalState : public State<GameCharacter>
{
public:
    virtual void onEnter(GameCharacter* owner) override;
    virtual void update(GameCharacter* owner, float dm) override;
    virtual void onExit(GameCharacter* owner) override;
    virtual bool onMessage(GameCharacter* owner, Telegram &msg) override;

    CREATE_FUNC(GameCharacterGlobalState);
};

/**
	 追击某个指定角色
*/
class GameCharacterPursueState : public State<GameCharacter>
{
public:
    int targetId;                   // 追击目标

    virtual void onEnter(GameCharacter* owner) override;
    virtual void update(GameCharacter* owner, float dm) override;
    virtual void onExit(GameCharacter* owner) override;
    virtual bool onMessage(GameCharacter* owner, Telegram &msg) override;

    CREATE_FUNC(GameCharacterPursueState);
};

/**
	 进入庆祝胜利状态
*/
class GameCharacterWinState : public State<GameCharacter>
{
public:
    virtual void onEnter(GameCharacter* owner) override;
    virtual void update(GameCharacter* owner, float dm) override;
    virtual void onExit(GameCharacter* owner) override;
    virtual bool onMessage(GameCharacter* owner, Telegram &msg) override;

    CREATE_FUNC(GameCharacterWinState);
};

/**
	 进入死亡状态
*/
class GameCharacterDieState : public State<GameCharacter>
{
public:
    virtual void onEnter(GameCharacter* owner) override;
    virtual void update(GameCharacter* owner, float dm) override;
    virtual void onExit(GameCharacter* owner) override;
    virtual bool onMessage(GameCharacter* owner, Telegram &msg) override;

    CREATE_FUNC(GameCharacterDieState);
};

#endif