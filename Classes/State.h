#ifndef __STATE_H__
#define __STATE_H__

#include "cocos2d.h"
#include "Telegram.h"

/**
	 实体状态
*/
template <class entity_type>
class State : public cocos2d::Ref
{
public:
    virtual ~State(){};

    /**
    	 刚进入该状态的时候
    */
    virtual void onEnter(entity_type *owner)            =   0;

    /**
    	 每一帧的更新
    */
    virtual void update(entity_type *owner, float dm)   =   0;
    
    /**
    	 离开该状态的时候的回调
    */
    virtual void onExit(entity_type *owner)             =   0;
    
    /**
    	 用来处理消息的
    */
    virtual bool onMessage(entity_type *owner, Telegram& msg)               =   0;
    
    virtual bool init(){return true;}
};

#endif