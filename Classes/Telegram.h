#ifndef __TELEGRAM_H__
#define __TELEGRAM_H__

#include "cocos2d.h"
#include "GameCharacterAttribute.h"

using namespace cocos2d;

/**
	 消息类型
*/
enum TelegramEnum
{
    /**
    	 玩家的指示，指挥人物向左还是向右以及放弃指挥
    */
    TELEGRAM_ENUM_USER_MOVE_RIGHT,
    TELEGRAM_ENUM_USER_MOVE_LEFT,
    TELEGRAM_ENUM_USER_IDLE,
    TELEGRAM_ENUM_USER_CHANGE_TARGET,

    /**
    	 游戏角色之间的消息
    */
    TELEGRAM_ENUM_NORMAL_ATTACK,                     // 普通攻击

    /**
    	 队伍与队伍中的角色之间的消息
    */
    TELEGRAM_ENUM_TEAM_IDLE,                        // 队伍通知角色等待
    TELEGRAM_ENUM_TEAM_FREE_COMBAT,                 // 队伍通知角色自由战斗
    TELEGRAM_ENUM_TEAM_CELEBRATE                    // 队伍通知角色庆祝一下胜利
};

/**
	 消息结构，在这个系统中用户与实体、实体与实体的交互都是通过消息，而不是直接的函数调用，该消息类
     是一个通用的消息，当然可以自定义消息结构，但是必须从这里继承
*/
class Telegram : public Ref
{
protected:
    Telegram() 
    {
        senderId        =   0;
        receiverId      =   0;
        dispatchTime    =   0;
    };
    virtual ~Telegram() {};

public:
    int             senderId;               // 发送者id
    int             receiverId;             // 接受者id
    TelegramEnum    type;                   // 消息类型
    float           dispatchTime;           // 发送时间

    static Telegram *create(int senderId, int receiverId, TelegramEnum type, float dispatchTime)
    {
        Telegram *pRet  =   new Telegram();
        if (pRet != nullptr)
        {
            pRet->senderId      =   senderId;
            pRet->receiverId    =   receiverId;
            pRet->type          =   type;
            pRet->dispatchTime  =   dispatchTime;

            pRet->autorelease();
        }

        return pRet;
    }
};

/**
	 普通攻击
*/
class TelegramNormalAttack : public Telegram
{
public:
    TelegramNormalAttack()
    {
        type    =   TELEGRAM_ENUM_NORMAL_ATTACK;
    }

public:
    GameCharacterAttribute  senderAtt;          // 发送者的属性，用来计算目标伤害的

    static TelegramNormalAttack* create(int senderId, int receiverId, GameCharacterAttribute& senderAtt)
    {
        TelegramNormalAttack* pRet = new TelegramNormalAttack();
        if (pRet)
        {
            pRet->senderId      =   senderId;
            pRet->receiverId    =   receiverId;
            pRet->senderAtt     =   senderAtt;
            pRet->autorelease();
        }

        return pRet;
    }
};

#endif