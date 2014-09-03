#ifndef __GAME_CHARACTER_H__
#define __GAME_CHARACTER_H__

#include "BaseGameEntity.h"
#include "GameCharacterShape.h"
#include "StateMachine.h"
#include "MapGrid.h"
#include "GameCharacterAttribute.h"

using namespace std;

class GameTeam;

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
    * 当前是否可以普通攻击，因为存在所谓的最短间隔 
    */
    bool canNormalAttack();

    /**
    	 普通攻击，当可以切换状态的时候回调callBack
    */
    void normalAttack(int id);

    /**
    * 每次脱离普通攻击的时候被调用 
    */
    void exitNormalAttack();

    /**
    * 返回距离下一次可以攻击的数 
    */
    int getNextNormatAttackLeftCount();

    /**
    	 当进行普通攻击后，我们需要知道什么时候结束，可以在逻辑帧中判断
    */
    bool isNormalAttackFinish();

    /**
    	 判断另一个人物是否在该角色的攻击范围内
    */
    bool isInAttackDistance(GameCharacter* other);

    /**
    *  在视线范围内的所有角色，这里范围以网格为单位
    */
    vector<GameCharacter*> getCharactersInView();

    /**
    * 返回在视线范围内的所有敌人（敌人是相对的概念） 
    */
    vector<GameCharacter*> getFoeCharactersInView();

    /**
    * 如果other想跟随该角色，那么应该站在网格的哪些位置呢，这里按照优先级顺序排列
    */
    vector<int> getFollowGridIndex(GameCharacter* other);

    /**
    * 受到普通攻击的接口 
    */
    void sufferNormalAttack(GameCharacterAttribute& attribute);

    /**
    * 设置和返回该角色所属的队伍 
    */
    CC_SYNTHESIZE(GameTeam*, m_team, Team);

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

    /**
    *  在返回视野范围内的其他角色的时候按照的排序函数，按照里该角色的距离从
    *  近到远来排列
    */
    typedef std::function<bool (GameCharacter* character1, GameCharacter* character2)> SortFunc;
    bool charactersInViewSortFunc(GameCharacter* character1, GameCharacter* character2);

    /**
    * 返回某个角色的跟随地址
    */
    typedef std::function<bool (int gridIndex1, int gridIndex2)> FollowGridSortFunc;
    bool followGridSortFunc(int index1, int index2);

    StateMachine<GameCharacter>*    m_stateMachine;             // 该角色的状态机，相当于该角色的AI
    GameCharacterShape*             m_shape;                    // 该角色的外形
    MapGrid*                        m_graph;                    // 该角色所在的网格
    ObjectOnMapGrid                 m_objectOnGrid;             // 该对象在网格上的占位
    GameCharacterAttribute          m_attribute;                // 该角色的各种属性
    GameCharacterTypeEnum           m_characterType;            // 角色类型

    MoveTo*    m_moveAction;                                    // 用来移动的

    /**
    * @_@ 保存的一些临时数据 
    */
    int m_normatAttTargetId;                                    // 临时保存的攻击目标id
    int m_frameCount;                                           // 每一帧累加
    int m_lastExitNormalAttackFrame;                            // 保存上一次离开普通攻击状态的帧数
};

#endif