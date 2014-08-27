#include "PCInputManager.h"

PCInputManager::PCInputManager()
{
    _keyBoardListener   =   nullptr;
    _delegate           =   nullptr;
    _currentKeyCode     =   EventKeyboard::KeyCode::KEY_NONE;
}

PCInputManager::~PCInputManager()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(_keyBoardListener);
    CC_SAFE_RELEASE(_keyBoardListener);
}

bool PCInputManager::init()
{
    // 在此处监听键盘操作
    _keyBoardListener   =   EventListenerKeyboard::create();
    _keyBoardListener->onKeyPressed     =   CC_CALLBACK_2(PCInputManager::onKeyPressed, this);
    _keyBoardListener->onKeyReleased    =   CC_CALLBACK_2(PCInputManager::onKeyReleased, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_keyBoardListener, 1);

    // 设置每一帧的回调函数，比如玩家一直按着某个键的时候就要在每一帧中都要调用委托来通知
    Director::getInstance()->getScheduler()->schedule(CC_CALLBACK_1(PCInputManager::update, this), 
        this, 0, false, INPUTMANAGER_SCHEDULE_KEY);

    return true;
}

void PCInputManager::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* pEvent)
{
    _currentKeyCode =   keyCode;
}

void PCInputManager::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* pEvent)
{
    _currentKeyCode =   EventKeyboard::KeyCode::KEY_NONE;
}

void PCInputManager::update(float dm)
{
    if (_delegate == nullptr)
    {
        return;
    }

    switch (_currentKeyCode)
    {
    case EventKeyboard::KeyCode::KEY_A:
        _delegate->moveToLeft();
        break;

    case EventKeyboard::KeyCode::KEY_D:
        _delegate->moveToRight();
        break;

    case EventKeyboard::KeyCode::KEY_NONE:
        _delegate->idle();
        break;

    case EventKeyboard::KeyCode::KEY_SPACE:
        _delegate->changeTarget();
        break;

    default:
        break;
    }
}