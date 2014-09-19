#ifndef __NORMAL_CLOSE_RANGE_WEAPON_H__
#define __NORMAL_CLOSE_RANGE_WEAPON_H__

#include <string>
#include "Weapon.h"

using namespace std;

/**
* 普通的近距离攻击武器，这个基本是所有近战型角色必备的武器 
*/
class NormalCloseRangeWeapon : public Weapon
{
public:
    NormalCloseRangeWeapon(GameCharacter* owner, float minAttackInterval = 3, string actionName = "atk1");
    virtual ~NormalCloseRangeWeapon();

    virtual void attack(GameCharacter* target) override;
    virtual bool isInAttackRange(GameCharacter* target) override;
    virtual bool isReadyForNextAttack() override;
    virtual bool isAttacking() override;

private:
    /**
    *  对应的动画文件中的动作名称，主要是考虑到在做动画文件的时候命名不规范的原因
    *  普通的近距离攻击武器基本上就只需要播放动画就OK了
    */
    const string ACTION_NAME;

    /**
    *  普通攻击的限制基本上就只有最短攻击间隔
    */
    float           m_minAttackInterval;
    float           m_lastAttackTime;               // 最近一次攻击时间
    float           m_nextAttackReadyTime;          // 下一次攻击准备完毕时间
};

#endif