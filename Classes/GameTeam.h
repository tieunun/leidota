#ifndef __GAME_TEAM_H__
#define __GAME_TEAM_H__

#include "cocos2d.h"
#include "Telegram.h"
#include "BaseGameEntity.h"
#include "Formation.h"

using namespace cocos2d;
using namespace std;

class GameCharacter;
class GoalTeamThink;

/**
*	队伍类型，当前主要是用来分清哪个队伍是玩家，哪个队伍是敌人 
*/
enum GameTeamTypeEnum
{
    GAME_TEAM_TYPE_PLAYER,          // 玩家队伍
    GAME_TEAM_TYPE_ENEMY,           // 敌方队伍
};

/**
	 游戏中我方队伍的概念，为了实现队伍级别的组合、AI的概念
     @_@    这里就不考虑敌方队伍这个概念
*/
class GameTeam : public Ref
{
public:
    ~GameTeam();

    // 给该队伍添加成员，同时指定该角色在阵型中的位置编号
    void addMember(GameCharacter* player, int posId = 0);

    // 每一帧中回调，在这里会调用所有成员的update
    void update(float dm);

    // 返回队伍阵型
    Formation& getTeamFormation() { return m_formation; }

    // 开始集体前进
    void collectiveForwardStart();

    // 结束集体前进（在开始攻击敌方队伍之前要调用来解除保持阵型的约束）
    void collectiveForwardEnd();

    // 给所有的成员发送消息，期望以后队伍级别的作为成员的脑袋，给每一个成员分配目标
    void sendMessageToAllMember(Telegram& msg);

    // 给某个指定的角色发送消息
    void sendMessageToOneMember(Telegram& msg, GameCharacter* aCharacter);

    // 判断是否所有人都在阵型指定位置
    bool isEveryMemberInPos();

    // 从阵型中找到指定位置id的成员，如果没有了，就返回null
    GameCharacter* getMemberIdFromFormation(int posId);

    const list<GameCharacter*>& getMembers() { return m_members; }

    // 返回队伍大脑
    GoalTeamThink* getTeamBrain() { return m_teamBrain; }

    // 队伍id，用来全局唯一标示一个队伍
    CC_SYNTHESIZE(int, m_teamId, TeamId);

    // 创建一个队伍，在调用这个函数后该队伍就被加入到TeamMgr中了
    static GameTeam* create(GameTeamTypeEnum teamType);

    // 是否可以被删除
    bool canRemove() { return (m_teamState & GAME_TEAM_STATE_REMOVE) != 0; }

private:
    GameTeam();

    // 删除处于死亡状态的角色
    void removeDeadCharacter();

    void setCanRemove() { m_teamState |= GAME_TEAM_STATE_REMOVE; }

    /**
    *	关于移动状态的 
    */
    void setCollectiveForwardState() { m_teamMoveState = GAME_TEAM_MOVE_COLLECTIVEFORWARD; }
    void setStayState() { m_teamMoveState = GAME_TEAM_MOVE_STAY; }
    void setFollowPlayerState() { m_teamMoveState = GAME_TEAM_MOVE_FOLLOW_PLAYER; }

    bool iscollectiveForwardState() { return m_teamMoveState == GAME_TEAM_MOVE_COLLECTIVEFORWARD; }
    bool isStayState() { return m_teamMoveState == GAME_TEAM_MOVE_STAY; }
    bool isFollowPlayerState() { return m_teamMoveState == GAME_TEAM_MOVE_FOLLOW_PLAYER; }

    static int m_nextValidId;                                       // 下一个有效地队伍id

    /**
    *	队伍当前状态的位
    */
    enum GameTeamStateEnum
    {
        GAME_TEAM_STATE_ACTIVE  =   0x0001,         // 当前队伍处于活跃状态
        GAME_TEAM_STATE_REMOVE  =   0x0002,         // 可以移除当前队伍了
    };

    list<GameCharacter*>        m_members;                          // 队伍的所有成员
    Formation                   m_formation;                        // 阵型

    GoalTeamThink*              m_teamBrain;                        // 队伍级别的大脑

    float m_advanceRate;                                            // 队伍推进速度

    CC_SYNTHESIZE_READONLY(GameTeamTypeEnum, m_teamType, TeamType); // 队伍类型

    int                        m_teamState;                        // 队伍当前状态

    /**
    *	队伍当前移动状态 
    */
    enum GameTeamMoveStateEnum
    {
        GAME_TEAM_MOVE_STAY,                                    // 固顶
        GAME_TEAM_MOVE_COLLECTIVEFORWARD,                       // 集体前进
        GAME_TEAM_MOVE_FOLLOW_PLAYER,                           // 依据成员
    };
    GameTeamMoveStateEnum       m_teamMoveState;
};

#endif