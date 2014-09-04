#ifndef __UI_VIEW_MANAGER_H__
#define __UI_VIEW_MANAGER_H__

#include <map>
#include "UIViewIdEnum.h"
#include "RefreshUIMsg.h"

using namespace std;

class UIView;

/**
* 窗口管理器，全局单件
*/
class UIViewManager
{
public:
    typedef map<UIViewIdEnum, UIView*> UIViewMap;

    static UIViewManager* instance();

    /**
    * 注册 
    */
    void registerUIView(UIView* view);

    /**
    *  移除
    */
    void removeUIView(UIView* view);

    /**
    *  发送一个消息，刷新相关的视图
    */
    void refreshView(RefreshUIMsg& msg);

private:
    UIViewManager();
    ~UIViewManager();

    UIViewMap  m_uiViewMap;                    // 保存当前存在的所有窗口

    static UIViewManager*       m_instance;
};

#define UIViewMgr   UIViewManager::instance()

#endif