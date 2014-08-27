#ifndef __PC_INPUT_MANAGER_H__
#define __PC_INPUT_MANAGER_H__

#include "InputManager.h"
#include "cocos2d.h"

using namespace cocos2d;

/**
	 PC端输入的管理，这里是为了使PC端可以通过键盘来操作
*/
class PCInputManager : public InputManager
{
public:
    PCInputManager();
    ~PCInputManager();
    bool init();

private:
    EventListenerKeyboard *_keyBoardListener;

    // 监听键盘的
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* pEvent);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* pEvent);

    // 每一帧的回调
    void update(float dm);

    EventKeyboard::KeyCode  _currentKeyCode;                // 当前被按下的键盘KeyCode
};

#endif