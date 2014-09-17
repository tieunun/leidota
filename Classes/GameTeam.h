#ifndef __GAME_TEAM_H__
#define __GAME_TEAM_H__

#include "cocos2d.h"
#include "StateMachine.h"
#include "Telegram.h"
#include "BaseGameEntity.h"
#include "Formation.h"

using namespace cocos2d;
using namespace std;

class GameCharacter;

/**
	 游戏中我方队伍的概念，为了实现队伍级别的组合、AI的概念
     @_@    这里就不考虑敌方队伍这个概念
*/
class GameTeam : public Ref
{
public:
    typedef StateMachine<GameTeam>  TeamStateMachine;

    GameTeam();
    ~GameTeam();

    /**
    	 设置主角全局索引id
    */
    void setLeaderId(GameCharacter* player);

    /**
    	 添加佣兵
    */
    void addMercenaryIds(GameCharacter* player);

    /**
    	 每一帧中回调
    */
    void update(float dm);

    bool init();

    /**
    	 向全体队员发送某个消息
    */
    void sendMsgToAll(Telegram& msg);

    /**
    * 为了实现队伍级别的推进，队伍中角色移动的时候会向上级也就是队伍通知 
    */
    void playerMoving(GameCharacter* player);

    /**
    	 返回状态机，方便在状态中修改
    */
    TeamStateMachine* getFSM();

    // 返回队伍阵型
    Formation& getTeamFormation() { return m_formation; }

    /**
    	 队伍id，用来全局唯一标示一个队伍
    */
    CC_SYNTHESIZE(int, m_teamId, TeamId);
    CC_SYNTHESIZE_READONLY(GameEntityTypeEnum, m_type, TeamType);

    CREATE_FUNC(GameTeam);

private:

    /**
    * 向所有的佣兵发送消息 
    */
    void sendToMercenaries( Telegram& msg );

    static int m_nextValidId;                                   // 下一个有效地队伍id
    /**
    	 这里之所以没有直接使用GameCharacter是因为所有的最好都是从EntityManager中获取
    */
    int                         m_leaderId;                     // 主角id
    list<int>                   m_mercenaryIdList;              // 佣兵id列表
    TeamStateMachine*           m_stateMachine;                 // 该队伍的状态机

    Formation                   m_formation;                    // 阵型
};

#endif