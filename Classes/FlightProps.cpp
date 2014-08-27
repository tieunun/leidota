#include "FlightProps.h"
#include "FlightPropsMgr.h"
#include "MessageDispatcher.h"

FlightProps::FlightProps(FlightPropsTypeEnum type, int senderId, Vec2 velocity)
{
    this->type      =   type;
    this->senderId  =   senderId;
    this->velocity  =   velocity;
    shouldBeDel     =   false;
    isEffective     =   true;
    senderAtt       =   ((GameCharacter*)EntityMgr->getEntityFromID(senderId))->getAttribute();
}

void FlightProps::update(float dm)
{
    // 默认的都是匀速运动的
    this->setPosition(this->getPosition() + velocity * dm);
}

Rect FlightProps::getCollisionRect()
{
    auto tmpRect    =   this->getBoundingBox();
    return tmpRect;
}

OneToOneArmatureFlightProps::OneToOneArmatureFlightProps(int senderId, int targetId, 
    Armature* armature, std::string actionName, float rate)
    :FlightProps(FLIGHTPROPS_TYPE_FIXED_POINT_ONE_TARGET, senderId, Vec2(0, 0))
{
    this->m_targetId            =   targetId;
    armature->retain();
    this->m_armature            =   armature;
    this->m_actionName          =   actionName;
    this->setContentSize(armature->getContentSize());

    // 计算速度
    auto tmpSenderPos           =   EntityMgr->getEntityFromID(senderId)->getShape()->getPosition();
    auto tmpTargetPos           =   ((GameCharacterShape*)EntityMgr->getEntityFromID(targetId)->getShape())->getCenterPos();
    this->velocity              =   tmpTargetPos - tmpSenderPos;
    this->velocity.normalize();
    this->velocity              *=  rate;
}

OneToOneArmatureFlightProps::~OneToOneArmatureFlightProps()
{
    CC_SAFE_RELEASE(m_armature);
}

bool OneToOneArmatureFlightProps::init()
{
    if (!FlightProps::init())
    {
        return false;
    }

    this->addChild(m_armature);

    // 改为在移动的时候使用第一帧的
    // this->m_armature->getAnimation()->play(m_actionName, -1, 0);
    return true;
}

OneToOneArmatureFlightProps* OneToOneArmatureFlightProps::create(int senderId, int targetId, int skillId)
{
    Armature* tmpArmature   =   nullptr;
    string  tmpActionName   =   "";
    float   tmpRate         =   500;

    switch (skillId)
    {
    case 1:                                     // 雪精灵的雪球攻击
        ArmatureDataManager::getInstance()->addArmatureFileInfo("xuejingling-texiao.ExportJson");
        tmpActionName   =   "atk";
        tmpArmature     =   Armature::create("xuejingling-texiao");
        tmpRate         =   2000;

    default:
        break;
    }

    OneToOneArmatureFlightProps* pRet   =   
        new OneToOneArmatureFlightProps(senderId, targetId, tmpArmature, tmpActionName, tmpRate);
    if (pRet != nullptr)
    {
        pRet->init();
        pRet->autorelease();
    }

    // 加入到管理器中
    FliProMgr->registerFlightProps(pRet);
    return pRet;
}

void OneToOneArmatureFlightProps::onCollision(int id)
{
    // 如果不是之前设定的目标，就抛弃
    if (id != m_targetId)
    {
        return;
    }

    // 打到了@_@暂时就作为打击普通攻击吧
    // this->shouldBeDel   =   true;
    this->isEffective   =   false;

    // 同时继续播放动画
    m_armature->getAnimation()->play(m_actionName, -1, 0);

    // 同时发出消息
    auto tmpMsg = TelegramNormalAttack::create(this->senderId, id, this->senderAtt);
    Dispatch->dispatchMessage(*tmpMsg);
}

void OneToOneArmatureFlightProps::update(float dm)
{
    if (this->isEffective)
    {
        FlightProps::update(dm);
    }
    else if (!m_armature->getAnimation()->isPlaying())
    {
        this->shouldBeDel   =   true;
    }
}