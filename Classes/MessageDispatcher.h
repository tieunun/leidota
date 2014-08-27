#ifndef __MESSAGE_DISPATCHER_H__
#define __MESSAGE_DISPATCHER_H__

#include "Telegram.h"
#include "BaseGameEntity.h"

/**
	主管消息发送的，全局单件
*/
class MessageDispatcher
{
protected:
    MessageDispatcher(){}
    ~MessageDispatcher(){}

    void disCharge(BaseGameEntity* pReceiver, Telegram& msg);

    static MessageDispatcher* _instance;

public:
    static MessageDispatcher* instance();

    /**
    	发送消息
    */
    void dispatchMessage(Telegram& msg);

    /**
    	 为了方便玩家给所有敌人发送消息的接口
    */
    void dispatchMessageToAllEnemy(Telegram& msg);
};

#define Dispatch MessageDispatcher::instance()

#endif