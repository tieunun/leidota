#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

#include "BaseGameEntity.h"
#include "cocos2d.h"

/**
	 用来更新实体的链表的数量，实体会被尽可能平均的分配在不同的链表上，然后在每一帧中
     只会调用一个链表上面的update，因为觉得这个没必要这么快
*/ 
#define ENTITY_UPDATE_LIST_NUM     1

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

    int _lastReturnGetOneId;                        // 最上一次调用getOneEntity返回的id

    EntityMap _entityUpdateList[ENTITY_UPDATE_LIST_NUM];  // 用来更新的
    int _lastUpdateListIndex;                       // 上一次调用update的链表索引

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
    	 执行所有实体的update
    */
    void executeUpdate(float dm);

    /**
    	 为了方便外部遍历当前所有的实体
    */
    const EntityMap* getEntityMap();

    /**
    	 主控实体，也就是接受用户输入的
    */
    CC_SYNTHESIZE(BaseGameEntity*, _mainEntity, mainEntity);

    /**
    	 @_@ 返回指定类型的一个对象，尽可能每次调用返回不同的
    */
    BaseGameEntity* getOneEntity(GameEntityTypeEnum type);
};

#define EntityMgr   EntityManager::instance()

#endif