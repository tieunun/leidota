#ifndef __GAME_TEAM_H__
#define __GAME_TEAM_H__

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

/**
	 游戏中队伍的概念，为了实现队伍级别的组合、AI的概念
*/
class GameTeam
{
public:
    GameTeam();

private:
    /**
    	 这里之所以没有直接使用GameCharacter是因为所有的最好都是从EntityManager中获取
    */
    int         m_leaderId;                     // 主角id
    list<int>   m_mercenaryIdList;              // 佣兵id列表
};

#endif