#ifndef __GAME_MAP_H__
#define __GAME_MAP_H__

#include "cocos2d.h"
#include "MapGrid.h"
#include "GameCharacter.h"

using namespace cocos2d;

/**
	 游戏地图，同时上面会有网格
*/
class GameMap : public Layer
{
public:
    bool init () override;

    /**
    	 关于阵型的不好搞，干脆固定算了，@_@ 以后再根据配置来做，这里的三个函数
         对应三个进入口
    */
    void placeCharacter1(GameCharacter* character);
    void placeCharacter2(GameCharacter* character);
    void placeCharacter3(GameCharacter* character);

    /**
    	 敌人阵型如何放，以后也需要通过特定的方式，这里就FUCK直接放到固定的位置上
    */
    void placeEnemyCharacter1(GameCharacter* character);
    void placeEnemyCharacter2(GameCharacter* character);
    void placeEnemyCharacter3(GameCharacter* character);

    CREATE_FUNC(GameMap);

protected:

    GameMap():GRIDW(64),GRIDH(60),XNUM(20),YNUM(5){}
    ~GameMap();

    /**
    	 用来方便将某个人物安置到某个网格上的
    */
    void placeOneCharacterToIndex(GameCharacter* character, int nodeIndex);

    Node*       m_bg;               // 背景图片
    MapGrid*    m_mapGrid;          // 地图上面供人移动的网格

    /**
    	 地图网格的一些常量，以后应该在配置表里面
    */
    const int GRIDW;
    const int GRIDH;
    const int XNUM;
    const int YNUM;
};

#endif