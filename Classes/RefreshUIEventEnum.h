#ifndef __REFRESH_UI_EVENT_ENUM_H__
#define __REFRESH_UI_EVENT_ENUM_H__

/**
* 各种要求刷新界面的事件的枚举值。每个视图会对应一到多个事件，在发生这些事件的时候会
* 去调用界面的刷新事件接收函数
*/
enum RefreshUIEventEnum
{
    REFRESH_UI_EVENT_CHARACTER,                      // 当角色数据发生变化的时候
    REFRESH_UI_EVENT_ATTACK_CHARACTER                // 当主角攻击目标或者数据发生变化的时候
};

#endif