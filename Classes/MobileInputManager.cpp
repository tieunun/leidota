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
    if (!Layer::init())
    {
        return false;
    }

    m_touchListener =   EventListenerTouchOneByOne::create();
    m_touchListener->onTouchBegan   =   CC_CALLBACK_2(MobileInputManager::onTouchBegin, this);
    m_touchListener->onTouchEnded   =   CC_CALLBACK_2(MobileInputManager::onTouchEnd, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(m_touchListener, 2);

    // 创建一个按钮
    m_button      =   Sprite::create("changetarget.png");
    auto tmpButtonSize  =   m_button->getContentSize();
    m_button->setPosition(tmpButtonSize.width / 2 + 30, tmpButtonSize.height / 2 + 30);
    this->addChild(m_button);
    auto tmpButtonListener = EventListenerTouchOneByOne::create();
    tmpButtonListener->onTouchBegan =   CC_CALLBACK_2(MobileInputManager::onTouchButtonBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(tmpButtonListener, 1);

    // 设置每一帧的回调函数
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
            //_delegate->idle();
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
    auto tmpCurrentPos  =   convertTouchToNodeSpace(pTouch);
    auto tmpRect    =   m_button->getBoundingBox();
    if (tmpRect.containsPoint(tmpCurrentPos))
    {
        return false;
    }

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

bool MobileInputManager::onTouchButtonBegan(Touch* pTouch, Event* pEvent)
{
    auto tmpPoint   =   convertTouchToNodeSpace(pTouch);
    auto tmpRect    =   m_button->getBoundingBox();
    if (tmpRect.containsPoint(tmpPoint))
    {
        _delegate->changeTarget();
        return true;
    }
    return false;
}