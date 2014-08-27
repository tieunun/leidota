#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include "cocos2d.h"

using namespace cocos2d;

#define INPUTMANAGER_SCHEDULE_KEY   "INPUTMANAGER_SCHEDULE_KEY"

/**
	 输入管理的代理，游戏逻辑本身是不需要知道具体的输入设备的，对于任何输入方式，
     只需要实现如下接口就可以从外部输入设备获取输入给游戏逻辑
*/
class InputManagerDelegate
{
public:
    /**
    	 用来操纵人物移动的操作
         @_@    现在只需要控制人物向右或者向左
    */
    virtual void moveToLeft()   =   0;      // 向左
    virtual void moveToRight()  =   0;      // 向右
    virtual void idle()         =   0;      // 什么都不操作
    virtual void changeTarget() =   0;      // 选择目标
};

/**
	 所有输入控制设备的基类
*/
class InputManager
{
public:
    virtual bool init() = 0;
    CC_SYNTHESIZE(InputManagerDelegate *, _delegate, Delegate);
};

#endif