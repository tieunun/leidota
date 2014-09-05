#ifndef __MOBILITY_CONTROL_SYSTEM_H__
#define __MOBILITY_CONTROL_SYSTEM_H__

#include "ControlSystem.h"

/**
* 角色的移动控制系统，之前把角色的移动都放在状态机中，现在觉得每个角色应该有能力管理自己的移动 
*/
class MobilityControlSystem : public ControlSystem
{
public:
    MobilityControlSystem(GameCharacter* owner, float updatePeriod = 0);

protected:
    void update(float dm) override;
};

#endif