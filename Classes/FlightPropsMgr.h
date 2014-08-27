#ifndef __FLIGHT_PROPS_MGR_H__
#define __FLIGHT_PROPS_MGR_H__

#include "cocos2d.h"
#include "FlightProps.h"

using namespace std;

/**
	 飞行道具管理类，负责管理当前正在飞行中的所有道具
*/
class FlightPropsMgr
{
public:

    typedef map<int, FlightProps*>  FlightPropsMap;

    static FlightPropsMgr* instance();

    /**
    	 注册
    */
    void registerFlightProps(FlightProps* flight);

    /**
    	 删除
    */
    void removeFlightProps(FlightProps* flight);

    /**
    	 每一帧的调用，来修改飞行道具的位置，以及做一些其他的逻辑
    */
    void update(float dm);

private:
    FlightPropsMgr(){}

    /**
    	 测试某一个
    */
    bool collisionTest(int id, FlightProps* flight);

    static FlightPropsMgr*      m_instance;                 // 单件

    FlightPropsMap              m_allFlights;               // 所有正在飞行中的
};

#define FliProMgr   FlightPropsMgr::instance()

#endif