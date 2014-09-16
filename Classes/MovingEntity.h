#ifndef __MOVING_ENTITY_H__
#define __MOVING_ENTITY_H__

#include "cocos2d.h"

using namespace cocos2d;

/**
* 用来记录一个角色作为可移动体的所有信息，比如移动速度、加速度、质量等信息
*/
class MovingEntity
{
public:
    MovingEntity(float maxSpeed = 100, float maxForce = 10, float mass = 1, float radius = 60)
    {
        m_maxSpeed      =   maxSpeed;
        m_maxForce      =   maxForce;
        m_mass          =   mass;
        m_radius        =   radius;
    }

    ~MovingEntity(){}

    CC_SYNTHESIZE(Vec2, m_position, Position);              // 当前的坐标
    CC_SYNTHESIZE_READONLY(Vec2, m_velocity, Velocity);     // 速度
    CC_SYNTHESIZE(float, m_mass, Mass);                     // 质量
    CC_SYNTHESIZE(float, m_maxSpeed, MaxSpeed);             // 最大速度
    CC_SYNTHESIZE(float, m_maxForce, MaxForce);             // 最大驱动力
    CC_SYNTHESIZE(float, m_radius, Radius);                 // 角色所占半径

    void setVelocity(Vec2 aVelocity)
    {
        // 这里需要将速度控制在最大速度内
        if (aVelocity.lengthSquared() > m_maxSpeed * m_maxSpeed)
        {
            aVelocity.normalize();
            aVelocity  *=  m_maxSpeed;
        }

        m_velocity  =   aVelocity;
    }
};

#endif