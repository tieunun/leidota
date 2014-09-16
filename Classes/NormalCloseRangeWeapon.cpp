#include "NormalCloseRangeWeapon.h"
#include "Telegram.h"
#include "GameCharacter.h"
#include "TimeTool.h"
#include "MathTool.h"

NormalCloseRangeWeapon::NormalCloseRangeWeapon( GameCharacter* owner, float minAttackInterval, string actionName )
    :Weapon(owner, Weapon::WeaponTypeEnum::NORMAL_CLOSE_RANGE_WEAPON), ACTION_NAME(actionName)
{
    m_minAttackInterval     =   minAttackInterval;
}

NormalCloseRangeWeapon::~NormalCloseRangeWeapon()
{

}

void NormalCloseRangeWeapon::attack( GameCharacter* target )
{
    // 对于普通的近程攻击只需要播放动画，然后在对应的动作帧发出消息就OK了
    m_pOwner->getShape()->playAction(ACTION_NAME);
    m_lastAttackTime        =   TimeTool::getSecondTime();
    m_nextAttackReadyTime   =   m_lastAttackTime + m_minAttackInterval;
}

bool NormalCloseRangeWeapon::isInAttackRange( GameCharacter* target )
{
    // 比如这种是否在攻击范围内的判定，是与使用的武器类型有关
    auto tmpOwnerObject     =   m_pOwner->getObjectOnGrid();
    auto tmpTargetObject    =   target->getObjectOnGrid();

    auto tmpMapGrid         =   m_pOwner->getMapGrid();
    // 如果在一条水平线上，并且间距在3个格子内
    if (tmpMapGrid->testTwoIndexInOneHorizon(tmpOwnerObject->nodeIndex, tmpTargetObject->nodeIndex) 
        && abs(tmpTargetObject->nodeIndex - tmpOwnerObject->nodeIndex) <= 3)
    {
        return true;
    }

    // 如果在交叉位置处
    if (tmpOwnerObject->nodeIndex == tmpMapGrid->getLeftTopGridIndex(tmpTargetObject->nodeIndex)
        || tmpOwnerObject->nodeIndex == tmpMapGrid->getRightTopGridIndex(tmpTargetObject->nodeIndex)
        || tmpOwnerObject->nodeIndex == tmpMapGrid->getRightBottomGridIndex(tmpTargetObject->nodeIndex)
        || tmpOwnerObject->nodeIndex == tmpMapGrid->getLeftBottomGridIndex(tmpTargetObject->nodeIndex))
    {
        return true;
    }

    return false;
}

bool NormalCloseRangeWeapon::isReadyForNextAttack()
{
    return m_nextAttackReadyTime < TimeTool::getSecondTime();
}
