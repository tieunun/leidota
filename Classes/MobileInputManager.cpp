#include "MobileInputManager.h"

MobileInputManager::MobileInputManager():m_lastTouchPoint(200, 0)
{
    m_touchListener     =   nullptr;
    m_controlDirection  =   INVALID_CONTROL_DIRECTION;
}

MobileInputManager::~MobileInputManager()
{

}

bool MobileInputManager::init()
{
    m_touchListener =   EventListenerTouchOneByOne::create();
    m_touchListener->onTouchBegan   =   CC_CALLBACK_2(MobileInputManager::onTouchBegin, this);
    m_touchListener->onTouchEnded   =   CC_CALLBACK_2(MobileInputManager::onTouchEnd, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(m_touchListener, 1);

    // 设置每一帧的回调函数，比如玩家一直按着某个键的时候就要在每一帧中都要调用委托来通知
    Director::getInstance()->getScheduler()->schedule(CC_CALLBACK_1(MobileInputManager::update, this), 
        this, 0, false, INPUTMANAGER_SCHEDULE_KEY);

    return true;
}

void MobileInputManager::update(float dm)
{
    switch (m_controlDirection)
    {
    case MobileInputManager::INVALID_CONTROL_DIRECTION:
        {
            _delegate->idle();
            break;
        }
        
    case MobileInputManager::RIGHT_CONTROL_DIRECTION:
        {
            _delegate->moveToRight();
        	break;
        }

    case MobileInputManager::LEFT_CONTROL_DIRECTION:
        {
            _delegate->moveToLeft();
        	break;
        }

    default:
        {
        	break;
        }
    }
}

bool MobileInputManager::onTouchBegin(Touch* pTouch, Event* pEvent)
{
    auto tmpCurrentPos  =   pTouch->getLocationInView();
    if (tmpCurrentPos.x >m_lastTouchPoint.x)
    {
        m_controlDirection  =   RIGHT_CONTROL_DIRECTION;
    }
    else if (tmpCurrentPos.x < m_lastTouchPoint.x)
    {
        m_controlDirection  =   LEFT_CONTROL_DIRECTION;
    }
    else
    {
        m_controlDirection  =   INVALID_CONTROL_DIRECTION;
    }

    return true;
}

void MobileInputManager::onTouchEnd(Touch* pTouch, Event* pEvent)
{
    m_lastTouchPoint    =   pTouch->getLocationInView();
    m_controlDirection  =   INVALID_CONTROL_DIRECTION;
}