#include "GameCharacterState.h"
#include "GraphSearchAStar.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "GameTeam.h"
#include "UIViewManager.h"

// 一个用来判断当前是否在某个状态的宏
#define ISINSTATE(state)    dynamic_cast<state*>(owner->getFSM()->getCurrentState()) != nullptr

// 判断之前的状态
#define ISPREINSTATE(state)     dynamic_cast<state*>(owner->getFSM()->getPreState()) != nullptr

void GameCharacterIdleState::onEnter(GameCharacter* owner)
{
    owner->getShape()->playAction(IDLE_ACTION);
}

void GameCharacterIdleState::update(GameCharacter* owner, float dm)
{
    if (m_reverseStateFrameCount > 0)
    {
        m_reverseStateFrameCount--;
    }
    
    if (m_reverseStateFrameCount == 0)
    {
        owner->getFSM()->reverseState();
    }
}

void GameCharacterIdleState::onExit(GameCharacter* owner)
{
    m_reverseStateFrameCount    =   -1;
}

bool GameCharacterIdleState::onMessage(GameCharacter* owner, Telegram &msg)
{
    return false;
}

void GameCharacterMovingState::onEnter(GameCharacter* owner)
{
    if (movingDirection == MOVING_DIRECTION_LEFT)
    {
        auto tmpLeftGridIndex = owner->getMapGrid()->getLeftGridIndex(owner->getObjectOnGrid()->nodeIndex);
        owner->moveToGridIndex(tmpLeftGridIndex, owner->getAttribute().getRate());
    }
    else if (movingDirection == MOVING_DIRECTION_RIGHT)
    {
        auto tmpRightGridIndex = owner->getMapGrid()->getRightGridIndex(owner->getObjectOnGrid()->nodeIndex);
        owner->moveToGridIndex(tmpRightGridIndex, owner->getAttribute().getRate());
    }
    else if (movingDirection == MOVING_DIRECTION_TOP)
    {
        auto tmpTopGridIndex = owner->getMapGrid()->getTopGridIndex(owner->getObjectOnGrid()->nodeIndex);
        owner->moveToGridIndex(tmpTopGridIndex, owner->getAttribute().getRate());
    }
    else
    {
        auto tmpBottomIndex = owner->getMapGrid()->getBottomGridIndex(owner->getObjectOnGrid()->nodeIndex);
        owner->moveToGridIndex(tmpBottomIndex, owner->getAttribute().getRate());
    }

    // 同时需要告知队伍自己正在移动
    if (owner->getTeam() != nullptr)
    {
        owner->getTeam()->playerMoving(owner);
    }
}

void GameCharacterMovingState::update(GameCharacter* owner, float dm)
{
    // 当移动到目标的时候，切换到之前的状态
    if (!owner->isMoving())
    {
        if (ISPREINSTATE(GameCharacterIdleState))
        {
            owner->getFSM()->changeState(GameCharacterAutoState::create());
        }
        else
        {
            owner->getFSM()->reverseState();
        }
    }
}

void GameCharacterMovingState::onExit(GameCharacter* owner)
{

}

bool GameCharacterMovingState::onMessage(GameCharacter* owner, Telegram &msg)
{
    // 如果当前在移动中，对于任何消息都不接受
    if (msg.type == TELEGRAM_ENUM_NORMAL_ATTACK)
    {
        return false;
    }
    return true;
}

void GameCharacterAutoState::onEnter(GameCharacter* owner)
{
    if (owner->getShape()->isNotInAnimation())
    {
        owner->getShape()->playAction(IDLE_ACTION);
    }
    update(owner, 0);
}

void GameCharacterAutoState::update(GameCharacter* owner, float dm)
{
    vector<GameCharacter*> tmpTargetCharacters = owner->getFoeCharactersInView();   // 范围内的所有敌人
    
    if (tmpTargetCharacters.size() == 0)
    {
        // 可视范围内没有敌人，进入idle状态，在若干帧之后再回来
        auto tmpState = GameCharacterIdleState::create();
        tmpState->setReverseStateFrameCount(20);
        owner->getFSM()->changeState(tmpState);
    }
    else
    {
        // 在可视范围内找到敌人
        // 判断是否有一个敌人进入攻击范围，如果有的话，就进入攻击状态，并攻击该角色
        auto tmpInAttackDistanceCharacter = getCharacterInAttackDistance(owner, tmpTargetCharacters);
        if (tmpInAttackDistanceCharacter != nullptr)
        {
            if (!owner->canNormalAttack())
            {
                // 需要等待若干帧
                auto tmpState   =   GameCharacterIdleState::create();
                tmpState->setReverseStateFrameCount(owner->getNextNormatAttackLeftCount());
                owner->getFSM()->changeState(tmpState);
                return;
            }

            // 切换到攻击状态
            auto tmpState       =   GameCharacterNormalAttack::create();
            tmpState->targetId  =   tmpInAttackDistanceCharacter->getId();
            owner->getFSM()->changeState(tmpState);
            return;
        }

        // 寻找过去的路径，只要找到第一条路线
        vector<int> tmpTargetRoute;
        for (int i = 0; i < tmpTargetCharacters.size(); i++)
        {
            tmpTargetRoute = owner->getMapGrid()->getRouteToTargetObj(owner->getObjectOnGrid(), tmpTargetCharacters[i]->getObjectOnGrid());
            if (tmpTargetRoute.size() > 0)
            {
                break;
            }
        }

        // 根据这个路径选择下一步该如何移动
        if (tmpTargetRoute.size() > 0)
        {
            auto tmpTargetGridIndex =   tmpTargetRoute[1];
            // 切换到移动状态，这里寻找方向
            auto tmpState = GameCharacterMovingState::create();
            if (tmpTargetGridIndex == owner->getMapGrid()->getLeftGridIndex(owner->getObjectOnGrid()->nodeIndex))
            {
                tmpState->movingDirection   =   GameCharacterMovingState::MOVING_DIRECTION_LEFT;
            }
            else if (tmpTargetGridIndex == owner->getMapGrid()->getTopGridIndex(owner->getObjectOnGrid()->nodeIndex))
            {
                tmpState->movingDirection   =   GameCharacterMovingState::MOVING_DIRECTION_TOP;
            }
            else if (tmpTargetGridIndex == owner->getMapGrid()->getRightGridIndex(owner->getObjectOnGrid()->nodeIndex))
            {
                tmpState->movingDirection   =   GameCharacterMovingState::MOVING_DIRECTION_RIGHT;
            }
            else
            {
                tmpState->movingDirection   =   GameCharacterMovingState::MOVING_DIRECTION_BOTTOM;
            }
            owner->getFSM()->changeState(tmpState);
        }
        else
        {
            // 进入Idle状态一下
            auto tmpState   =   GameCharacterIdleState::create();
            tmpState->setReverseStateFrameCount(20);
            owner->getFSM()->changeState(tmpState);
        }
    }
}

void GameCharacterAutoState::onExit(GameCharacter* owner)
{

}

bool GameCharacterAutoState::onMessage(GameCharacter* owner, Telegram &msg)
{
    return false;
}

GameCharacter* GameCharacterAutoState::getCharacterInAttackDistance(GameCharacter* owner, vector<GameCharacter*>& targets)
{
    for (int i = 0; i < targets.size(); i++)
    {
        if (isInAttackDistance(owner, targets[i]))
        {
            return targets[i];
        }
    }

    return nullptr;
}

bool GameCharacterAutoState::isInAttackDistance(GameCharacter* owner, GameCharacter* target)
{
    return owner->isInAttackDistance(target);
}

void GameCharacterNormalAttack::onEnter(GameCharacter* owner)
{
    // 调整方向
    GameCharacter* target   =   (GameCharacter*)EntityMgr->getEntityFromID(targetId);
    if (target == nullptr)
    {
        owner->getFSM()->changeState(GameCharacterAutoState::create());
        return;
    }

    if (owner->getMapGrid()->testIsAtLeft(owner->getObjectOnGrid()->nodeIndex, target->getObjectOnGrid()->nodeIndex))
    {
        owner->getShape()->faceToRight();
    }
    else
    {
        owner->getShape()->faceToLeft();
    }

    // 如果当前的进攻者是主角，就在被攻击目标上有选择光环
    if (EntityMgr->getmainEntity() == owner)
    {
        target->getShape()->showHalo(GameCharacterShape::HALO_RED);
        RefreshUIMsg tmpMsg(REFRESH_UI_EVENT_ATTACK_CHARACTER, target);
        UIViewMgr->refreshView(tmpMsg);
    }

    owner->normalAttack(targetId);
}

void GameCharacterNormalAttack::update(GameCharacter* owner, float dm)
{
    if (owner->isNormalAttackFinish())
    {
        if (ISPREINSTATE(GameCharacterMovingState))
        {
            owner->getFSM()->changeState(GameCharacterAutoState::create());
        }
        else
        {
            owner->getFSM()->reverseState();
        }
    }
}

void GameCharacterNormalAttack::onExit(GameCharacter* owner)
{
    owner->exitNormalAttack();

    // 如果对方角色还在，在离开战斗时，清除掉选择光环
    auto tmpTarget  =   dynamic_cast<GameCharacter*>(EntityMgr->getEntityFromID(targetId));
    if (tmpTarget != nullptr)
    {
        tmpTarget->getShape()->hideHalo();
    }
    RefreshUIMsg tmpMsg(REFRESH_UI_EVENT_ATTACK_CHARACTER, nullptr);
    UIViewMgr->refreshView(tmpMsg);
}

bool GameCharacterNormalAttack::onMessage(GameCharacter* owner, Telegram &msg)
{
    return false;
}

void GameCharacterGlobalState::onEnter(GameCharacter* owner)
{

}

void GameCharacterGlobalState::update(GameCharacter* owner, float dm)
{
    
}

void GameCharacterGlobalState::onExit(GameCharacter* owner)
{

}

bool GameCharacterGlobalState::onMessage(GameCharacter* owner, Telegram &msg)
{
    switch (msg.type)
    {
    case TELEGRAM_ENUM_USER_MOVE_LEFT:
        {
            // 判断是否可以向左移动
            auto tmpLeftGridIndex   =   owner->getMapGrid()->getLeftGridIndex(owner->getObjectOnGrid()->nodeIndex);
            if (tmpLeftGridIndex != INVALID_NODE_INDEX)
            {
                // 先移除自己所占有的区域，然后查询是否可以过去，如果可以，就移动过去
                owner->getMapGrid()->removeObjectFromGrid(owner->getObjectOnGrid());
                auto tmpRoute = GraphSearchAStar::search(*owner->getMapGrid(), owner->getObjectOnGrid()->nodeIndex, tmpLeftGridIndex, MapGrid::calculateH);
                if (tmpRoute.size() > 0)
                {
                    // 如果找到了路，才切换到向左移动状态
                    auto tmpState               =   GameCharacterMovingState::create();
                    tmpState->movingDirection   =   GameCharacterMovingState::MOVING_DIRECTION_LEFT;
                    owner->getFSM()->changeState(tmpState);
                }
                else
                {
                    owner->getMapGrid()->addObjectToGrid(owner->getObjectOnGrid());
                }
            }

            return true;
        }

    case TELEGRAM_ENUM_USER_MOVE_RIGHT:
        {
            // 判断是否可以向右移动
            auto tmpLeftGridIndex   =   owner->getMapGrid()->getRightGridIndex(owner->getObjectOnGrid()->nodeIndex);
            if (tmpLeftGridIndex != INVALID_NODE_INDEX)
            {
                // 先移除自己所占有的区域，然后查询是否可以过去，如果可以，就移动过去
                owner->getMapGrid()->removeObjectFromGrid(owner->getObjectOnGrid());
                auto tmpRoute = GraphSearchAStar::search(*owner->getMapGrid(), owner->getObjectOnGrid()->nodeIndex, tmpLeftGridIndex, MapGrid::calculateH);
                if (tmpRoute.size() > 0)
                {
                    auto tmpState               =   GameCharacterMovingState::create();
                    tmpState->movingDirection   =   GameCharacterMovingState::MOVING_DIRECTION_RIGHT;
                    owner->getFSM()->changeState(tmpState);
                }
                else
                {
                    owner->getMapGrid()->addObjectToGrid(owner->getObjectOnGrid());
                }
            }

            return true;
        }

    case TELEGRAM_ENUM_NORMAL_ATTACK:
        {
            // 受到普通攻击，计算一下伤害
            auto tmpAttMsg  =   (TelegramNormalAttack*)&msg;
            owner->sufferNormalAttack(tmpAttMsg->senderAtt);
            if (owner->getAttribute().getHp() <= 0)
            {
                // 该人物死亡
                owner->getFSM()->changeState(GameCharacterDieState::create());
            }

            return true;
        }

    case TELEGRAM_ENUM_USER_CHANGE_TARGET:
        {
            // 切换攻击目标
            auto tmpState   =   GameCharacterPursueState::create();
            // 找一个敌人
            auto tmpTaget   =   EntityMgr->getOneEntity(GAME_ENTITY_TYPE_ENEMY_CHARACTER);
            if (tmpTaget != nullptr)
            {
                tmpState->targetId  =   tmpTaget->getId();
                owner->getFSM()->changeState(tmpState); 
            }
            else
            {
                owner->getFSM()->changeState(GameCharacterAutoState::create());
            }
            
            return true;
        }

    case TELEGRAM_ENUM_TEAM_IDLE:                       // 队伍通知手下暂停行动
        {
            owner->getFSM()->changeState(GameCharacterIdleState::create());
            return true;
        }

    case TELEGRAM_ENUM_TEAM_FREE_COMBAT:                // 队伍通知手下自由战斗
        {
            owner->getFSM()->changeState(GameCharacterAutoState::create());
            return true;
        }

    case TELEGRAM_ENUM_TEAM_CELEBRATE:                  // 队伍通知手下庆祝
        {
            owner->getFSM()->changeState(GameCharacterWinState::create());
            return true;
        }

    case TELEGRAM_ENUM_TEAM_FOLLOW_SPECIFIED_PLAYER:    // 队伍通知首先去跟随某个角色
        {
            auto tmpTarget  =   (GameCharacter*)msg.extraInfo;
            if (owner != tmpTarget)
            {
                auto tmpState   =   GameCharacterFollowOne::create();
                tmpState->followTargetId    =   tmpTarget->getId();
                owner->getFSM()->changeState(tmpState);
            }
            return true;
        }
        
    default:
        break;
    }

    return false;
}

void GameCharacterPursueState::onEnter(GameCharacter* owner)
{

}

void GameCharacterPursueState::update(GameCharacter* owner, float dm)
{
    // 处于追击状态的时候，先寻找要追击的目标，如果目标不在了，就切换到Auto
    auto tmpTargetCharacter = (GameCharacter*)EntityMgr->getEntityFromID(targetId);
    if (tmpTargetCharacter == nullptr)
    {
        owner->getFSM()->changeState(GameCharacterAutoState::create());
        return;
    }

    // 如果在攻击范围内，就直接攻击
    if (owner->isInAttackDistance(tmpTargetCharacter))
    {
        // 查看现在是否可以攻击
        if (!owner->canNormalAttack())
        {
            // 需要等待若干帧
            auto tmpState   =   GameCharacterIdleState::create();
            tmpState->setReverseStateFrameCount(owner->getNextNormatAttackLeftCount());
            owner->getFSM()->changeState(tmpState);
            return;
        }

        // 切换到攻击状态
        auto tmpState       =   GameCharacterNormalAttack::create();
        tmpState->targetId  =   targetId;
        owner->getFSM()->changeState(tmpState);
        return;
    }

    // 寻找过去的路
    auto tmpRoute = owner->getMapGrid()->getRouteToTargetObj(owner->getObjectOnGrid(), 
        tmpTargetCharacter->getObjectOnGrid());
    if (tmpRoute.size() == 0)
    {
        // 如果找不到路，还进入Auto
        owner->getFSM()->changeState(GameCharacterAutoState::create());
    }
    else
    {
        // 找到了前进的路，就移动过去
        auto tmpTargetGridIndex =   tmpRoute[1];
        auto tmpMagGrid         =   owner->getMapGrid();
        auto tmpOwnerGridIndex  =   owner->getObjectOnGrid()->nodeIndex;
        auto tmpState           =   GameCharacterMovingState::create();   
        if (tmpMagGrid->testIsAtTop(tmpTargetGridIndex, tmpOwnerGridIndex))
        {
            tmpState->movingDirection   =   GameCharacterMovingState::MOVING_DIRECTION_TOP;
        }
        else if (tmpMagGrid->testIsAtRight(tmpTargetGridIndex, tmpOwnerGridIndex))
        {
            tmpState->movingDirection   =   GameCharacterMovingState::MOVING_DIRECTION_RIGHT;
        }
        else if (tmpMagGrid->testIsAtBottom(tmpTargetGridIndex, tmpOwnerGridIndex))
        {
            tmpState->movingDirection   =   GameCharacterMovingState::MOVING_DIRECTION_BOTTOM;
        }
        else
        {
            tmpState->movingDirection   =   GameCharacterMovingState::MOVING_DIRECTION_LEFT;
        }
        owner->getFSM()->changeState(tmpState);
    }
}

void GameCharacterPursueState::onExit(GameCharacter* owner)
{

}

bool GameCharacterPursueState::onMessage(GameCharacter* owner, Telegram &msg)
{
    return false;
}

void GameCharacterWinState::onEnter(GameCharacter* owner)
{
    owner->getShape()->playAction(WIN_ACTION);
}

void GameCharacterWinState::update(GameCharacter* owner, float dm)
{

}

void GameCharacterWinState::onExit(GameCharacter* owner)
{

}

bool GameCharacterWinState::onMessage(GameCharacter* owner, Telegram &msg)
{
    // 对于在胜利状态的时候对于用户操作的消息不作处理
    return msg.type >= TELEGRAM_ENUM_USER_MOVE_RIGHT && msg.type <= TELEGRAM_ENUM_USER_CHANGE_TARGET;
}

void GameCharacterDieState::onEnter(GameCharacter* owner)
{
    owner->getShape()->stopAllActions();
}

void GameCharacterDieState::update(GameCharacter* owner, float dm)
{
    owner->die();
}

void GameCharacterDieState::onExit(GameCharacter* owner)
{

}

bool GameCharacterDieState::onMessage(GameCharacter* owner, Telegram &msg)
{
    return true;
}

void GameCharacterFollowOne::onEnter( GameCharacter* owner )
{
    auto tmpMap         =   owner->getMapGrid();
    auto tmpSourceIndex =   owner->getObjectOnGrid()->nodeIndex;

    // 查找一个到达跟随位置的路径
    auto tmpTargetCharacter =   dynamic_cast<GameCharacter*>(EntityMgr->getEntityFromID(followTargetId));
    if (tmpTargetCharacter == nullptr)
    {
        // 说明跟随目标没有了，进入Auto
        owner->getFSM()->changeState(GameCharacterAutoState::create());
        return;
    }

    // 获取跟随位置
    auto tmpTargetGrids =   owner->getFollowGridIndex(tmpTargetCharacter);
    if (tmpTargetGrids.size() == 0)
    {
        // 如果没有跟随位置了
        owner->getFSM()->changeState(GameCharacterAutoState::create());
        return;
    }
    
    // 如果现在就在跟随位置中，就不用动
    for (int i = 0; i < tmpTargetGrids.size(); i++)
    {
        if (tmpTargetGrids[i] == tmpSourceIndex)
        {
            owner->getFSM()->changeState(GameCharacterAutoState::create());
            return;
        }
    }

    // 获取过去的路径
    vector<int> tmpRoute;
    for (int i = 0; i < tmpTargetGrids.size(); i++)
    {
        // @_@ 必须先把自己占得坑位移除
        tmpMap->removeObjectFromGrid(owner->getObjectOnGrid());
        tmpRoute    =   GraphSearchAStar::search(*tmpMap, tmpSourceIndex, tmpTargetGrids[i], MapGrid::calculateH);
        tmpMap->addObjectToGrid(owner->getObjectOnGrid());
        if (tmpRoute.size() != 0)
        {
            break;
        }
    }

    // 尝试移动过去
    auto tmpState   =   GameCharacterMovingState::create();
    if (tmpRoute[1] == tmpMap->getLeftGridIndex(tmpSourceIndex))
    {
        tmpState->movingDirection   =   GameCharacterMovingState::MOVING_DIRECTION_LEFT;
    }
    else if (tmpRoute[1] == tmpMap->getTopGridIndex(tmpSourceIndex))
    {
        tmpState->movingDirection   =   GameCharacterMovingState::MOVING_DIRECTION_TOP;
    }
    else if (tmpRoute[1] == tmpMap->getRightGridIndex(tmpSourceIndex))
    {
        tmpState->movingDirection   =   GameCharacterMovingState::MOVING_DIRECTION_RIGHT;
    }
    else
    {
        tmpState->movingDirection   =   GameCharacterMovingState::MOVING_DIRECTION_BOTTOM;
    }
    owner->getFSM()->changeState(tmpState);
}