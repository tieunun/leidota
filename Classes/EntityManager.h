#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

#include "BaseGameEntity.h"
#include "cocos2d.h"

/**
	全局的用来管理游戏中存在的所有实体，作为单件
*/
class EntityManager
{
public:
    typedef std::map<int, BaseGameEntity*> EntityMap;

private:
    EntityMap* _entityMap;     // 全局的实体id--->实体的映射
    static EntityManager *_instance;

    EntityManager();
    ~EntityManager();
    EntityManager(const EntityManager&);
    EntityManager &operator=(const EntityManager&);

public:
    static EntityManager* instance();
    
    /**
    	 注册一个实体，以id为键
    */
    void registerEntity(BaseGameEntity *entity);

    /**
    	 根据实体id返回对应的实体
    */
    BaseGameEntity* getEntityFromID(int id);

    /**
    	 移除某个实体
    */
    void removeEntity(BaseGameEntity *entity);

    /**
    	 为了方便外部遍历当前所有的实体
    */
    const EntityMap* getEntityMap();

    /**
    	 主控实体，也就是接受用户输入的
    */
    CC_SYNTHESIZE(BaseGameEntity*, _mainEntity, mainEntity);
};

#define EntityMgr   EntityManager::instance()

#endif