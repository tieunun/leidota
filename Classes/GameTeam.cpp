#include "GameTeam.h"
#include "MessageDispatcher.h"
#include "EntityManager.h"
#include "TeamManager.h"
#include "GameCharacter.h"
#include "GoalTeamThink.h"

int GameTeam::m_nextValidId =   0;

GameTeam::GameTeam()
{
    m_teamId        =   m_nextValidId++;
    m_advanceRate   =   0;
    m_teamBrain     =   new GoalTeamThink(this);
    m_teamState     =   GAME_TEAM_STATE_ACTIVE;
    setStayState();
}

GameTeam::~GameTeam()
{
    // 析构队伍的同时也析构所有的成员，因为成员属于队伍
    for (auto tmpIterator = m_members.begin(); tmpIterator != m_members.end(); tmpIterator++)
    {
        CC_SAFE_RELEASE_NULL(*tmpIterator);
    }
    m_members.clear();

    CC_SAFE_DELETE(m_teamBrain);
    m_teamBrain =   nullptr;
}

void GameTeam::addMember(GameCharacter* player, int posId)
{
    player->retain();
    m_members.push_back(player);
    player->setTeam(this);
    player->getMovingEntity().setFormationPosId(posId);
}

void GameTeam::update(float dm)
{
    // 首先删除当前处于死亡状态的成员
    removeDeadCharacter();

    // 队伍级别的大脑思考一下
    m_teamBrain->process();

    // 调用该队的所有成员的update
    for (auto tmpIterator = m_members.begin(); tmpIterator != m_members.end(); tmpIterator++)
    {
        (*tmpIterator)->update(dm);
    }

    // 再删除处于死亡状态的队员
    removeDeadCharacter();

    // 更新队伍位置，如果队伍速度大于0，就表示是集体移动
    auto tmpTeamPos =   m_formation.getFormationAnchor();
    if (iscollectiveForwardState())
    {
        tmpTeamPos.setPoint(tmpTeamPos.x + dm * m_advanceRate, tmpTeamPos.y);
        m_formation.setFormationAnchor(tmpTeamPos);
    }
    else if (isFollowPlayerState())
    {
        // 否则队伍就始终跟随最前面的人
        auto tmpFormationX    =   0;
        for (auto tmpIterator = m_members.begin(); tmpIterator != m_members.end(); tmpIterator++)
        {
            auto tmpXPos    =   (*tmpIterator)->getMovingEntity().getPosition().x;
            if (m_formation.getFormationType() == Formation::FORMATION_TYPE_RIGHT && 
                tmpFormationX < tmpXPos)
            {
                tmpFormationX   =   tmpXPos;
            }
            if (m_formation.getFormationType() == Formation::FORMATION_TYPE_LEFT &&
                tmpFormationX > tmpXPos)
            {
                tmpFormationX   =   tmpXPos;
            }
        }
        tmpTeamPos.setPoint(tmpFormationX, tmpTeamPos.y);
        m_formation.setFormationAnchor(tmpTeamPos);
    }

    // 判断是否可以被删除
    if (m_members.size() == 0)
    {
        setCanRemove();
    }
}

GameTeam* GameTeam::create(GameTeamTypeEnum teamType)
{
    auto pRet   =   new GameTeam();
    if (pRet != nullptr)
    {
        pRet->autorelease();
        pRet->m_teamType    =   teamType;
        TeamMgr->registerTeam(pRet);
    }

    return pRet;
}

void GameTeam::removeDeadCharacter()
{
    for (auto tmpIterator = m_members.begin(); tmpIterator != m_members.end();)
    {
        if ((*tmpIterator)->isDead())
        {
            CC_SAFE_RELEASE_NULL(*tmpIterator);
            tmpIterator = m_members.erase(tmpIterator);
        }
        else
        {
            tmpIterator++;
        }
    }
}

void GameTeam::sendMessageToAllMember( Telegram& msg )
{
    for (auto tmpIterator = m_members.begin(); tmpIterator != m_members.end(); tmpIterator++)
    {
        sendMessageToOneMember(msg, *tmpIterator);
    }
}

void GameTeam::sendMessageToOneMember( Telegram& msg, GameCharacter* aCharacter )
{
    msg.receiverId  =   aCharacter->getId();
    Dispatch->dispatchMessage(msg);
}

void GameTeam::collectiveForwardStart()
{
    /**
    *  @_@ 虽然我很想直接调用队伍成员的接口，但是为了结构型，是不允许队伍直接操作队员
    *  必须走消息
    */ 
    auto tmpMsg =   Telegram::create(0, 0, TELEGRAM_ENUM_TEAM_COLLECTIVE_FORWARD);
    sendMessageToAllMember(*tmpMsg);

    // 同时让锚点在每一帧中向前移动，@_@先这样写，通过速度来控制阵型移动
    m_advanceRate   =   40;

    setCollectiveForwardState();
}

void GameTeam::collectiveForwardEnd()
{
    auto tmpMsg =   Telegram::create(0, 0, TELEGRAM_ENUM_TEAM_CANCEL_COLLECTIVE_FORWARD);
    sendMessageToAllMember(*tmpMsg);

    m_advanceRate   =   0;

    setFollowPlayerState();
}

bool GameTeam::isEveryMemberInPos()
{
    auto tmpIterator = m_members.begin();
    for (; tmpIterator != m_members.end(); tmpIterator++)
    {
        auto tmpMovingEntity    =   (*tmpIterator)->getMovingEntity();
        auto tmpPos1            =   tmpMovingEntity.getPosition();
        auto tmpPos2            =   m_formation.getPositionByPosId(tmpMovingEntity.getFormationPosId());
        if ((tmpPos1 - tmpPos2).getLengthSq() > 900 || tmpMovingEntity.getSpeed() != 0)
        {
            return false;
        }
    }

    return true;
}

GameCharacter* GameTeam::getMemberIdFromFormation( int posId )
{
    for (auto tmpIterator = m_members.begin(); tmpIterator != m_members.end(); tmpIterator++)
    {
        if ((*tmpIterator)->getMovingEntity().getFormationPosId() == posId)
        {
            return *tmpIterator;
        }
    }

    return nullptr;
}
