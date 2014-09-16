#include "WeaponControlSystem.h"
#include "NormalCloseRangeWeapon.h"

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
        m_currentWeapon->attack(target);
    }
}
