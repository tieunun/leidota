#ifndef __GAME_CHARACTER_ATTRIBUTE_H__
#define __GAME_CHARACTER_ATTRIBUTE_H__

#include "cocos2d.h"

/**
	 封装了关于角色属性以及和属性相关的各种计算，主要是考虑到以后计算方式可能会经常变化，还有就是
     有些技能可能会同时修改多个属性
*/
class GameCharacterAttribute
{
public:
    /**
    	 当前暂时一个人物只有血量、攻击值、防御值以及移动速度
    */
    GameCharacterAttribute(float hp = 100, float attack = 10, float defense = 20, 
        float rate = 80, float attDistance = 650);

    /**
    	 遭受别人的普通攻击，这里之所以把对方的所有属性都传入，是因为考虑到可能普通攻击的杀伤计算
         可能还和其他乱七八糟的属性有关，返回该角色被攻击后的当前属性
    */
    GameCharacterAttribute& sufferNormalAttack(GameCharacterAttribute& otherAttr);

    CC_SYNTHESIZE(float, m_hp, Hp);
    CC_SYNTHESIZE(float, m_attack, Attack);
    CC_SYNTHESIZE(float, m_defense, Defense);
    CC_SYNTHESIZE(float, m_rate, Rate);
    CC_SYNTHESIZE(float, m_attDistance, AttDistance);
};

#endif