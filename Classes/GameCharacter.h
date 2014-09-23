#ifndef __GAME_CHARACTER_H__
#define __GAME_CHARACTER_H__

#include "BaseGameEntity.h"
#include "GameCharacterShape.h"
#include "GameCharacterAttribute.h"

#include "WeaponControlSystem.h"

#include "MovingEntity.h"
#include "SteeringBehaviors.h"

using namespace std;

class GameTeam;
class GoalCharacterThink;

/**
	 在打仗中的游戏角色
*/
class GameCharacter : public BaseGameEntity
{
public:
    /**
    	 @_@ 主要是创建一个角色需要的参数太多，以后应该是从配置表中获取，
         这里先手写一些角色的类
    */
    static GameCharacter* create(int id);

    /**
    	 每一帧的更新
    */
    virtual void update(float dm) override;

    /**
    	 用来处理消息的
    */
    virtual bool handleMessage(Telegram& msg) override;

    GameCharacterShape *getShape() { return m_shape; };

    /**
    	 关于角色属性的部分
    */
    GameCharacterAttribute& getAttribute() { return m_attribute; }
    WeaponControlSystem* const getWeaponControlSystem() { return m_weaponControlSystem; }
    MovingEntity& getMovingEntity() { return m_movingEntity; }
    SteeringBehaviors* const getSteeringBehaviros() { return m_steeringBehaviors; }

    /**
    * 有关当前角色状态的判断 
    */
    bool isAlive() { return m_state == alive; }
    bool isDead() { return m_state == dead; }

    /**
    * 有关当前目标的判断，比如在队伍分配目标的时候会用到这个接口
    */
    bool hasGoal();

    // 设置和返回该角色所属的队伍 
    CC_SYNTHESIZE(GameTeam*, m_team, Team);

    // 角色id，表示一种类型的人物 
    CC_SYNTHESIZE_READONLY(int, m_characterId, CharacterId);

protected:
    GameCharacter();
    ~GameCharacter();

    /**
    * 更新当前的坐标 
    */
    void updateMovement(float dm);

    /**
    * 角色的当前状态
    * @_@ 目前只有死和活两种状态
    */
    enum GameCharacterStateEnum
    {
        alive,                  // 当前活着
        dead,                   // 已经死了
    };

    GameCharacterShape*             m_shape;                    // 该角色的外形
    GameCharacterAttribute          m_attribute;                // 该角色的各种属性

    GoalCharacterThink*             m_brain;                    // 作为大脑存在的，是目标规划的最高级别

    /**
    * 一些系统
    */
    WeaponControlSystem*            m_weaponControlSystem;      // 武器系统

    MovingEntity                    m_movingEntity;             // 用来代表角色移动的对象
    SteeringBehaviors*              m_steeringBehaviors;        // 驱动力产生对象

    double                          m_lastUpdateTime;           // 最近一次调用update的时间
    GameCharacterStateEnum          m_state;                    // 角色当前当前状态
};

#endif