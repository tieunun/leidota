#include "GameCharacter.h"
#include "GameCharacterState.h"
#include "FlightProps.h"
#include "MessageDispatcher.h"
#include "UIViewManager.h"
#include "TargetControlSystem.h"
#include "PathPlanner.h"
#include "GoalThink.h"

GameCharacter* GameCharacter::create(int id)
{
    /**
    	 临时就创建一种角色，这里是将组成一个游戏角色的部分拼接在一起
    */
    auto tmpRet = new GameCharacter();
    tmpRet->autorelease();

    /**
    	 Fuck主要是以后会有多种人物，这里吧，暂时就这样搞
    */
    /**
        在此处拼装状态机、外形等
    */
    tmpRet->m_stateMachine  =   StateMachine<GameCharacter>::create(tmpRet);
    tmpRet->m_stateMachine->retain();
    tmpRet->m_characterId   =   id;
    switch (id)
    {
    case 1:                                                 // 对应的是宙斯
        {
            // 不同的角色有不同的外形
            tmpRet->m_shape         =   GameCharacterShape::create("zhousi.ExportJson", "zhousi");
            tmpRet->m_shape->retain();

            // 不同的角色有不同的状态转换表
            //tmpRet->m_stateMachine->changeState(GameCharacterIdleState::create());
            //tmpRet->m_stateMachine->setGlobalState(GameCharacterGlobalState::create());

            // 不同的角色有不同的初始属性
            tmpRet->m_attribute     =   GameCharacterAttribute(200, 10, 30, 70);

            // 角色类型，宙斯属于近程攻击单位
            tmpRet->m_characterType =   GAMECHARACTER_TYPE_ENUM_SHORT_RANGE;

            break;
        }

    case 2:                                                 // 精灵
        {
            tmpRet->m_shape         =   GameCharacterShape::create("xuejingling-qian.ExportJson", "xuejingling-qian");
            tmpRet->m_shape->retain();
            // 另一套状态转换表@_@ 以后看能不能将这部分的AI逻辑移到lua中去，不然坑死我了
            // 目前先使用一套AI逻辑@_@  FUCK
            // auto tmpState = GameCharacterPursueState::create();
            // tmpState->targetId  =   4;
            //tmpRet->m_stateMachine->changeState(GameCharacterIdleState::create());
            //tmpRet->m_stateMachine->setGlobalState(GameCharacterGlobalState::create());

            tmpRet->m_attribute     =   GameCharacterAttribute(100, 40, 10, 90, 700);
            
            // 雪精灵远程攻击单位
            tmpRet->m_characterType =   GAMECHARACTER_TYPE_ENUM_LONG_RANGE;

            break;
        }

    case 3:                                                 // 骑士
        {
            tmpRet->m_shape         =   GameCharacterShape::create("Aer.ExportJson", "Aer");
            tmpRet->m_shape->retain();

            //tmpRet->m_stateMachine->changeState(GameCharacterIdleState::create());
            //tmpRet->m_stateMachine->setGlobalState(GameCharacterGlobalState::create());

            tmpRet->m_attribute     =   GameCharacterAttribute(150, 20, 20, 80);
            
            // 骑士：进程攻击单位
            tmpRet->m_characterType =   GAMECHARACTER_TYPE_ENUM_SHORT_RANGE;

            break;
        }

    case 4:                                                 // 野猪怪
        {
            tmpRet->m_shape         =   GameCharacterShape::create("Pig.ExportJson", "Pig");
            tmpRet->m_shape->retain();

            //tmpRet->m_stateMachine->changeState(GameCharacterIdleState::create());
            //tmpRet->m_stateMachine->setGlobalState(GameCharacterGlobalState::create());

            tmpRet->m_attribute     =   GameCharacterAttribute(400, 1, 10, 60 + CCRANDOM_0_1() * 20);

            // 野猪怪：近程攻击单位
            tmpRet->m_characterType =   GAMECHARACTER_TYPE_ENUM_SHORT_RANGE;

            break;
        }

    case 5:                                                 // 牛人
        {
            tmpRet->m_shape         =   GameCharacterShape::create("Niu.ExportJson", "Niu");
            tmpRet->m_shape->retain();

            //tmpRet->m_stateMachine->changeState(GameCharacterIdleState::create());
            //tmpRet->m_stateMachine->setGlobalState(GameCharacterGlobalState::create());

            tmpRet->m_attribute     =   GameCharacterAttribute(400, 1, 10, 50 + CCRANDOM_0_1() * 20);

            // 牛人：近程攻击单位
            tmpRet->m_characterType =   GAMECHARACTER_TYPE_ENUM_SHORT_RANGE;

            break;
        }

    default:
        break;
    }
    
    
    return tmpRet;
}

GameCharacter::GameCharacter()
{
    m_stateMachine                  =   nullptr;
    m_shape                         =   nullptr;
    m_graph                         =   nullptr;
    m_moveAction                    =   nullptr;
    m_team                          =   nullptr;
    m_frameCount                    =   0;
    m_lastExitNormalAttackFrame     =   0;

    /**
    * 各种控制系统 
    */
    m_targetControlSystem           =   new TargetControlSystem(this, 0.5);

    // 路径规划器
    m_pathPlanner                   =   new PathPlanner(this);

    // 角色的大脑
    m_brain                         =   new GoalThink(this);
}

GameCharacter::~GameCharacter()
{
    // 移除该角色在网格中所占的坑位
    m_graph->removeObjectFromGrid(&m_objectOnGrid);
    CC_SAFE_RELEASE_NULL(m_stateMachine);

    // 将该角色的显示从显示列表中移除
    m_shape->removeFromParent();
    CC_SAFE_RELEASE_NULL(m_shape);

    CC_SAFE_DELETE(m_targetControlSystem);
    m_targetControlSystem   =   nullptr;

    CC_SAFE_DELETE(m_pathPlanner);
    m_pathPlanner           =   nullptr;

    CC_SAFE_DELETE(m_brain);
    m_brain                 =   nullptr;
}

void GameCharacter::update(float dm)
{
    m_brain->process();

    /**
    * 这里严重注意：在状态机中可能会删除自己，比如调用die的时候 
    */
    m_frameCount++;

    // @_@ 每一帧都调整一下血量
    m_shape->setHpRatio(m_attribute.getHp() / m_attribute.getFullHp());

    /**
    * 更新该角色身上的所有控制系统 
    */
    m_targetControlSystem->tryUpdate();

    // m_stateMachine->update(dm);
}

bool GameCharacter::handleMessage(Telegram& msg)
{
    return m_stateMachine->handleMessage(msg);
}

StateMachine<GameCharacter>* GameCharacter::getFSM()
{
    return m_stateMachine;
}

GameCharacterShape* GameCharacter::getShape()
{
    return m_shape;
}

void GameCharacter::setGridGraph(MapGrid* graph)
{
    m_graph =   graph;
}

ObjectOnMapGrid* GameCharacter::getObjectOnGrid()
{
    return &m_objectOnGrid;
}

void GameCharacter::moveToGridIndex(int nodeIndex, float rate)
{
    // 如果移动目标是无效的格子索引
    if (nodeIndex == INVALID_NODE_INDEX)
    {
        return;
    }

    NavGraphNode& tmpResourceGrid    =   m_graph->getNodeByIndex(m_objectOnGrid.nodeIndex);
    NavGraphNode& tmpTargetGird      =   m_graph->getNodeByIndex(nodeIndex);

    // 调整朝向
    if (m_graph->testIsAtLeft(m_objectOnGrid.nodeIndex, nodeIndex))
    {
        m_shape->faceToRight();
    }
    else if (m_graph->testIsAtRight(m_objectOnGrid.nodeIndex, nodeIndex))
    {
        m_shape->faceToLeft();
    }

    // 先修改之前占领的网格
    m_graph->removeObjectFromGrid(&m_objectOnGrid);
    // 判断目标节点是否可以行走，如果该节点不可走，就不能移动
    if (!tmpTargetGird.passable())
    {
        m_shape->playAction(IDLE_ACTION);
        return;
    }
    // 新的坑位
    m_objectOnGrid.nodeIndex    =   nodeIndex;
    m_graph->addObjectToGrid(&m_objectOnGrid);

    // 播放移动的动画
    m_shape->playAction(RUN_ACTION);

    // 首先结束之前的动作
    if (m_moveAction != nullptr)
    {
        m_shape->stopAction(m_moveAction);
        onMoveOver(nullptr);
    }

    // 设置缓动过去
    auto tmpDirection       =   (tmpTargetGird.getX() - m_shape->getPositionX()) * (tmpTargetGird.getX() - m_shape->getPositionX()) + 
         (tmpTargetGird.getY() - m_shape->getPositionY()) * (tmpTargetGird.getY() - m_shape->getPositionY());
    tmpDirection        =   sqrt(tmpDirection);
    m_moveAction        =   MoveTo::create(tmpDirection / rate, Vec2(tmpTargetGird.getX(), tmpTargetGird.getY()));
    // 增加一个动作结束的回调
    m_shape->runAction(Sequence::create(m_moveAction, CallFuncN::create(std::bind(&GameCharacter::onMoveOver, this, std::placeholders::_1)), nullptr));
}

MapGrid* GameCharacter::getMapGrid()
{
    return m_graph;
}

bool GameCharacter::isMoving()
{
    return m_moveAction != nullptr;
}

void GameCharacter::onMoveOver(Node* pNode)
{
    m_moveAction    =   nullptr;
}

GameCharacterAttribute& GameCharacter::getAttribute()
{
    return m_attribute;
}

void GameCharacter::die()
{
    // 目前就直接删除掉自己吧
    CC_SAFE_RELEASE(this);
}

GameCharacterTypeEnum GameCharacter::getCharacterType()
{
    return m_characterType;
}

void GameCharacter::normalAttack(int id)
{
    m_normatAttTargetId =   id;
    if (this->getCharacterType() == GAMECHARACTER_TYPE_ENUM_SHORT_RANGE)
    {
        // 对于近程攻击单位，只用播放动画就OK了
        this->getShape()->playAction(NORMAL_ATTACK_ACTION, false, std::bind(&GameCharacter::onShortAttEffect, this, std::placeholders::_1));
    }
    else if (this->getCharacterType() == GAMECHARACTER_TYPE_ENUM_LONG_RANGE)
    {
        // 远程攻击单位，还需要在特定动画的位置丢出大便
        this->getShape()->playAction(NORMAL_ATTACK_ACTION, false, std::bind(&GameCharacter::onLongAttLaunch, this, std::placeholders::_1));
    }
}

void GameCharacter::onShortAttEffect(string evt)
{
    // 给对方发出消息
    auto tmpMsg = TelegramNormalAttack::create(this->getId(), m_normatAttTargetId, this->getAttribute());
    Dispatch->dispatchMessage(*tmpMsg);
}

void GameCharacter::onLongAttLaunch(string evt)
{
    // 丢出大便
    // 目标还在
    if (EntityMgr->getEntityFromID(m_normatAttTargetId) != nullptr)
    {
        auto tmpSnowBall    =   OneToOneArmatureFlightProps::create(this->getId(), m_normatAttTargetId, 1);
        tmpSnowBall->setPosition(this->getShape()->getCenterPos());
        this->getShape()->getParent()->addChild(tmpSnowBall);
    }
}

bool GameCharacter::isNormalAttackFinish()
{
    // 目前就用不在动画中来作为结束
    return this->getShape()->isNotInAnimation();
}

bool GameCharacter::isInAttackDistance(GameCharacter* other)
{
    // 近程攻击单位判断是否在攻击范围的方法和远程攻击单位判断方法不同
    if (this->getCharacterType() == GAMECHARACTER_TYPE_ENUM_SHORT_RANGE)
    {
        // 近程攻击单位
        auto tmpOwnerObject     =   this->getObjectOnGrid();
        auto tmpTargetObject    =   other->getObjectOnGrid();

        // 如果在一条水平线上，并且间距在3个格子内
        if (this->getMapGrid()->testTwoIndexInOneHorizon(tmpOwnerObject->nodeIndex, tmpTargetObject->nodeIndex) 
            && abs(tmpTargetObject->nodeIndex - tmpOwnerObject->nodeIndex) <= 3)
        {
            return true;
        }

        // 如果在交叉位置处
        if (tmpOwnerObject->nodeIndex == other->getMapGrid()->getLeftTopGridIndex(tmpTargetObject->nodeIndex)
            || tmpOwnerObject->nodeIndex == other->getMapGrid()->getRightTopGridIndex(tmpTargetObject->nodeIndex)
            || tmpOwnerObject->nodeIndex == other->getMapGrid()->getRightBottomGridIndex(tmpTargetObject->nodeIndex)
            || tmpOwnerObject->nodeIndex == other->getMapGrid()->getLeftBottomGridIndex(tmpTargetObject->nodeIndex))
        {
            return true;
        }
    }
    else if (this->getCharacterType() == GAMECHARACTER_TYPE_ENUM_LONG_RANGE)
    {
        // 远程攻击单位以半径判断
        return (this->getShape()->getPosition() - other->getShape()->getPosition()).getLength() 
            <= other->getAttribute().getAttDistance();
    }

    return false;
}

void GameCharacter::exitNormalAttack()
{
    m_lastExitNormalAttackFrame =   m_frameCount;
}

bool GameCharacter::canNormalAttack()
{
    return (m_frameCount - m_lastExitNormalAttackFrame) > m_attribute.getAttInterval();
}

int GameCharacter::getNextNormatAttackLeftCount()
{
    return m_attribute.getAttInterval() - (m_frameCount - m_lastExitNormalAttackFrame) + 1;
}

vector<GameCharacter*> GameCharacter::getCharactersInView()
{
    vector<GameCharacter*>  pRet;
    
    /**
    *  @_@ 觉得当前在场上的角色的数量肯定是小于探索范围内网格的数量，所以还是遍历所有的
    *  角色吧，比如范围是10，那么可能的网格数量就有5 * 20 = 100，但是一个地图肯定没有这么多人
    */
    auto tmpIterator    =   EntityMgr->getEntityMap()->begin();
    for (; tmpIterator != EntityMgr->getEntityMap()->end(); )
    {
        auto tmpCharacter   =   dynamic_cast<GameCharacter*>(tmpIterator->second);
        tmpIterator++;
        
        // 不包含自己
        if (tmpCharacter == this)
        {
            continue;
        }

        if (m_graph->isInScope(m_objectOnGrid.nodeIndex, tmpCharacter->getObjectOnGrid()->nodeIndex, m_attribute.getViewDistance()))
        {
            pRet.push_back(tmpCharacter);
        }
    }

    /**
    * 这里按照距离来排序，最近的在前面 
    */
    SortFunc tmpSortFunc    =   std::bind(&GameCharacter::charactersInViewSortFunc, 
        this, std::placeholders::_1, std::placeholders::_2);
    sort(pRet.begin(), pRet.end(), tmpSortFunc);

    return pRet;
}

bool GameCharacter::charactersInViewSortFunc( GameCharacter* character1, GameCharacter* character2 )
{
    auto tmpDistance1   =   
        m_graph->getDistanceInGrid(m_objectOnGrid.nodeIndex, character1->getObjectOnGrid()->nodeIndex);
    auto tmpDistance2   =   
        m_graph->getDistanceInGrid(m_objectOnGrid.nodeIndex, character2->getObjectOnGrid()->nodeIndex);
    return tmpDistance1 < tmpDistance2;
}

vector<GameCharacter*> GameCharacter::getFoeCharactersInView()
{
    auto pRet   =   getCharactersInView();
    
    // 剔除与自己同类型的，也就是去掉自己人，剩下的就是敌人
    auto tmpIterator    =   pRet.begin();
    while (tmpIterator != pRet.end())
    {
        if ((*tmpIterator)->getType() == this->getType())
        {
            tmpIterator =   pRet.erase(tmpIterator);
        }
        else
        {
            tmpIterator++;
        }
    }

    return pRet;
}

vector<int> GameCharacter::getFollowGridIndex( GameCharacter* other )
{
    vector<int> pRet;
    int tmpOtherGridIndex   =   other->getObjectOnGrid()->nodeIndex;

    // 如果other是近战型的，就优先靠前
    auto tmpGridIndex   =   m_graph->getRightGridIndex(tmpOtherGridIndex, 2);
    pRet.push_back(m_graph->getTopGridIndex(tmpGridIndex, 2));
    pRet.push_back(m_graph->getBottomGridIndex(tmpGridIndex, 2));
    pRet.push_back(m_graph->getTopGridIndex(tmpGridIndex));
    pRet.push_back(m_graph->getBottomGridIndex(tmpGridIndex));

    // 前斜角
    tmpGridIndex    =   m_graph->getRightGridIndex(tmpOtherGridIndex);
    pRet.push_back(m_graph->getTopGridIndex(tmpGridIndex, 2));
    pRet.push_back(m_graph->getBottomGridIndex(tmpGridIndex, 2));

    // 并排
    pRet.push_back(m_graph->getTopGridIndex(tmpOtherGridIndex, 2));
    pRet.push_back(m_graph->getBottomGridIndex(tmpOtherGridIndex, 2));

    // 后面的
    tmpGridIndex    =   m_graph->getLeftGridIndex(tmpOtherGridIndex, 2);
    pRet.push_back(m_graph->getTopGridIndex(tmpGridIndex, 2));
    pRet.push_back(m_graph->getBottomGridIndex(tmpGridIndex, 2));
    pRet.push_back(m_graph->getTopGridIndex(tmpGridIndex));
    pRet.push_back(m_graph->getBottomGridIndex(tmpGridIndex));

    // 后斜角的
    tmpGridIndex    =   m_graph->getLeftGridIndex(tmpOtherGridIndex);
    pRet.push_back(m_graph->getTopGridIndex(tmpGridIndex, 2));
    pRet.push_back(m_graph->getBottomGridIndex(tmpGridIndex, 2));

    // 剔除掉无效的格子序号
    auto tmpIterator    =   pRet.begin();
    while (tmpIterator != pRet.end())
    {
        if (*tmpIterator == INVALID_NODE_INDEX)
        {
            tmpIterator =   pRet.erase(tmpIterator);
        }
        else
        {
            tmpIterator++;
        }
    }

    // 按照离该角色的距离来排列，距离短的在前面
    FollowGridSortFunc tmpFunc =   std::bind(&GameCharacter::followGridSortFunc, this, std::placeholders::_1, std::placeholders::_2);
    sort(pRet.begin(), pRet.end(), tmpFunc);

    return pRet;
}

bool GameCharacter::followGridSortFunc( int index1, int index2 )
{
    return m_graph->getDistanceInGrid(m_objectOnGrid.nodeIndex, index1) < m_graph->getDistanceInGrid(m_objectOnGrid.nodeIndex, index2);
}

void GameCharacter::sufferNormalAttack( GameCharacterAttribute& attribute )
{
    int tmpHp    =   m_attribute.getHp();
    // 修改属性
    m_attribute.sufferNormalAttack(attribute);
    tmpHp   -=  m_attribute.getHp();

    char tmpHpStr[20];
    sprintf(tmpHpStr, "-%d", tmpHp);

    if (this->getType() == GAME_ENTITY_TYPE_PLAYER_CHARACTER)
    {
        // 如果玩家角色就飘黄字
        m_shape->floatNumber(string(tmpHpStr), GameCharacterShape::FLOAT_NUMBER_YELLOW);
    }
    else
    {
        // 否则飘红字
        m_shape->floatNumber(string(tmpHpStr), GameCharacterShape::FLOAT_NUMBER_RED);
    }

    // @_@ 通知更新
    RefreshUIMsg tmpMsg(REFRESH_UI_EVENT_CHARACTER, this);
    UIViewMgr->refreshView(tmpMsg);
}

std::string GameCharacter::getIconSrc()
{
    string tmpSrc =   "character/icon/";
    switch (m_characterId)
    {
    case 1:
        {
            tmpSrc  +=  "003.png";
            break;
        }

    case 2:
        {
            tmpSrc  +=  "003.png";
            break;
        }

    case 3:
        {
            tmpSrc  +=  "004.png";
            break;
        }

    case 4:
        {
            tmpSrc  +=  "005.png";
            break;
        }

    case 5:
        {
            tmpSrc  +=  "006.png";
            break;
        }

    default:
        break;
    }

    return tmpSrc;
}

PathPlanner* const GameCharacter::getPathPlanner()
{
    return m_pathPlanner;
}
