#include "Weapon.h"

Weapon::Weapon( GameCharacter* owner, Weapon::WeaponTypeEnum type )
{
    m_pOwner        =   owner;
    m_weaponType    =   type;
}

Weapon::~Weapon()
{

}

Weapon::WeaponTypeEnum Weapon::getWeaponType()
{
    return m_weaponType;
}
