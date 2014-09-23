#include "GoalCharacterThink.h"
#include "GoalAttackSpecifiedTarget.h"

GoalCharacterThink::GoalCharacterThink( GameCharacter* owner ):GoalComposite<GameCharacter>(owner)
{

}

GoalCharacterThink::~GoalCharacterThink()
{

}

GoalStateEnum GoalCharacterThink::process()
{
    return GoalComposite<GameCharacter>::process();
}

void GoalCharacterThink::activate()
{

}

/**
* 我发现队员的大脑已经变成了接受队伍消息的地方了，然后根据队伍消息来调整自己的目标 
*/
bool GoalCharacterThink::handleMessage( Telegram& msg )
{
    // 首先交给子目标处理
    if (forwardMessageToFrontMostSubgoal(msg)) return true;

    switch (msg.type)
    {
    case TELEGRAM_ENUM_TEAM_COLLECTIVE_FORWARD:                 // 队伍通知手下跟随队伍前进
        {
            m_pOwner->getSteeringBehaviros()->keepFormationOn();
            return true;
        }

    case TELEGRAM_ENUM_TEAM_CANCEL_COLLECTIVE_FORWARD:          // 队伍通知手下不用跟随队伍了
        {
            m_pOwner->getSteeringBehaviros()->keepFormationOff();
            return true;
        }

    case TELEGRAM_ENUM_TEAM_ATTACK_SPECIFIED_TARGET:            // 队伍命令手下攻击指定目标
        {
            // 删除所有的当前任务，然后设定新的任务
            removeAllSubgoals();
            int tmpTargetId =   (int)msg.extraInfo;
            addSubgoal(new GoalAttackSpecifiedTarget(m_pOwner, tmpTargetId));
            return true;
        }

    default:
        break;
    }

    return false;
}
