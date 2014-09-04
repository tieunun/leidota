#include "UIView.h"
#include "UIViewManager.h"

UIView::UIView( UIViewIdEnum id )
{
    m_viewId    =   id;
}

void UIView::setWeeList()
{

}

vector<RefreshUIEventEnum>& UIView::getWeeList()
{
    return m_weeList;
}

UIViewIdEnum UIView::getViewId()
{
    return m_viewId;
}

void UIView::onEnter()
{
    setWeeList();
    Layer::onEnter();

    // 需要在管理中注册
    UIViewMgr->registerUIView(this);
}

void UIView::onExit()
{
    Layer::onExit();

    // 需要在管理中移除
    UIViewMgr->removeUIView(this);
}

void UIView::onWee( RefreshUIMsg& msg )
{

}