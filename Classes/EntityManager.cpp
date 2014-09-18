#include "EntityManager.h"

EntityManager* EntityManager::_instance =   nullptr;

EntityManager::EntityManager()
{
    _entityMap  =   new EntityMap();
}

EntityManager::~EntityManager()
{
    CC_SAFE_DELETE(_entityMap);
}

EntityManager::EntityManager(const EntityManager& entity)
{
    
}

EntityManager& EntityManager::operator=(const EntityManager&)
{
    return *this;
}

EntityManager* EntityManager::instance()
{
    if (_instance == nullptr)
    {
        _instance   =   new EntityManager();
    }

    return _instance;
}

void EntityManager::registerEntity(BaseGameEntity *entity)
{
    _entityMap->insert(EntityMap::value_type(entity->getId(), entity));
}

BaseGameEntity* EntityManager::getEntityFromID(int id)
{
    auto tmpIterator = _entityMap->find(id);
    if (tmpIterator == _entityMap->end())
    {
        return nullptr;
    }

    return tmpIterator->second;
}

void EntityManager::removeEntity(BaseGameEntity *entity)
{
    // 如果移除的是主控实体，就把这个也设置为nullptr
    _entityMap->erase(entity->getId());
}

const EntityManager::EntityMap* EntityManager::getEntityMap()
{
    return _entityMap;
}