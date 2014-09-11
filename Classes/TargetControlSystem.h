#ifndef __TARGET_CONTROL_SYSTEM_H__
#define __TARGET_CONTROL_SYSTEM_H__

#include "cocos2d.h"
#include "ControlSystem.h"

using namespace cocos2d;

/**
* 目标控制系统，角色通过该对象来决定当前的攻击目标，之前是放在Auto中，每次更新的时候都会检查一下，这样
* 攻击目标就无法固定下来，而且也不好写集火的部分，所以将此逻辑独立到目标控制系统中
* @_@ 但是目前还是需要GameCharacterAutoState来帮助
*/
class TargetControlSystem : public ControlSystem
{
public:
    TargetControlSystem(GameCharacter* owner, float updatePeriod = 0);
    ~TargetControlSystem();

    /**
    * 外部强制该角色攻击某个目标
    */
    void forceTargetId(int id);

    /**
    * 与上面的不同，不是强制设置攻击目标，因为目前在自动状态下攻击目标的选择是由GameCharacterAutoState中的逻辑
    * 来控制的，所以提供该接口
    */
    void setTargetId(int id);

    /**
    * 返回当前该角色的攻击目标
    */
    GameCharacter* getTargetCharacter();
   
    /**
    *  当前系统的状态，决定了目标选择的方式
    */
    enum ChoiceTargetState
    {
        CHOICE_TARGET_AUTO_PRECEDENCE,                                      // 优先自动选择目标（一般就是根据距离）
        CHOICE_TARGET_USER_PRECEDENCE                                       // 优先选择玩家指定的，只有当无法攻击的时候才会切换为自动
    };
    CC_SYNTHESIZE_READONLY(ChoiceTargetState, m_choiceTargetState, ChoiceTargetState);

protected:
    void update(float dm) override;

    /**
    *  根据内部的逻辑调整当前攻击的目标
    */
    void updateTarget();

    int m_targetId;                                 // 攻击目标id
};

#endif