#include "TargetControlSystem.h"
#include "EntityManager.h"
#include "GameCharacter.h"

TargetControlSystem::TargetControlSystem( GameCharacter* owner )
{
    m_pOwner        =   owner;
    m_targetId      =   INVALID_GAME_ENTITY_ID;
}

TargetControlSystem::~TargetControlSystem()
{

}

void TargetControlSystem::update()
{
    // @_@ 这里的逻辑先这样写
    if (getTarget() != nullptr)
    {
        return;
    }
    else
    {
        // @_@ 这里也先这样写，因为目前就只有2个人
        if (m_pOwner->getId() == 0)
        {
            m_targetId  =   1;
        }
        else
        {
            m_targetId  =   0;
        }
    }
}

GameCharacter* TargetControlSystem::getTarget()
{
    return dynamic_cast<GameCharacter*>(EntityMgr->getEntityFromID(m_targetId));
}
