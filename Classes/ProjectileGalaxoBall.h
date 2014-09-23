#ifndef __PROJECTILE_GALAXO_BALL_H__
#define __PROJECTILE_GALAXO_BALL_H__

#include "Projectile.h"

/**
*	闪电球，跟踪型
*/
class ProjectileGalaxoBall : public Projectile
{
public:
    ProjectileGalaxoBall(GameCharacterAttribute& att, int targetId);
    virtual ~ProjectileGalaxoBall();

    virtual void update(float dm) override;
    virtual void updateMovement(float dm) override;
    virtual Node* getShape() override;

private:
    Armature*       m_ball;         // 闪电球的动画，作为该闪电球的显示
    int             m_targetId;     // 攻击目标
    Vec2            m_targetPos;    // 目标坐标
};

#endif