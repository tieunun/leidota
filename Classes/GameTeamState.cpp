#include "GameTeamState.h"
#include "TeamManager.h"

void GameTeamWaitState::onEnter(GameTeam* owner)
{
    // 通知所有的手下进入idle状态
    auto tmpMsg =   Telegram::create(0, 0, TELEGRAM_ENUM_TEAM_IDLE, 0);
    owner->sendMsgToAll(*tmpMsg);
}

void GameTeamWaitState::update(GameTeam* owner, float dm)
{
    m_frameCount++;
    if (m_frameCount >= m_waitFrame)
    {
        // 切换到战斗状态
        owner->getFSM()->changeState(GameTeamFreeCombatState::create());
    }
}

void GameTeamFreeCombatState::onEnter(GameTeam* owner)
{
    // 需要向手下的所有角色发送消息，表示命令角色们开始自由战斗
    auto tmpMsg =   Telegram::create(0, 0, TELEGRAM_ENUM_TEAM_FREE_COMBAT, 0);
    owner->sendMsgToAll(*tmpMsg);
}

void GameTeamFreeCombatState::update(GameTeam* owner, float dm)
{
    
}

void GameTeamFreeCombatState::onExit(GameTeam* owner)
{

}
bool GameTeamFreeCombatState::onMessage(GameTeam* owner, Telegram &msg)
{
    return false;
}

void GameTeamGlobalState::update(GameTeam* owner, float dm)
{
    
}

bool GameTeamGlobalState::onMessage(GameTeam* owner, Telegram &msg)
{
    
    return false;
}

void GameTeamCelebrateState::onEnter(GameTeam* owner)
{
    // 通知全队庆祝一下
    auto tmpMsg =   Telegram::create(0, 0, TELEGRAM_ENUM_TEAM_CELEBRATE, 0);
    owner->sendMsgToAll(*tmpMsg);
}