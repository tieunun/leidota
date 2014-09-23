#ifndef __WEAPON_H__
#define __WEAPON_H__

class GameCharacter;

/**
* 武器类型@_@，也不知道怎么去分类，暂时就这样来划分吧 
*/
enum WeaponTypeEnum
{
    NORMAL_CLOSE_RANGE_WEAPON,          // 普通近战
    NORMAL_LONG_RANGE_WEAPON,           // 普通远程
};

/**
* 在该游戏中将各种攻击方式（包括普通近距离攻击、远距离攻击、技能攻击等）从角色身上分离出来，抽象为
* 武器，比如如果有一个角色有多种攻击方式，就相当于该角色携带有多种武器，而每种攻击的表现、效果灯逻
* 辑都封装到武器中，该类是所有武器的基类
*/
class Weapon
{
public:
    Weapon(GameCharacter* owner, WeaponTypeEnum type);
    virtual ~Weapon();

    /**
    * 返回武器类型 
    */
    WeaponTypeEnum getWeaponType();

    /**
    * 使用该武器攻击指定的目标
    */
    virtual void attack(GameCharacter* target) = 0;

    /**
    * 检查目标是否在该武器的攻击范围内
    */
    virtual bool isInAttackRange(GameCharacter* target) = 0;

    /**
    * 检查该武器是否可以准备好继续下一次攻击（可能会有各种各样的限制） 
    */
    virtual bool isReadyForNextAttack() = 0;

    /**
    * 检查当前是否正在攻击中
    * @_@ 主要是因为每种武器的攻击时长不同，而且在攻击时长内的时候是不允许移动
    */
    virtual bool isAttacking() = 0;

protected:
    GameCharacter*      m_pOwner;                          // 武器的拥有者
    WeaponTypeEnum      m_weaponType;                      // 武器类型
};

#endif