#ifndef __FORMATION_H__
#define __FORMATION_H__

#include "cocos2d.h"

using namespace cocos2d;

/**
* 小队的移动阵型，在部队集体移动的时候要保持这个阵型，这个阵型如下
* 对于左边的
* 6      3     0 
* 7      4     1
* 8      5     2
* 对于右边的
* 0      3     6
* 1      4     7
* 2      5     8
*/
class Formation
{
public:
    /**
    * 阵型样式 
    */
    enum FormationTypeEnum
    {
        FORMATION_TYPE_RIGHT,               // 面向右边的阵型
        FORMATION_TYPE_LEFT,                // 面向左边的阵型
    };

    Formation(FormationTypeEnum type = FORMATION_TYPE_RIGHT):m_xPosInterval(200), m_yPosInterval(115)
    {
        m_formationType =   type;
    }

    /**
    * 在阵型中的每个一个角色都有一个编号，根据编号，在阵型移动的时候会尽可能在指定的位置上
    * 这里就是根据阵型中的编号返回对应的坐标
    */
    Vec2 getPositionByPosId(int posId)
    {
        float   tmpxInterval    =   0;
        switch (m_formationType)
        {
        case FORMATION_TYPE_LEFT:
            tmpxInterval    =   m_xPosInterval;
            break;

        case FORMATION_TYPE_RIGHT:
            tmpxInterval    =   -m_xPosInterval;
            break;
        }

        return m_formationAnchor + Vec2(posId / 3 * tmpxInterval, - posId % 3 * m_yPosInterval);
    }

    CC_SYNTHESIZE(Vec2, m_formationAnchor, FormationAnchor);                // 阵型的锚点，其实就是0号位
    CC_SYNTHESIZE(FormationTypeEnum, m_formationType, FormationType);       // 阵型样式

private:
    /**
    * 阵型中的一些常量 
    */
    const float             m_xPosInterval;                 // x方向上的偏移
    const float             m_yPosInterval;                 // y方向上的偏移
};

#endif