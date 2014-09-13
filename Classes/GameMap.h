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
    * 设置摄像机在初始位置移动了多少，这样来调整4个层的移动，每个层相对于此移动不同的距离，其中摄像机
    * 是跟着主角的
    */
    void cameraMove(int x);

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

    /**
    	 用来方便将某个人物安置到某个网格上的
    */
    void placeOneCharacterToIndex(GameCharacter* character, int nodeIndex);

    /**
    * 每一帧的回调 
    */
    void update(float);

    CREATE_FUNC(GameMap);

    /**
    	 地图网格的一些常量，以后应该在配置表里面
    */
    const int GRIDW;
    const int GRIDH;
    const int XNUM;
    const int YNUM;

protected:

    GameMap():GRIDW(64),GRIDH(60),XNUM(65),YNUM(5),MOUNTAIN_MOVE_SCALE(0.5f),SKY_MOVE_SCALE(0.2f){}
    ~GameMap();

    /**
    * 构成地图的背景有4层，现在每一层都是由两幅图片拼成的
    */
    Node*       m_bg1;               // 人物所在的地面
    Node*       m_bg2;               // 背后的山
    Node*       m_bg3;               // 最后面的天空

    MapGrid*    m_mapGrid;          // 地图上面供人移动的网格

    /**
    * 为了形成分层，这里是人物移动的距离和其它层移动距离的比值 
    */
    const float MOUNTAIN_MOVE_SCALE;
    const float SKY_MOVE_SCALE;

    /**
    * 临时记录开始的时候的x 
    */
    float   m_initPosX;                 // 地图初始的时候的x
};

#endif