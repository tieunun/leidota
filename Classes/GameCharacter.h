#ifndef __GAME_CHARACTER_H__
#define __GAME_CHARACTER_H__

#include "BaseGameEntity.h"
#include "GameCharacterShape.h"
#include "GameCharacterAttribute.h"

#include "WeaponControlSystem.h"
#include "TargetControlSystem.h"

#include "MovingEntity.h"
#include "SteeringBehaviors.h"

using namespace std;

class GameTeam;
class GoalThink;

/**
	 在打仗中的游戏角色
*/
class GameCharacter : public BaseGameEntity
{
public:
    /**
    * 角色的当前状态
    * @_@ 目前只有死和活两种状态
    */
    enum GameCharacterStateEnum
    {
        alive,                  // 当前活着
        dead,                   // 已经死了
    };

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

    GameCharacterShape *getShape() override;

    /**
    	 关于角色属性的部分
    */
    GameCharacterAttribute& getAttribute();
    WeaponControlSystem* const getWeaponControlSystem();
    TargetControlSystem* const getTargetControlSystem();
    MovingEntity& getMovingEntity() { return m_movingEntity; }
    SteeringBehaviors* const getSteeringBehaviros() { return m_steeringBehaviors; }

    // 设置和返回该角色所属的队伍 
    CC_SYNTHESIZE(GameTeam*, m_team, Team);

    // 角色id，表示一种类型的人物 
    CC_SYNTHESIZE_READONLY(int, m_characterId, CharacterId);

    // 角色的当前状态
    CC_SYNTHESIZE_READONLY(GameCharacterStateEnum, m_state, State);

protected:
    GameCharacter();
    ~GameCharacter();

    /**
    * 更新当前的坐标 
    */
    void updateMovement(float dm);

    GameCharacterShape*             m_shape;                    // 该角色的外形
    GameCharacterAttribute          m_attribute;                // 该角色的各种属性

    GoalThink*                      m_brain;                    // 作为大脑存在的，是目标规划的最高级别

    /**
    * 一些系统
    */
    WeaponControlSystem*            m_weaponControlSystem;      // 武器系统
    TargetControlSystem*            m_targetControlSystem;      // 目标选择系统

    MovingEntity                    m_movingEntity;             // 用来代表角色移动的对象
    SteeringBehaviors*              m_steeringBehaviors;        // 驱动力产生对象

    float                           m_lastUpdateTime;           // 最近一次调用update的时间
};

#endif