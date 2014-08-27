#include "BaseGameEntity.h"
#include "EntityManager.h"

int BaseGameEntity::m_nextValidId = 0;

BaseGameEntity::BaseGameEntity()
{
    _id    =   m_nextValidId++;

    // ע�ᵽEntityManager
    EntityMgr->registerEntity(this);
}

BaseGameEntity::~BaseGameEntity()
{
    EntityMgr->removeEntity(this);
}

int BaseGameEntity::getId()
{
    return _id;
}