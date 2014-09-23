#include "NormalLongRangeWeapon.h"
#include "TimeTool.h"
#include "EntityManager.h"

NormalLongRangeWeapon::NormalLongRangeWeapon( GameCharacter* owner,
                                             ProjectileTypeEnum type, 
                                             float attDistance, 
                                             string attAnimationName /*= ""*/,
                                             float minAttackInterval /*= 3*/ )
                                             :Weapon(owner, NORMAL_LONG_RANGE_WEAPON),
                                             m_actionName(attAnimationName)
{
    m_attRadius         =   attDistance;
    m_minAttInterval    =   minAttackInterval;
    m_projectileType    =   type;
}

void NormalLongRangeWeapon::attack( GameCharacter* target )
{
    // 先播放动画
    m_pOwner->getShape()->playAction(m_actionName, false, 
        std::bind(&NormalLongRangeWeapon::onLaunchProjectile, this, std::placeholders::_1));
    m_lastAttackTime        =   TimeTool::getSecondTime();
    m_nextAttackReadyTime   =   m_lastAttackTime + m_minAttInterval;
    m_targetId              =   target->getId();
}

bool NormalLongRangeWeapon::isInAttackRange( GameCharacter* target )
{
    // 只需要在攻击范围内就OK
    auto tmpOwnerPos    =   m_pOwner->getMovingEntity().getPosition();
    auto tmpTargetPos   =   target->getMovingEntity().getPosition();
    return (tmpOwnerPos - tmpTargetPos).getLengthSq() <= m_attRadius * m_attRadius;
}

bool NormalLongRangeWeapon::isReadyForNextAttack()
{
    return m_nextAttackReadyTime < TimeTool::getSecondTime();
}

bool NormalLongRangeWeapon::isAttacking()
{
    return m_pOwner->getShape()->getCurrentAnimationName() == m_actionName;
}

void NormalLongRangeWeapon::onLaunchProjectile( string evt )
{
    if (isTargetAlive())
    {
        // 创建并发射子弹
        auto tmpProjectile  =   Projectile::create(m_pOwner->getAttribute(), 
            m_projectileType, (void*)m_targetId);
        
        // 放置初始位置
        tmpProjectile->setPosition(m_pOwner->getShape()->getCenterPos());

        // 设置初始速度，这里应该只是设置方向
        auto tmpCharacter   =   dynamic_cast<GameCharacter*>(EntityMgr->getEntityFromID(m_targetId));
        auto tmpTargetPos   =   tmpCharacter->getShape()->getCenterPos();
        auto tmpOwnerPos    =   m_pOwner->getShape()->getCenterPos();
        tmpProjectile->setOrientation((tmpTargetPos - tmpOwnerPos).getNormalized());
    }
}

bool NormalLongRangeWeapon::isTargetAlive()
{
    auto tmpCharacter   =   dynamic_cast<GameCharacter*>(EntityMgr->getEntityFromID(m_targetId));
    return tmpCharacter != nullptr && tmpCharacter->isAlive();
}
