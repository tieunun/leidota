#include "ProjectileGalaxoBall.h"
#include "MathTool.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"

ProjectileGalaxoBall::ProjectileGalaxoBall( GameCharacterAttribute& att, int targetId )
    :Projectile(att)
{
    ArmatureDataManager::getInstance()->addArmatureFileInfo("xuejingling-texiao.ExportJson");
    m_ball      =   Armature::create("xuejingling-texiao");
    m_ball->retain();
    m_rate      =   1800;
    m_targetId  =   targetId;
    GameCharacter* tmpTarget    =   dynamic_cast<GameCharacter*>(EntityMgr->getEntityFromID(m_targetId));
    m_targetPos =   tmpTarget->getShape()->getCenterPos();
}

ProjectileGalaxoBall::~ProjectileGalaxoBall()
{
    if (m_ball->getParent() != nullptr)
    {
        m_ball->removeFromParent();
    }
    CC_SAFE_RELEASE_NULL(m_ball);
}

void ProjectileGalaxoBall::update( float dm )
{
    // 判断是否到达
    if (canUpdateMovement())
    {
        float tmpDistance    =   (m_position - m_targetPos).getLengthSq();
        if (tmpDistance <= 400)
        {
            // 禁止更新位移
            updateMovementOff();
            // 说明此事闪电球已经到位，开始继续播放动画
            m_ball->getAnimation()->play("atk", -1, 0);

            // 发送消息给受击者，@_@发送者id没有必要
            auto tmpMsg = TelegramNormalAttack::create(0, m_targetId, m_senderAtt);
            Dispatch->dispatchMessage(*tmpMsg);
        }
    }
    else
    {
        // 当动画播放完毕后就
        if (!m_ball->getAnimation()->isPlaying())
        {
            updateOff();
        }
    }
}

void ProjectileGalaxoBall::updateMovement( float dm )
{
    // 获取当前位置
    GameCharacter* tmpTarget    =   dynamic_cast<GameCharacter*>(EntityMgr->getEntityFromID(m_targetId));
    if (tmpTarget != nullptr)
    {
        // 如果当前角色还没有消失，就要不停地修改速度来跟踪目标
        m_targetPos =   tmpTarget->getShape()->getCenterPos();
        Vec2 tmpToTarget    =   m_targetPos - m_position;
        tmpToTarget.normalize();
        m_velocity          =   tmpToTarget * m_rate;
    }

    // 修改位置
    m_position          +=  m_velocity * dm;
    m_ball->setPosition(m_position);
}

Node* ProjectileGalaxoBall::getShape()
{
    return m_ball;
}
