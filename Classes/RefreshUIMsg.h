#ifndef __REFRESH_UI_MSG_H__
#define __REFRESH_UI_MSG_H__

#include "RefreshUIEventEnum.h"

/**
* 用来刷新视图的消息结构 
*/
class RefreshUIMsg
{
public:
    RefreshUIMsg(RefreshUIEventEnum eventType, void* extraInfo)
    {
        this->eventType =   eventType;
        this->extraInfo =   extraInfo;
    }

    RefreshUIEventEnum  eventType;                      // 消息的事件类型
    void*               extraInfo;                      // 额外的事件，对应的UI应该要知道如何解析这个额外的数据
};

#endif