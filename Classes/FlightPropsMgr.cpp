#include "FlightPropsMgr.h"
#include "EntityManager.h"

FlightPropsMgr* FlightPropsMgr::m_instance  =   nullptr;

FlightPropsMgr* FlightPropsMgr::instance()
{
    if (m_instance == nullptr)
    {
        m_instance  =   new FlightPropsMgr();
    }

    return m_instance;
}

void FlightPropsMgr::registerFlightProps(FlightProps* flight)
{
    flight->retain();
    m_allFlights.insert(FlightPropsMap::value_type((int)flight, flight));
}

void FlightPropsMgr::removeFlightProps(FlightProps* flight)
{
    m_allFlights.erase((int)flight);
    flight->removeFromParent();
    flight->release();
}

void FlightPropsMgr::update(float dm)
{
    // 更新当前飞行道具的位置、状态等
    for (auto tmpFlightIterator = m_allFlights.begin(); tmpFlightIterator != m_allFlights.end();)
    {
        auto tmpFlight  =   tmpFlightIterator->second;
        // @_@ 现在就让迭代器向后
        tmpFlightIterator++;
        tmpFlight->update(dm);

        // 删除被标记为要删除的飞行道具
        if (tmpFlight->shouldBeDel)
        {
            removeFlightProps(tmpFlight);
            continue;
        }

        // 如果该飞行器已经无效，但是可能还在播放飞行器的动画，所以还是允许经过update
        if (!tmpFlight->isEffective)
        {
            continue;
        }

        // 遍历所有的场上角色，依次检测碰撞
        auto tmpMap =   EntityMgr->getEntityMap();
        for (auto tmpCharacterIterator = tmpMap->begin(); tmpCharacterIterator != tmpMap->end();)
        {
            auto tmpCharacter   =   tmpCharacterIterator->second;
            tmpCharacterIterator++;
            // 对自己无效
            if (tmpCharacter->getId() == tmpFlight->senderId)
            {
                continue;
            }
            if (collisionTest(tmpCharacter->getId(), tmpFlight))
            {
                tmpFlight->onCollision(tmpCharacter->getId());
                if (tmpFlight->shouldBeDel || !tmpFlight->isEffective)
                {
                    break;
                }
            }
        }

        // 如果飞行器飞行出了屏幕，也删除
        auto tmpWinSize     =   Director::getInstance()->getVisibleSize();
        auto tmpOrig        =   Director::getInstance()->getVisibleOrigin();
        // 这里获取在世界中的坐标
        auto tmpFlightPos   =   tmpFlight->convertToWorldSpace(Vec2(0, 0));
        if (tmpFlightPos.x < 0 || tmpFlightPos.x > tmpWinSize.width || tmpFlightPos.y < 0 || tmpFlightPos.y > tmpWinSize.height)
        {
            tmpFlight->shouldBeDel  =   true;
        }

        // 删除被标记为要删除的飞行道具
        if (tmpFlight->shouldBeDel)
        {
            removeFlightProps(tmpFlight);
        }
    }
}

bool FlightPropsMgr::collisionTest(int id, FlightProps* flight)
{
    auto tmpShape = (GameCharacterShape*)EntityMgr->getEntityFromID(id)->getShape();
    //return tmpShape->getCollisionRect().intersectsRect(flight->getCollisionRect());
    //return flight->getCollisionRect().containsPoint(tmpShape->getCenterPos());
    return tmpShape->getCollisionRect().containsPoint(flight->getPosition());
}