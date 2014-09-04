#ifndef __UI_VIEW_H__
#define __UI_VIEW_H__

#include "cocos2d.h"
#include "RefreshUIMsg.h"
#include "UIViewIdEnum.h"

using namespace cocos2d;
using namespace std;

/**
* 主要还是考虑到以后如何在数据发生变化的时候通知视图这一层，所以这里要求所有的视图都必须继承于类，这里
* 采用所谓的事件的方式，当数据发生变化时，如果需要刷新界面，就调用发送消息，消息发送机制会自动把消息发给
* 当前正在使用的视图
*/
class UIView : public Layer
{
public:
    /**
    * 创建的时候需要传入唯一的标志
    */
    UIView(UIViewIdEnum id);

    /**
    * 当接收到事件的回调函数 
    */
    virtual void onWee(RefreshUIMsg& msg);

    /**
    * 返回该视图监听的事件 
    */
    vector<RefreshUIEventEnum>& getWeeList();

    /**
    * 返回该视图的唯一标识符 
    */
    UIViewIdEnum getViewId();

    void onEnter() override;

    void onExit() override;

protected:
    /**
    * 如果子类需要监听某个刷新事件的话，需要覆盖该函数 
    */
    virtual void setWeeList();

    vector<RefreshUIEventEnum>      m_weeList;                      // 该视图监听的事件
    UIViewIdEnum                    m_viewId;                       // 界面唯一标识符
};

#endif