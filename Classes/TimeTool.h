#ifndef __TIME_TOOL_H__
#define __TIME_TOOL_H__

#include "cocos2d.h"

using namespace cocos2d;

/**
* 主要就是一些时间工具，比如获取当前秒
*/
class TimeTool
{
public:
    /**
    * 返回当前以秒为单位的时间，这里可能会出现的问题是在跨天的时候
    */
    static double getSecondTime()
    {
        struct timeval tv;
        memset(&tv, 0, sizeof(tv));
        gettimeofday(&tv, nullptr);
        double tmpCurrentTime = tv.tv_sec + (double)tv.tv_usec / 1000000.0;
        //CCLOG("tv.tv_sec = %ld, tv.tv_usec = %ld", tv.tv_sec, tv.tv_usec);
        //CCLOG("getSecondTime = %f", tmpCurrentTime);
        return tmpCurrentTime;
    }
};

#endif