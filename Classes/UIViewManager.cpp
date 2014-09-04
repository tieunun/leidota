#include "UIViewManager.h"
#include "UIView.h"

UIViewManager* UIViewManager::m_instance    =   nullptr;

UIViewManager::UIViewManager()
{

}

UIViewManager::~UIViewManager()
{

}

UIViewManager* UIViewManager::instance()
{
    if (m_instance == nullptr)
    {
        m_instance  =   new UIViewManager();
    }

    return m_instance;
}

void UIViewManager::registerUIView( UIView* view )
{
    m_uiViewMap.insert(UIViewMap::value_type(view->getViewId(), view));
}

void UIViewManager::removeUIView(UIView* view)
{
    m_uiViewMap.erase(view->getViewId());
}

void UIViewManager::refreshView( RefreshUIMsg& msg )
{
    // 考虑到视图不会很多，这里就直接遍历所有视图算了
    for (auto tmpViewIterator = m_uiViewMap.begin(); tmpViewIterator != m_uiViewMap.end();)
    {
        auto tmpView    =   tmpViewIterator->second;
        tmpViewIterator++;
        auto tmpWeeList =   tmpView->getWeeList();
        for (int i = 0; i < tmpWeeList.size(); i++)
        {
            if (tmpWeeList[i] == msg.eventType)
            {
                tmpView->onWee(msg);
                break;
            }
        }
    }
}