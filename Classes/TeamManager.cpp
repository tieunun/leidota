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
    removeCanRemoveTeam();

    auto tmpIterator    =   m_allTeam.begin();
    for (; tmpIterator != m_allTeam.end(); )
    {
        auto tmpTeam    =   tmpIterator->second;
        tmpIterator++;
        tmpTeam->update(dm);
    }

    removeCanRemoveTeam();
}

void TeamManager::registerTeam(GameTeam* pTeam)
{
    pTeam->retain();
    m_allTeam.insert(TeamMap::value_type(pTeam->getTeamId(), pTeam));
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

void TeamManager::removeCanRemoveTeam()
{
    for (auto tmpIterator = m_allTeam.begin(); tmpIterator != m_allTeam.end();)
    {
        if (tmpIterator->second->canRemove())
        {
            CC_SAFE_RELEASE_NULL(tmpIterator->second);
            tmpIterator =   m_allTeam.erase(tmpIterator);
        }
        else
        {
            tmpIterator++;
        }
    }
}
