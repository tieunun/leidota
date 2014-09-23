#ifndef __NORMAL_LONG_RANGE_WEAPON_H__
#define __NORMAL_LONG_RANGE_WEAPON_H__

#include "Weapon.h"
#include "GameCharacter.h"
#include "Projectile.h"

/**
*	普通远距离攻击武器，就是发射出一个飞行物去攻击目标 
*/
class NormalLongRangeWeapon : public Weapon
{
public:
    /**
    *	创建时需要指定发射的“子弹类型”，主要是TMD还会有角色的一个动画
    */
    NormalLongRangeWeapon(GameCharacter* owner, ProjectileTypeEnum type, float attDistance, 
        string attAnimationName = "atk1",float minAttackInterval = 3);

    virtual void attack(GameCharacter* target);
    virtual bool isInAttackRange(GameCharacter* target);
    virtual bool isReadyForNextAttack();
    virtual bool isAttacking();

private:
    /**
    *	当发射“子弹的时候” 
    */
    void onLaunchProjectile(string evt);

    /**
    *	目标是否存在 
    */
    bool isTargetAlive();

    float           m_attRadius;                    // 攻击半径
    float           m_minAttInterval;               // 最小攻击间隔
    const string    m_actionName;                   // 开火的时候的角色动画

    float   m_lastAttackTime;                         // 最近一次发送攻击的时间
    float   m_nextAttackReadyTime;                    // 

    int     m_targetId;

    ProjectileTypeEnum m_projectileType;            // “子弹”类型
};

#endif