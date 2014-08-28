#ifndef __MOBILE_INPUT_MANAGER_H__
#define __MOBILE_INPUT_MANAGER_H__

#include "cocos2d.h"
#include "InputManager.h"

using namespace cocos2d;

/**
	 作为手机端的输入管理，手机端就是靠触屏来指挥的
*/
class MobileInputManager : public InputManager, public Layer
{
public:
    MobileInputManager();
    ~MobileInputManager();
    bool init();

private:
    /**
    	 控制方向
    */
    enum ControlDirection
    {
        INVALID_CONTROL_DIRECTION,
        RIGHT_CONTROL_DIRECTION,
        LEFT_CONTROL_DIRECTION
    };

    EventListenerTouchOneByOne* m_touchListener;            // 触屏监听
    Node*                       m_button;                   // 切换的按钮
    Point                       m_lastTouchPoint;           // 上一次触屏的屏幕坐标
    ControlDirection            m_controlDirection;         // 当前的控制方向

    // 每一帧的回调
    void update(float dm);

    // 监听触屏事件的
    bool onTouchBegin(Touch* pTouch, Event* pEvent);
    void onTouchEnd(Touch* pTouch, Event* pEvent);

    // 点击按钮
    bool onTouchButtonBegan(Touch* pTouch, Event* pEvent);
};

#endif