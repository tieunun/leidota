#include "TeamManager.h"
#include "GameTeam.h"

TeamManager* TeamManager::m_instance = nullptr;

TeamManager* TeamManager::instance()
{
    if (m_instance == nullptr)
    {
        m_instance  =   new TeamManager();
    }

    return m_instance;
}

void TeamManager::update(float dm)
{
    auto tmpIterator    =   m_allTeam.begin();
    for (; tmpIterator != m_allTeam.end(); )
    {
        auto tmpTeam    =   tmpIterator->second;
        tmpIterator++;
        tmpTeam->update(dm);
    }
}

void TeamManager::registerTeam(GameTeam* pTeam)
{
    pTeam->retain();
    m_allTeam.insert(TeamMap::value_type(pTeam->getTeamId(), pTeam));
}

void TeamManager::removeTeam(GameTeam* pTeam)
{
    m_allTeam.erase(pTeam->getTeamId());
    CC_SAFE_RELEASE_NULL(pTeam);
}

const TeamManager::TeamMap& TeamManager::getTeamMap()
{
    return m_allTeam;
}

GameTeam* TeamManager::getTeamFromId(int id)
{
    auto tmpIterator    =   m_allTeam.find(id);
    if (tmpIterator == m_allTeam.end())
    {
        return nullptr;
    }

    return tmpIterator->second;
}