#include "MessageDispatcher.h"
#include "EntityManager.h"

MessageDispatcher* MessageDispatcher::_instance = nullptr;

/**
	向某个人发出一个消息
*/
void MessageDispatcher::disCharge(BaseGameEntity* pReceiver, Telegram& msg)
{
    if (pReceiver == nullptr)
    {
        // 此时接受者已经不存在了
        return;
    }

    pReceiver->handleMessage(msg);
}

MessageDispatcher* MessageDispatcher::instance()
{
    if (_instance == nullptr)
    {
        _instance   =   new MessageDispatcher();
    }

    return _instance;
}

void MessageDispatcher::dispatchMessage(Telegram& msg)
{
    // TODO 以后可能会有延时的情况
    BaseGameEntity* tmpReceiver =   EntityMgr->getEntityFromID(msg.receiverId);
    disCharge(tmpReceiver, msg);
}