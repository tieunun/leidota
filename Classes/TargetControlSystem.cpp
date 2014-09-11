#include "TargetControlSystem.h"
#include "GameCharacter.h"
#include "EntityManager.h"
#include "UIViewManager.h"

TargetControlSystem::TargetControlSystem( GameCharacter* owner, float updatePeriod):ControlSystem(owner, updatePeriod)
{
    m_targetId          =   INVALID_GAME_ENTITY_ID;
    m_choiceTargetState =   CHOICE_TARGET_AUTO_PRECEDENCE;

    // 一开始就强制玩家优先攻击某个目标
    forceTargetId(m_targetId);
}

TargetControlSystem::~TargetControlSystem()
{

}

GameCharacter* TargetControlSystem::getTargetCharacter()
{
    return dynamic_cast<GameCharacter*>(EntityMgr->getEntityFromID(m_targetId));
}

void TargetControlSystem::updateTarget()
{
    switch (m_choiceTargetState)
    {
    case TargetControlSystem::CHOICE_TARGET_AUTO_PRECEDENCE:
        {
            // 自动选择目标
            // @_@这一部分逻辑暂时放在GameCharacterAutoState中
            m_targetId  =   INVALID_GAME_ENTITY_ID;
        	break;
        }

    case TargetControlSystem::CHOICE_TARGET_USER_PRECEDENCE:
        {
            // 当玩家选择的目标不存在了，就切换到自动选择
            if (getTargetCharacter() == nullptr)
            {
                m_choiceTargetState =   CHOICE_TARGET_AUTO_PRECEDENCE;
                updateTarget();
            }
        	break;
        }

    default:
        break;
    }
}

void TargetControlSystem::forceTargetId( int id )
{
    m_choiceTargetState =   CHOICE_TARGET_USER_PRECEDENCE;
    setTargetId(id);
}

void TargetControlSystem::setTargetId( int id )
{
    m_targetId      =   id;

    // @_@ 这样写吧，如果当前角色是主角，就要界面切换一下攻击目标
    RefreshUIMsg    tmpMsg(REFRESH_UI_EVENT_ATTACK_CHARACTER);
    UIViewMgr->refreshView(tmpMsg);

    // 同时如果是主角的话，还要在主角攻击目标脚下有光环
    GameCharacter* tmpTarget    =   dynamic_cast<GameCharacter*>(EntityMgr->getEntityFromID(m_targetId));
    if (tmpTarget != nullptr && tmpTarget == EntityMgr->getmainEntity())
    {
        // @_@ 去掉除了该目标身下的光环
        auto tmpIterator    =   EntityMgr->getEntityMap()->begin();
        for (; tmpIterator != EntityMgr->getEntityMap()->end(); tmpIterator++)
        {
            GameCharacter* tmpCharacter =   dynamic_cast<GameCharacter*>(tmpIterator->second);
            if (tmpTarget != tmpCharacter)
            {
                tmpTarget->getShape()->hideHalo();
            }
            else
            {
                tmpTarget->getShape()->showHalo(GameCharacterShape::HALO_RED);
            }
        }
    }
}

void TargetControlSystem::update( float dm )
{
    updateTarget();
}
