#include "NormalCloseRangeWeapon.h"
#include "Telegram.h"
#include "GameCharacter.h"
#include "TimeTool.h"
#include "MathTool.h"

NormalCloseRangeWeapon::NormalCloseRangeWeapon( GameCharacter* owner, float minAttackInterval, string actionName )
    :Weapon(owner, Weapon::WeaponTypeEnum::NORMAL_CLOSE_RANGE_WEAPON), m_actionName(actionName), m_xOffset(110), m_yOffset(8)
{
    m_minAttackInterval     =   minAttackInterval;
}

NormalCloseRangeWeapon::~NormalCloseRangeWeapon()
{

}

void NormalCloseRangeWeapon::attack( GameCharacter* target )
{
    // 对于普通的近程攻击只需要播放动画，然后在对应的动作帧发出消息就OK了
    m_pOwner->getShape()->playAction(m_actionName, false);
    m_lastAttackTime        =   TimeTool::getSecondTime();
    m_nextAttackReadyTime   =   m_lastAttackTime + m_minAttackInterval;
}

bool NormalCloseRangeWeapon::isInAttackRange( GameCharacter* target )
{
    auto tmpOwnerPos    =   m_pOwner->getMovingEntity().getPosition();
    auto tmpTargetPos   =   target->getMovingEntity().getPosition();
    auto tmpOwnerRadius =   m_pOwner->getMovingEntity().getRadius();
    auto tmpTargetRadius=   target->getMovingEntity().getRadius();
    
    // 对于普通攻击的攻击范围这个很难界定，因为可能每一个移动体的距离不同，所以这里做一个范围，与双方的半径有关，切是一个矩形
    auto tmpAttRectW    =   (tmpOwnerRadius + tmpTargetRadius + m_xOffset) * 2;
    auto tmpAttRectH    =   (tmpTargetRadius - m_yOffset) * 2;
    Rect tmpAttRect(tmpOwnerPos.x - tmpAttRectW / 2, tmpOwnerPos.y - tmpAttRectH / 2, tmpAttRectW, tmpAttRectH);

    return tmpAttRect.containsPoint(tmpTargetPos);
}

bool NormalCloseRangeWeapon::isReadyForNextAttack()
{
    return m_nextAttackReadyTime < TimeTool::getSecondTime();
}

bool NormalCloseRangeWeapon::isAttacking()
{
    return m_pOwner->getShape()->getCurrentAnimationName() == m_actionName;
}