#ifndef __FLIGHT_PROPS_H__
#define __FLIGHT_PROPS_H__

#include "cocos2d.h"
#include "cocostudio/CCArmature.h"
#include "GameCharacter.h"
#include "EntityManager.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace std;

/**
	 飞行道具类型
*/
enum FlightPropsTypeEnum
{
    FLIGHTPROPS_TYPE_FIXED_POINT_ONE_TARGET                 // 发射后目标坐标固顶并且只攻击一个敌人的飞行道具
};

/**
	 飞行中的道具，在创建该道具的时候需要设置内容尺寸，以方便
     相交检测
*/
class FlightProps : public Sprite
{
protected:
    FlightProps(FlightPropsTypeEnum type, int senderId, Vec2 velocity);

public:
    /**
    	 更新当前正在飞行中的道具
    */
    virtual void update(float dm);

    /**
    	当检测到碰撞的时候的回调
    */
    virtual void onCollision(int id){};

    /**
    	 返回用来检测碰撞的矩形
    */
    virtual Rect getCollisionRect();

    FlightPropsTypeEnum     type;                       // 道具类型
    int                     senderId;                   // 发送者id
    Vec2                    velocity;                   // 飞行速度
    GameCharacterAttribute  senderAtt;                  // 发送者的属性，用于计算效果（可能在飞行过程中发送者死掉了）

    bool                    shouldBeDel;                // 要被删除的标记
    bool                    isEffective;                // 是否有效（可能存在该飞行道具仍然存在，但是已经无效）
};

/**
    点对点的远程攻击，并且飞行道具是Armature，所以必须在固定帧数内到达，所以速度
    不是固定的
*/
class OneToOneArmatureFlightProps : public FlightProps
{
protected:
    int         m_targetId;                         // 接收者id
    string      m_actionName;                       // 动作名称
    Armature*   m_armature;                         // 动画

    /**
    	 发送、接收者，播放的动画以及飞行速率
    */
    OneToOneArmatureFlightProps(int senderId, int targetId, Armature* armature, string actionName, float rate);
    ~OneToOneArmatureFlightProps();

    /**
    	 卧槽，在构造函数中将Armature加入
    */
    bool init() override;

public:
    /**
    	 创建一个点对点攻击的飞行道具
    */
    static OneToOneArmatureFlightProps* create(int senderId, int targetId, int skillId);
    void onCollision(int id);

    void update(float dm);
};

#endif