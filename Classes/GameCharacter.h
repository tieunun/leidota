#ifndef __GAME_CHARACTER_H__
#define __GAME_CHARACTER_H__

#include "BaseGameEntity.h"
#include "GameCharacterShape.h"
#include "StateMachine.h"
#include "MapGrid.h"
#include "GameCharacterAttribute.h"

using namespace std;

/**
	 游戏角色类型
*/
enum GameCharacterTypeEnum
{
    GAMECHARACTER_TYPE_ENUM_SHORT_RANGE,                // 近程攻击单位
    GAMECHARACTER_TYPE_ENUM_LONG_RANGE                  // 远程攻击单位
};

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

    /**
    	 返回该人物的状态机
    */
    StateMachine<GameCharacter> *getFSM();

    GameCharacterShape *getShape() override;

    /**
    	 设置该角色所在的网格
    */
    void setGridGraph(MapGrid* graph);

    /**
    	 用来访问和修改该对象在网格上的占位
    */
    ObjectOnMapGrid* getObjectOnGrid();

    /**
    	 将该角色移动到网格上面索引为nodeIndex的格子，一定是一个相邻的格子，先在网格上
         占位置，然后缓动过去，这里要保证nodeIndex是可以走的
    */
    void moveToGridIndex(int nodeIndex, float rate = 10);

    /**
    	 觉得还是让AI能够直接访问网格会好些
    */
    MapGrid* getMapGrid();

    /**
    	 当前是否在移动中
    */
    bool isMoving();

    /**
    	 关于角色属性的部分
    */
    GameCharacterAttribute& getAttribute();

    /**
    	 角色死亡时的处理
    */
    void die();

    /**
    	 返回角色类型
    */
    GameCharacterTypeEnum getCharacterType();

    /**
    	 普通攻击，当可以切换状态的时候回调callBack
    */
    void normalAttack(int id);

    /**
    	 当进行普通攻击后，我们需要知道什么时候结束，可以在逻辑帧中判断
    */
    bool isNormalAttackFinish();

    /**
    	 判断另一个人物是否在该角色的攻击范围内
    */
    bool isInAttackDistance(GameCharacter* other);

protected:
    GameCharacter();
    ~GameCharacter();

    /**
    	 当移动结束的时候
    */
    void onMoveOver(Node* pNode);

    /**
    	 当进程攻击其效果的时候，在此处向目标发出通知
    */
    void onShortAttEffect(string evt);

    /**
    	 当远程攻击单位要丢出飞行道具的时候回调，在此处完成飞行道具的创建
    */
    void onLongAttLaunch(string evt);

    StateMachine<GameCharacter>*    m_stateMachine;             // 该角色的状态机，相当于该角色的AI
    GameCharacterShape*             m_shape;                    // 该角色的外形
    MapGrid*                        m_graph;                    // 该角色所在的网格
    ObjectOnMapGrid                 m_objectOnGrid;             // 该对象在网格上的占位
    GameCharacterAttribute          m_attribute;                // 该角色的各种属性
    GameCharacterTypeEnum           m_characterType;            // 角色类型

    MoveTo*    m_moveAction;                                    // 用来移动的

    int m_normatAttTargetId;                                    // 临时保存的攻击目标id
};

#endif