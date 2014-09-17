#ifndef __STEERING_BEHAVIORS_H__
#define __STEERING_BEHAVIORS_H__

#include "cocos2d.h"
#include "Formation.h"

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

    void setTarget(Vec2 t) { m_vTarget = t; }
    
    void setTargetId(int id) { m_targetId = id; }

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
    void pursuitOn() { m_behaviorsFlag |= PURSUIT; }
    void pursuitOff() { if(On(PURSUIT)) m_behaviorsFlag ^= PURSUIT; }
    void keepFormationOn() { m_behaviorsFlag |= KEEP_FORMATION; }
    void keepFormationOff() { if(On(KEEP_FORMATION)) m_behaviorsFlag ^= KEEP_FORMATION; }

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

    // 用来驱动角色追击指定角色的
    Vec2 pursuit(int targetId);

    // 用来保持阵型的驱动力
    Vec2 keepFormation(Formation& aFormation, int posId);

private:
    // 判断是否是邻居，如果是邻居，该角色就会受到邻居的影响
    bool isNeighbor(GameCharacter* other);

    /**
    *   在m_vSteeringForce上增加force，同时判定是否到达最大驱动力，如果
    *   到最大驱动力了，就返回false
    */
    bool accumulateForce(Vec2& steeringForce, Vec2 force);

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
        PURSUIT             =   1 << 4,                 // 追击指定角色
        KEEP_FORMATION      =   1 << 5,                 // 保持阵型
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

    /**
    * 与separation行为相关的数据
    */
    const float     m_separationMagnify;                // 被扩大的倍数

    /**
    * 与wallavoidance行为相关的数据 
    */
    const float     m_wallAvoidanceMagnify;             // 被扩大的倍数

    /**
    * 与pursue行为有关的数据 
    */
    int             m_targetId;                         // 正在追击的对手id
};

#endif