#include "GameTeam.h"
#include "MessageDispatcher.h"
#include "EntityManager.h"
#include "TeamManager.h"
#include "GameCharacter.h"

int GameTeam::m_nextValidId =   0;

GameTeam::GameTeam()
{
    m_teamId        =   m_nextValidId++;
}

GameTeam::~GameTeam()
{

}

void GameTeam::addMember(GameCharacter* player, int posId)
{
    player->retain();
    m_members.push_back(player);
    player->setTeam(this);
    player->getMovingEntity().setFormationPosId(posId);
}

void GameTeam::update(float dm)
{
    // 首先删除当前处于死亡状态的成员
    removeDeadCharacter();

    // 调用该队的所有成员的update
    for (auto tmpIterator = m_members.begin(); tmpIterator != m_members.end(); tmpIterator++)
    {
        (*tmpIterator)->update(dm);
    }

    // 再删除处于死亡状态的队员
    removeDeadCharacter();
}

GameTeam* GameTeam::create()
{
    auto pRet   =   new GameTeam();
    if (pRet != nullptr)
    {
        pRet->autorelease();
        TeamMgr->registerTeam(pRet);
    }

    return pRet;
}

void GameTeam::removeDeadCharacter()
{
    for (auto tmpIterator = m_members.begin(); tmpIterator != m_members.end();)
    {
        if ((*tmpIterator)->getState() == GameCharacter::dead)
        {
            CC_SAFE_RELEASE_NULL(*tmpIterator);
            tmpIterator = m_members.erase(tmpIterator);
        }
        else
        {
            tmpIterator++;
        }
    }
}
