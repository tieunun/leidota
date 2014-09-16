#include "SteeringBehaviors.h"
#include "GameCharacter.h"
#include "MathTool.h"

SteeringBehaviors::SteeringBehaviors( GameCharacter* owner ):m_arrivePrecision(1)
{
    m_pOwner        =   owner;
    m_behaviorsFlag =   NONE;
}

SteeringBehaviors::~SteeringBehaviors()
{

}

cocos2d::Vec2 SteeringBehaviors::calculate()
{
    m_vSteeringForce.setPoint(0, 0);

    /**
    * 要知道每一种驱动力的优先级其实是不同的，比如需要首要保证人物之间不能重叠和
    * 撞墙，然后才是arrive和seek
    */


    // @_@ 临时这样写
    if (On(SEEK))
    {
        m_vSteeringForce    +=  seek(m_vTarget);
    }
    if (On(ARRIVE))
    {
        m_vSteeringForce    +=  arrive(m_vTarget);
    }

    return m_vSteeringForce;
}

cocos2d::Vec2 SteeringBehaviors::seek( const Vec2 &target )
{
    auto tmpMovingEntity    =   m_pOwner->getMovingEntity();
    Vec2 tmpDesiredVelocity = 
        (target - tmpMovingEntity.getPosition()).getNormalized()
        * tmpMovingEntity.getMaxSpeed();
    return tmpDesiredVelocity - tmpMovingEntity.getVelocity();
}

cocos2d::Vec2 SteeringBehaviors::arrive( const Vec2& target )
{
    auto tmpMovingEntity    =   m_pOwner->getMovingEntity();
    Vec2 tmpToTarget        =   target - tmpMovingEntity.getPosition();

    float dist  =   tmpToTarget.length();
    dist        =   dist < m_arrivePrecision ? 0 : dist;
    if (dist > 0)
    {
        // 期望的速度，这里期望的肯定是以最快的方式接近过去
        float tmpSpeed  =   dist * 10;
        tmpSpeed = tmpSpeed > tmpMovingEntity.getMaxSpeed() ? tmpMovingEntity.getMaxSpeed() : tmpSpeed;
        Vec2 tmpDesiredVelocity =   tmpToTarget * tmpSpeed / dist;
        return tmpDesiredVelocity - tmpMovingEntity.getVelocity();
    }

    return Vec2(0,0);
}

cocos2d::Vec2 SteeringBehaviors::separation()
{
    // 首先找到该实体周围的在影响范围内的移动体

}

cocos2d::Vec2 SteeringBehaviors::wallAvoidance()
{
    // 

}
