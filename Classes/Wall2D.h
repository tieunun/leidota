#ifndef __WALL_2D_H__
#define __WALL_2D_H__

#include "cocos2d.h"

using namespace cocos2d;

/**
* 在地图上表示四周的墙，防止角色跑出去了 
*/
class Wall2D
{
public:
    Wall2D(Vec2 pointA, Vec2 pointB, Vec2 normal)
    {
        m_pointA    =   pointA;
        m_pointB    =   pointB;
        m_normal    =   normal.getNormalized();
    }

    CC_SYNTHESIZE_READONLY(Vec2, m_pointA, PointA);           // 一条线的两个点
    CC_SYNTHESIZE_READONLY(Vec2, m_pointB, PointB);
    CC_SYNTHESIZE_READONLY(Vec2, m_normal, Normal);           // 法线，指向内侧
};

#endif