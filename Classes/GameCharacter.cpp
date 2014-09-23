#include "GameCharacter.h"
#include "MessageDispatcher.h"
#include "UIViewManager.h"
#include "GoalCharacterThink.h"
#include "NormalCloseRangeWeapon.h"
#include "NormalLongRangeWeapon.h"
#include "TimeTool.h"

GameCharacter* GameCharacter::create(int id)
{
    /**
    	 临时就创建一种角色，这里是将组成一个游戏角色的部分拼接在一起
    */
    auto tmpRet = new GameCharacter();
    tmpRet->autorelease();

    /**
        Fuck主要是以后会有多种人物，这里吧，暂时就这样搞
        在此处拼装状态机、外形等
    */
    tmpRet->m_characterId   =   id;
    switch (id)
    {
    case 1:                                                 // 对应的是宙斯
        {
            // 不同的角色有不同的外形
            tmpRet->m_shape         =   GameCharacterShape::create("zhousi.ExportJson", "zhousi");
            tmpRet->m_shape->retain();

            // 不同的角色有不同的初始属性
            tmpRet->m_attribute     =   GameCharacterAttribute(80, 10, 30, 70);

            // 给它一些武器
            tmpRet->getWeaponControlSystem()->addWeapon(new NormalCloseRangeWeapon(tmpRet));

            break;
        }

    case 2:                                                 // 精灵
        {
            tmpRet->m_shape         =   GameCharacterShape::create("xuejingling-qian.ExportJson", "xuejingling-qian");
            tmpRet->m_shape->retain();

            tmpRet->m_attribute     =   GameCharacterAttribute(100, 40, 10, 90, 700);

            tmpRet->getWeaponControlSystem()->addWeapon(
                new NormalLongRangeWeapon(tmpRet, PROJECTILE_TYPE_GALAXO_BALL, 
                tmpRet->getAttribute().getAttDistance()));

            break;
        }

    case 3:                                                 // 骑士
        {
            tmpRet->m_shape         =   GameCharacterShape::create("Aer.ExportJson", "Aer");
            tmpRet->m_shape->retain();

            tmpRet->m_attribute     =   GameCharacterAttribute(150, 20, 20, 80);

            break;
        }

    case 4:                                                 // 野猪怪
        {
            tmpRet->m_shape         =   GameCharacterShape::create("Pig.ExportJson", "Pig");
            tmpRet->m_shape->retain();

            tmpRet->m_attribute     =   GameCharacterAttribute(400, 1, 10, 60 + CCRANDOM_0_1() * 20);

            break;
        }

    case 5:                                                 // 牛人
        {
            tmpRet->m_shape         =   GameCharacterShape::create("Niu.ExportJson", "Niu");
            tmpRet->m_shape->retain();

            tmpRet->m_attribute     =   GameCharacterAttribute(400, 1, 10, 50 + CCRANDOM_0_1() * 20);

            break;
        }

    default:
        break;
    }
    
    
    return tmpRet;
}

GameCharacter::GameCharacter()
{
    m_shape                         =   nullptr;
    m_team                          =   nullptr;
    m_state                         =   alive;

    // 武器控制系统
    m_weaponControlSystem           =   new WeaponControlSystem(this);

    // 角色的大脑
    m_brain                         =   new GoalCharacterThink(this);

    // 驱动力产生对象
    m_steeringBehaviors             =   new SteeringBehaviors(this);

    m_lastUpdateTime                =   -1;

    // 默认就打开几个驱动力
    m_steeringBehaviors->wallAvoidanceOn();
    m_steeringBehaviors->separationOn();
}

GameCharacter::~GameCharacter()
{
    // 将该角色的显示从显示列表中移除
    m_shape->removeFromParent();
    CC_SAFE_RELEASE_NULL(m_shape);

    CC_SAFE_DELETE(m_weaponControlSystem);
    m_weaponControlSystem   =   nullptr;

    CC_SAFE_DELETE(m_brain);
    m_brain                 =   nullptr;

    CC_SAFE_DELETE(m_steeringBehaviors);
    m_steeringBehaviors     =   nullptr;
}

void GameCharacter::update(float dm)
{
    // 如果当前角色是死亡的，那么就不用update了
    if (m_state == dead)
    {
        return;
    }

    if (m_lastUpdateTime == -1)
    {
        m_lastUpdateTime    =   TimeTool::getSecondTime();
    }
    auto tmpDmTime     =    TimeTool::getSecondTime() - m_lastUpdateTime;
    //CCLOG("%f", tmpDmTime);
    // 思考一下
    m_brain->process();

    // 根据MovingEntity来调整Shape的坐标
    updateMovement(tmpDmTime);

    // 这里以hp归零作为死亡的标准，然后在一个统一的地方删除处于死亡状态的
    if (getAttribute().getHp() <= 0)
    {
        m_state =   dead;
    }

    m_lastUpdateTime    =   TimeTool::getSecondTime();

    // @_@ 额外的数字标签
    m_shape->setPosNumber(m_movingEntity.getFormationPosId());
}

bool GameCharacter::handleMessage(Telegram& msg)
{
    // 首先把消息交给该成员的脑袋
    if (m_brain->handleMessage(msg)) return true;

    /**
    *	这里主要处理一些非AI的消息 
    */
    switch (msg.type)
    {
    case TELEGRAM_ENUM_NORMAL_ATTACK:                       // 收到普通攻击后
        {
            // 普通攻击只扣血，同时还要飘字
            TelegramNormalAttack* tmpMsg    =   dynamic_cast<TelegramNormalAttack*>(&msg);
            auto tmpFrontHp                 =   m_attribute.getHp();
            m_attribute.sufferNormalAttack(tmpMsg->senderAtt);
            auto tmpHp                      =   tmpFrontHp - m_attribute.getHp();
            if (this->getType() == GAME_ENTITY_TYPE_PLAYER_CHARACTER)
            {
                m_shape->floatNumber(tmpHp, GameCharacterShape::FLOAT_NUMBER_YELLOW);
            }
            else if (this->getType() == GAME_ENTITY_TYPE_ENEMY_CHARACTER)
            {
                m_shape->floatNumber(tmpHp, GameCharacterShape::FLOAT_NUMBER_RED);
            }
            if (m_attribute.getHp() <= 0)
            {
                m_state =   dead;
            }
            m_shape->setHpRatio(m_attribute.getHp() / m_attribute.getFullHp());
            return true;
        }

    default:
        break;
    }
    return false;
}

void GameCharacter::updateMovement(float dm)
{
    m_shape->setPosition(m_movingEntity.getPosition());
    // 如果当前武器系统使得该角色无法移动
   if (!m_weaponControlSystem->canCharacterMove())
   {
       return;
   }
    /**
    *  @_@ 这里其实要计算驱动力、加速度、速度等，并更新MovingEntity中的
    *  信息
    */
    // 总的合力
    Vec2 tmpForce   =   m_steeringBehaviors->calculate();
    if (tmpForce.getLengthSq() < 5)
    {
        // 如果力过小，就直接把速度降为0
        const double BrakingRate = 0.1; 
        m_movingEntity.setVelocity(m_movingEntity.getVelocity() * BrakingRate);                                     
    }
    else
    {
        // 加速度
        Vec2 tmpAccel   =   tmpForce / m_movingEntity.getMass();
        // 改变当前速度
        m_movingEntity.setVelocity(m_movingEntity.getVelocity() + tmpAccel * dm);
    }

    // 调用移动的动画
    if (m_movingEntity.getSpeed() > 5)
    {
        if (m_movingEntity.getVelocity().x > 0)
        {
            m_shape->faceToRight();
        }
        else
        {
            m_shape->faceToLeft();
        }
        m_shape->playAction(RUN_ACTION);
    }
    else
    {
        m_shape->playAction(IDLE_ACTION);
    }

    // 改变当前坐标
    m_movingEntity.setPosition(m_movingEntity.getPosition() + m_movingEntity.getVelocity() * dm);
    m_shape->setPosition(m_movingEntity.getPosition());
}

bool GameCharacter::hasGoal()
{
    return m_brain->hasSubgoal();
}
