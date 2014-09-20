#include "WeaponControlSystem.h"
#include "NormalCloseRangeWeapon.h"
#include "GameCharacter.h"
#include "EntityManager.h"

WeaponControlSystem::WeaponControlSystem( GameCharacter* owner )
{
    m_pOwner            =   owner;
    m_currentWeapon     =   nullptr;
}

WeaponControlSystem::~WeaponControlSystem()
{
    for (auto tmpIterator = m_allWeapons.begin(); tmpIterator != m_allWeapons.end(); tmpIterator++)
    {
        delete tmpIterator->second;
    }

    m_allWeapons.clear();
}

void WeaponControlSystem::addWeapon(Weapon* aWeapon)
{
    // 添加到m_allWeapons中
    m_allWeapons.insert(WeaponMap::value_type(aWeapon->getWeaponType(), aWeapon));

    // @_@ 当前先这样写
    m_currentWeapon     =   aWeapon;
}

void WeaponControlSystem::takeWeaponAndAttack( GameCharacter* target )
{
    // 使用当前的武器攻击敌人
    if (m_currentWeapon->isInAttackRange(target) && m_currentWeapon->isReadyForNextAttack())
    {
        // 面向敌人
        if (m_pOwner->getMovingEntity().getPosition().x > target->getMovingEntity().getPosition().x)
        {
            m_pOwner->getShape()->faceToLeft();
        }
        else
        {
            m_pOwner->getShape()->faceToRight();
        }

        m_currentWeapon->attack(target);
        m_targetId  =   target->getId();
    }
}

bool WeaponControlSystem::isInAttackRange( GameCharacter* target )
{
    return m_currentWeapon->isInAttackRange(target);
}

bool WeaponControlSystem::canCharacterMove()
{
    // 只有当前武器不在攻击或者当前目标存在并且在攻击范围内
    auto tmpTarget  =   dynamic_cast<GameCharacter*>(EntityMgr->getEntityFromID(m_targetId));
    return !m_currentWeapon->isAttacking() && !(tmpTarget != nullptr && isInAttackRange(tmpTarget));
}
