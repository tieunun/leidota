#include "EntityManager.h"

EntityManager* EntityManager::_instance =   nullptr;

EntityManager::EntityManager()
{
    _entityMap  =   new EntityMap();
    _lastReturnGetOneId = INVALID_GAME_ENTITY_ID;
    _lastUpdateListIndex = 0;
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
    
    // 首先找到当前链表上挂的最少的
    EntityMap* tmpMap = &_entityUpdateList[0];
    for (int i = 1; i < ENTITY_UPDATE_LIST_NUM; i++)
    {
        if (tmpMap->size() > _entityUpdateList[i].size())
        {
            tmpMap =   &_entityUpdateList[i];
        }
    }

    // 挂在这上面
    tmpMap->insert(EntityMap::value_type(entity->getId(), entity));
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
    if (entity == _mainEntity)
    {
        _mainEntity =   nullptr;
    }
    _entityMap->erase(entity->getId());

    // 同时移除update链表上的
    for (int i = 0; i < ENTITY_UPDATE_LIST_NUM; i++)
    {
        auto tmpMap = &_entityUpdateList[i];
        if (tmpMap->find(entity->getId()) != tmpMap->end())
        {
            tmpMap->erase(entity->getId());
            return;
        }
    }
}

void EntityManager::executeUpdate(float dm)
{
    _lastUpdateListIndex++;
    _lastUpdateListIndex = _lastUpdateListIndex >= ENTITY_UPDATE_LIST_NUM ? 0 : _lastUpdateListIndex;
    EntityMap::const_iterator  tmpIterator =   _entityUpdateList[_lastUpdateListIndex].begin();
    std::vector<BaseGameEntity*> tmpAllEntity;

    // 先把要更新的放在一个vector，然后再update，防止在update中把自己删掉了
    for (;tmpIterator != _entityUpdateList[_lastUpdateListIndex].end(); tmpIterator++)
    {
        tmpAllEntity.push_back(tmpIterator->second);
    }

    for (int i = 0; i < tmpAllEntity.size(); i++)
    {
        tmpAllEntity[i]->update(dm);
    }
}

const EntityManager::EntityMap* EntityManager::getEntityMap()
{
    return _entityMap;
}

BaseGameEntity* EntityManager::getOneEntity(GameEntityTypeEnum type)
{
    auto tmpIterator    =   _entityMap->begin();
    auto tmpCount       =   0;
    for (; tmpIterator != _entityMap->end(); tmpIterator++)
    {
        if (tmpIterator->second->getType() == type)
        {
            tmpCount++;
            if (tmpIterator->second->getId() > _lastReturnGetOneId)
            {
                _lastReturnGetOneId =   tmpIterator->second->getId();
                return tmpIterator->second;
            } 
        }
    }

    if (tmpIterator == _entityMap->end() && tmpCount > 0)
    {
        _lastReturnGetOneId =   INVALID_GAME_ENTITY_ID;
        return getOneEntity(type);
    }

    return nullptr;
}