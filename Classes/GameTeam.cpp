#include "GameTeam.h"
#include "MessageDispatcher.h"
#include "GameTeamState.h"
#include "EntityManager.h"
#include "TeamManager.h"
#include "GameCharacter.h"

int GameTeam::m_nextValidId =   0;

GameTeam::GameTeam()
{
    m_teamId        =   m_nextValidId++;
    m_leaderId      =   INVALID_GAME_ENTITY_ID;
    m_stateMachine  =   TeamStateMachine::create(this);
    m_stateMachine->retain();
    m_stateMachine->changeState(GameTeamWaitState::create());
    m_stateMachine->setGlobalState(GameTeamGlobalState::create());
    m_mercenaryIdList.clear();
}

GameTeam::~GameTeam()
{
    CC_SAFE_RELEASE(m_stateMachine);
}

void GameTeam::setLeaderId(GameCharacter* player)
{
    m_leaderId  =   player->getId();
    player->setTeam(this);
}

void GameTeam::addMercenaryIds(GameCharacter* player)
{
    m_mercenaryIdList.push_back(player->getId());
    m_type  =   player->getType();
    player->setTeam(this);
}

void GameTeam::update(float dm)
{
    m_stateMachine->update(dm);

    // 判断如果该队伍的所有人都已经消息，就将自己删除
    if (EntityMgr->getEntityFromID(m_leaderId) != nullptr)
    {
        return;
    }

    for (auto tmpIterator = m_mercenaryIdList.begin(); tmpIterator != m_mercenaryIdList.end(); tmpIterator++)
    {
        if (EntityMgr->getEntityFromID(*tmpIterator) != nullptr)
        {
            return;
        }
    }

    TeamMgr->removeTeam(this);
}

void GameTeam::sendMsgToAll(Telegram& msg)
{
    msg.receiverId  =   m_leaderId;
    Dispatch->dispatchMessage(msg);

    auto tmpIterator    =   m_mercenaryIdList.begin();
    for (; tmpIterator != m_mercenaryIdList.end(); )
    {
        msg.receiverId  =   *tmpIterator;
        tmpIterator++;
        Dispatch->dispatchMessage(msg);
    }
}

bool GameTeam::init()
{
    return true;
}

GameTeam::TeamStateMachine* GameTeam::getFSM()
{
    return m_stateMachine;
}

void GameTeam::sendToMercenaries( Telegram& msg )
{
    auto tmpIterator    =   m_mercenaryIdList.begin();
    for (; tmpIterator != m_mercenaryIdList.end(); )
    {
        msg.receiverId  =   *tmpIterator;
        tmpIterator++;
        Dispatch->dispatchMessage(msg);
    }
}

void GameTeam::playerMoving( GameCharacter* player )
{
    // 目前只有移动的人物是老大的时候，才需要通知
    if (player->getId() != m_leaderId)
    {
        return;
    }

    // 构造消息通知佣兵跟随指定角色
    auto tmpMsg =   Telegram::create(0, 0, TELEGRAM_ENUM_TEAM_FOLLOW_SPECIFIED_PLAYER, 0, player);
    sendToMercenaries(*tmpMsg);
}
