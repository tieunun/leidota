#ifndef __PROJECTILE_MANAGER_H__
#define __PROJECTILE_MANAGER_H__

#include "cocos2d.h"
#include "Projectile.h"

using namespace std;

/**
*	正在飞行中的“子弹”管理器 
*/
class ProjectileManager
{
public:
    static ProjectileManager* instance();

    void addProjectile(Projectile* aProjectile);

    void update(float dm);

private:
    ProjectileManager() {}
    ~ProjectileManager() {}

    // 移除所有可以移除的子弹
    void removeAllCanRemoveProjectile();

    static ProjectileManager*   m_instance;

    typedef map<int, Projectile*> ProjectileMap;
    ProjectileMap               m_projectilesMap;          // 当前正在活跃的子弹
};

#define ProjectileMgr   ProjectileManager::instance()

#endif