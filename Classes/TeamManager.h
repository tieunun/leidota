#ifndef __TEAM_MANAGER_H__
#define __TEAM_MANAGER_H__

#include "cocos2d.h"
#include "GameTeam.h"

using namespace std;

/**
	 为了管理当前存在的所有队伍，因为考虑到以后可能会存在非常多的编队，作为
     全局单件
*/
class TeamManager
{
public:
    typedef map<int, GameTeam*> TeamMap;

    static TeamManager* instance();

    /**
    	 每一帧调用，其实就是更新每一支队伍
    */
    void update(float dm);

    /**
    	 注册一个队伍
    */
    void registerTeam(GameTeam* pTeam);

    /**
    	 移除一个队伍
    */
    void removeTeam(GameTeam* pTeam);

    /**
    	 为了方便外部直接访问所有的队伍
    */
    const TeamMap& getTeamMap();

    /**
    	 返回
    */
    GameTeam* getTeamFromId(int id);

private:
    static TeamManager* m_instance;

    TeamMap             m_allTeam;                  // 保存所有的队伍 
};

#define TeamMgr TeamManager::instance()

#endif