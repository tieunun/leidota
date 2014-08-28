#ifndef __TEAM_STATE_H__
#define __TEAM_STATE_H__

#include "State.h"
#include "GameTeam.h"

/**
	 此处主要保存了队伍级别的AI状态，包括比如：自由进攻、集火、该关卡升级向后推进、赢得胜利
*/

/**
	 队伍处于等待，通知所有的角色处于idle状态，等待若干帧，然后就会进入自由战斗
*/
class GameTeamWaitState : public State<GameTeam>
{
protected:
    GameTeamWaitState():m_waitFrame(30)
    {
        m_frameCount    =   0;
    }

    const int m_waitFrame;
    int       m_frameCount;  
public:
    virtual void onEnter(GameTeam* owner) override;
    virtual void update(GameTeam* owner, float dm) override;
    virtual void onExit(GameTeam* owner) override {m_frameCount = 0;}
    virtual bool onMessage(GameTeam* owner, Telegram &msg) override {return false;}

    CREATE_FUNC(GameTeamWaitState);
};

/**
	 队伍处于自由战斗状态，队员之间没有任何配合
*/
class GameTeamFreeCombatState : public State<GameTeam>
{
public:
    virtual void onEnter(GameTeam* owner) override;
    virtual void update(GameTeam* owner, float dm) override;
    virtual void onExit(GameTeam* owner) override;
    virtual bool onMessage(GameTeam* owner, Telegram &msg) override;

    CREATE_FUNC(GameTeamFreeCombatState);
};

/**
	 队伍全局的状态，主要进行一些队伍级别的判断
*/
class GameTeamGlobalState : public State<GameTeam>
{
public:
    virtual void onEnter(GameTeam* owner) override {};
    virtual void update(GameTeam* owner, float dm) override;
    virtual void onExit(GameTeam* owner) override {};
    virtual bool onMessage(GameTeam* owner, Telegram &msg) override;

    CREATE_FUNC(GameTeamGlobalState);
};

/**
	 队伍正处于庆祝状态
*/
class GameTeamCelebrateState : public State<GameTeam>
{
public:
    virtual void onEnter(GameTeam* owner) override;
    virtual void update(GameTeam* owner, float dm) override {};
    virtual void onExit(GameTeam* owner) override {};
    virtual bool onMessage(GameTeam* owner, Telegram &msg) override {return false;};

    CREATE_FUNC(GameTeamCelebrateState);
};

#endif