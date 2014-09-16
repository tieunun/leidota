#ifndef __WEAPON_CONTROL_SYSTEM_H__
#define __WEAPON_CONTROL_SYSTEM_H__

#include <map>
#include "Weapon.h"

using namespace std;

class GameCharacter;

/**
* 武器控制系统，这里负责维护角色身上的所有攻击方式，包括普通攻击、技能攻击等，包括
* 使用哪种方式来攻击的逻辑也在这里，关于攻击方式应该是要根据某些环境来得出使用某种
* 武器来攻击
*/
class WeaponControlSystem
{
public:
    WeaponControlSystem(GameCharacter* owner);
    ~WeaponControlSystem();

    /**
    * 给该武器系统添加武器类型 
    */
    void addWeapon(Weapon* aWeapon);

    /**
    * 使用当前武器并且攻击某个目标 
    */
    void takeWeaponAndAttack(GameCharacter* target);

private:
    GameCharacter*                              m_pOwner;           // 当前武器拥有者

    Weapon*                                     m_currentWeapon;    // 当前武器
    
    typedef map<Weapon::WeaponTypeEnum, Weapon*> WeaponMap;
    WeaponMap        m_allWeapons;                                  // 当前角色拥有的所有武器
};

#endif