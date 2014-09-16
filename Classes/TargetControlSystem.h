#ifndef __TARGET_CONTROL_SYSTEM_H__
#define __TARGET_CONTROL_SYSTEM_H__

class GameCharacter;

/**
* 目标控制系统，负责给该角色设置攻击目标，这里是为了封装目标选择的逻辑
*/
class TargetControlSystem
{
public:
    TargetControlSystem(GameCharacter* owner);
    ~TargetControlSystem();

    /**
    *  更新一下当前的目标选择系统的内部信息
    */
    void update();

    /**
    * 返回当前的目标 
    */
    GameCharacter* getTarget();

private:
    GameCharacter*  m_pOwner;                   // 所有者
    int             m_targetId;                 // 当前选定的攻击目标id
};

#endif