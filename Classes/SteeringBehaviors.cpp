#include "SteeringBehaviors.h"
#include "GameCharacter.h"

SteeringBehaviors::SteeringBehaviors( GameCharacter* owner )
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
    // @_@ ÁÙÊ±ÕâÑùÐ´
    if (On(SEEK))
    {
        m_vSteeringForce    +=  seek(m_vTarget);
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
