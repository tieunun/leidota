#ifndef __JOY_STICK_H__
#define __JOY_STICK_H__

#include "cocos2d.h"

using namespace cocos2d;

/**
* 游戏手柄，其实就是两个Sprite，一个作为底座，一个作为上面的跟着人手指移动的，外部可以通过接口获取当前的角度
*/
class JoyStick : public Sprite
{
public:
    /**
    * 分别是作为底座和控制点 
    */
    JoyStick(Sprite* pedestal, Sprite* ctrlPoint);

    bool init() override;

    CC_SYNTHESIZE_READONLY(Vec2, m_ctrlDirection, CtrlDirection);                   // 当前控制方向

    static JoyStick* create(Sprite* pedestal, Sprite* ctrlPoint);

protected:
    /**
    * 以下是在控制点上面的操作
    */
    bool onTouchBegan(Touch* pTouch, Event* pEvent);
    void onTouchMoved(Touch* pTouch, Event* pEvent);
    void onTouchEnded(Touch* pTouch, Event* pEvent);

    /**
    * 控制点跟随点击处 
    */
    void followTouchPoint(Touch* pTouch);

    float   m_pedestalRadius;                       // 基座的半径，控制点在这个范围内
    Vec2    m_pedestalCenter;                       // 基座的中心
    
    Sprite* m_pedestal;
    Sprite* m_ctrlPoint;
};

#endif