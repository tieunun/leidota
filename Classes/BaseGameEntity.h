#ifndef __BASE_GAME_ENTITY_H__
#define __BASE_GAME_ENTITY_H__

#include "Telegram.h"

/**
	 游戏实体类型，将游戏实体进行分类
*/
enum GameEntityTypeEnum
{
    GAME_ENTITY_TYPE_PLAYER_CHARACTER,                  // 玩家的角色
    GAME_ENTITY_TYPE_ENEMY_CHARACTER                    // 敌人的角色
};

#define INVALID_GAME_ENTITY_ID  -1

/**
	 游戏中所有的实体的基类
*/
class BaseGameEntity : public cocos2d::Ref
{
private:
    int                 _id;               // 每一个实体都有一个唯一的id
    GameEntityTypeEnum  _type;

    static int m_nextValidId;           // 实体的id采用自增的方式来给每一个实体设置唯一的id

protected:
    BaseGameEntity();
    virtual ~BaseGameEntity();

public:
    int getId();
    void setType(GameEntityTypeEnum type) {_type = type;};
    GameEntityTypeEnum getType() {return _type;};

    virtual void update(float dm) = 0;                      // 在每一帧中被调用
    virtual bool handleMessage(Telegram& msg) = 0;          // 所有实体都可以处理消息交流
    virtual Sprite* getShape() {return nullptr;}
};

#endif