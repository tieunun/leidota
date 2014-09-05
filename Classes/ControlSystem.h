#ifndef __CONTROL_SYSTEM_H__
#define __CONTROL_SYSTEM_H__

class GameCharacter;

/**
* 最终决定将一个角色的所有控制部分分离到每个控制系统中，比如移动控制系统负责将人物移动到要去的地方
* 攻击目标选择系统负责根据环境选择攻击目标，火控系统负责根据当前环境选择合适的攻击方式，这个作为控制
* 组建包含在每个GameCharacter中，每个GameCharacter定期更新这些组件
*/
class ControlSystem
{
public:
    ControlSystem(GameCharacter* owner, float updatePeriod = 0);
    ~ControlSystem();

    /**
    * 外部每次GameCharacter的update的时候都可以调用tryUpdate，但是因为这些系统没必要更新这么频繁，所以
    * 这里对update做了控制，保证了两次调用update的最短时间间隔
    */
    void tryUpdate();

protected:
   /**
    *  更新控制系统组件状态
    */
    virtual void update(float dm) = 0;

    GameCharacter*  m_owner;                        // 该系统的所有者

private:
    float   m_updatePeriod;                         // 两次update的最短时间间隔
    float   m_lastUpdateTime;                       // 最近一次调用update的时间，单位是毫秒
};

#endif