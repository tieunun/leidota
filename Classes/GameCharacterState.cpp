#include "GameCharacterState.h"
#include "GraphSearchAStar.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"

void GameCharacterIdleState::onEnter(GameCharacter* owner)
{
    _frameCount =   0;
    owner->getShape()->playAction(IDLE_ACTION);
}

void GameCharacterIdleState::update(GameCharacter* owner, float dm)
{
    _frameCount++;
    if (_frameCount >= enterToAutoFrame)
    {
        owner->getFSM()->changeState(GameCharacterAutoState::create());
    }
}

void GameCharacterIdleState::onExit(GameCharacter* owner)
{
    _frameCount =   0;
}

bool GameCharacterIdleState::onMessage(GameCharacter* owner, Telegram &msg)
{
    return false;
}

void GameCharacterMovingState::onEnter(GameCharacter* owner)
{
    if (movingDirection == MOVING_DIRECTION_LEFT)
    {
        owner->getShape()->faceToLeft();
        auto tmpLeftGridIndex = owner->getMapGrid()->getLeftGridIndex(owner->getObjectOnGrid()->nodeIndex);
        owner->moveToGridIndex(tmpLeftGridIndex, owner->getAttribute().getRate());
    }
    else if (movingDirection == MOVING_DIRECTION_RIGHT)
    {
        owner->getShape()->faceToRight();
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

    owner->getShape()->playAction(RUN_ACTION);
}

void GameCharacterMovingState::update(GameCharacter* owner, float dm)
{
    // 当移动到目标的时候，切换到之前的状态
    if (!owner->isMoving())
    {
        owner->getFSM()->reverseState();
    }
}

void GameCharacterMovingState::onExit(GameCharacter* owner)
{

}

bool GameCharacterMovingState::onMessage(GameCharacter* owner, Telegram &msg)
{
    // 如果当前在移动中，对于任何消息都不接受
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
    // 寻找敌人，确定下一个网格的位置，然后切换到移动状态过去
    // 遍历所有的场上的实体，找到敌人，确定哪个所需路径最近
    vector<GameCharacter*>  tmpTargetCharacters;
    // 暂时EntityMgr就是用来放GameCharacter的
    auto tmpAllCharacters = EntityMgr->getEntityMap();
    for (auto tmpIterator = tmpAllCharacters->begin(); tmpIterator != tmpAllCharacters->end(); tmpIterator++)
    {
        if (owner->getType() == GAME_ENTITY_TYPE_PLAYER_CHARACTER && tmpIterator->second->getType() == GAME_ENTITY_TYPE_ENEMY_CHARACTER)
        {
            // 如果该角色是玩家，并且目前是敌人
            tmpTargetCharacters.push_back((GameCharacter*)tmpIterator->second);
        }
        if (owner->getType() == GAME_ENTITY_TYPE_ENEMY_CHARACTER && tmpIterator->second->getType() == GAME_ENTITY_TYPE_PLAYER_CHARACTER)
        {
            tmpTargetCharacters.push_back((GameCharacter*)tmpIterator->second);
        }
    }

    // 判断是否有一个敌人进入攻击范围，如果有的话，就进入攻击状态，并攻击该角色
    auto tmpInAttackDistanceCharacter = getCharacterInAttackDistance(owner, tmpTargetCharacters);
    if (tmpInAttackDistanceCharacter != nullptr)
    {
        // 切换到攻击状态
        auto tmpState       =   GameCharacterNormalAttack::create();
        tmpState->targetId  =   tmpInAttackDistanceCharacter->getId();
        owner->getFSM()->changeState(tmpState);
        return;
    }

    // 寻找过去的路径
    vector<int> tmpTargetRoute;
    for (int i = 0; i < tmpTargetCharacters.size(); i++)
    {
        auto tmpRoute = owner->getMapGrid()->getRouteToTargetObj(owner->getObjectOnGrid(), tmpTargetCharacters[i]->getObjectOnGrid());
        if ((tmpRoute.size() > 0) && (tmpTargetRoute.size() == 0 || tmpRoute.size() < tmpTargetRoute.size()))
        {
            tmpTargetRoute  =   tmpRoute;
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
    else if (tmpTargetCharacters.size() > 0)
    {
        // 进入Idle状态一下
        owner->getFSM()->changeState(GameCharacterIdleState::create());
    }
    else
    {
        // 发现如果敌人都被消灭了，就进入欢呼胜利状态
        // 已经结束
        owner->getFSM()->changeState(GameCharacterWinState::create());
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

    // owner->getShape()->playAction(ATTACK_ACTION, false);
    owner->normalAttack(targetId);
}

void GameCharacterNormalAttack::update(GameCharacter* owner, float dm)
{
    if (owner->isNormalAttackFinish())
    {
        // 如果攻击结束，就切换到Auto状态
        //owner->getFSM()->changeState(GameCharacterAutoState::create());
        owner->getFSM()->reverseState();
    }
}

void GameCharacterNormalAttack::onExit(GameCharacter* owner)
{

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
            if (owner->getAttribute().sufferNormalAttack(tmpAttMsg->senderAtt).getHp() <= 0)
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
            tmpState->targetId  =   tmpTaget->getId();
            owner->getFSM()->changeState(tmpState);
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
    return true;
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