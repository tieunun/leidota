#include "cocos2d.h"
#include "ControlSystem.h"

using namespace cocos2d;

ControlSystem::ControlSystem( GameCharacter* owner, float updatePeriod )
{
    m_owner             =   owner;
    m_lastUpdateTime    =   0;
    m_updatePeriod      =   updatePeriod;
}

ControlSystem::~ControlSystem()
{

}

void ControlSystem::tryUpdate()
{
    // 获取当前时间戳
    struct timeval tv;
    memset(&tv, 0, sizeof(tv));
    gettimeofday(&tv, nullptr);
    float tmpCurrentTime = tv.tv_sec + tv.tv_usec / 1000000.0;
    float tmpInterval   =   tmpCurrentTime - m_lastUpdateTime;
    if (tmpInterval >= m_updatePeriod)
    {
        m_lastUpdateTime    =   tmpCurrentTime;
        update(tmpInterval);
    }
}
