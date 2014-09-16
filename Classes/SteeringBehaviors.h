#ifndef __STEERING_BEHAVIORS_H__
#define __STEERING_BEHAVIORS_H__

#include "cocos2d.h"

using namespace cocos2d;

class GameCharacter;

/**
* 用来计算行为驱动力的逻辑部分，施加在一个角色身上的合力在此处计算出来的
*/
class SteeringBehaviors
{
public:
    SteeringBehaviors(GameCharacter* owner);
    ~SteeringBehaviors();

    /**
    * 外部获取当前的驱动力的接口 
    */
    Vec2 calculate();

    void setTarget(Vec2 t) {m_vTarget = t;}
    
    /**
    * 关于驱动力的开启和关闭 
    */
    void seekOn() { m_behaviorsFlag |= SEEK; }
    void seekOff() { if (On(SEEK)) m_behaviorsFlag ^= SEEK;}
    void arriveOn() { m_behaviorsFlag |= ARRIVE; }
    void arriveOff() { if (On(ARRIVE)) m_behaviorsFlag ^= ARRIVE; }
    void separationOn() { m_behaviorsFlag |= SEPARATION; }
    void separationOff() { if (On(SEPARATION)) m_behaviorsFlag ^= SEPARATION; }
    void wallAvoidanceOn() { m_behaviorsFlag |= WALL_AVOIDANCE; }
    void wallAvoidanceOff() { if (On(WALL_AVOIDANCE)) m_behaviorsFlag ^= WALL_AVOIDANCE; }

private:
    /**
    *  各种驱动行为
    */
    Vec2 seek(const Vec2& target);
    
    Vec2 arrive(const Vec2& target);

    // 用来分离各个移动体
    Vec2 separation();

    // 用来让移动体不撞到四周的墙壁
    Vec2 wallAvoidance();

private:
    /**
    * 当前使用的行为驱动的标记 
    */
    enum BehaviorTypeEnum
    {
        NONE                =   0,                      // 初始化的
        SEEK                =   1,                      // 冲到指定的位置
        ARRIVE              =   1 << 1,                 // 到达指定位置
        SEPARATION          =   1 << 2,                 // 用来分离挨在一起的角色
        WALL_AVOIDANCE      =   1 << 3,                 // 用来避开墙壁的 
    };

    // 检查某个类型的驱动力是否开启
    bool On(BehaviorTypeEnum bt){return (m_behaviorsFlag & bt) == bt;}

    GameCharacter*  m_pOwner;

    int             m_behaviorsFlag;                    // 用来记录当前开启了哪些驱动
    Vec2            m_vSteeringForce;                   // 当前计算出来的驱动力

    /**
    * 与seek和arrive行为有关的数据 
    */
    Vec2            m_vTarget;                          // 目标坐标
    const float     m_arrivePrecision;                  // 接近的时候判断是到达的精度
};

#endif