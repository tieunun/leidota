#ifndef __GAME_CHARACTER_SHAPE_H__
#define __GAME_CHARACTER_SHAPE_H__

#include "cocos2d.h"
#include "cocostudio/CCArmatureDataManager.h"
#include "cocostudio/CCArmature.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace std;

/**
	 定义的一些动作在动画文件中的名称
*/
#define IDLE_ACTION     "idle"
#define RUN_ACTION      "run"
#define ATTACK_ACTION   "atk1"
#define WIN_ACTION      "win"

/**
	 游戏中的所有角色的外形，这些是添加到显示列表中的
*/
class GameCharacterShape : public Sprite
{
public:

    typedef std::function<void (string)> ActionFrameEventCallback;

    /**
    	 关于角色外形，都只用对应一个骨骼动画的配置文件
         @_@ 以后可能会改一下的
    */
    static GameCharacterShape* create(const std::string& fileName, const std::string& armatureName);

    /**
    	 播放动画，需要提供动画名称，向左还是向右，是否循环
    */
    void playAction(const std::string& actionName, bool loop = true, ActionFrameEventCallback eventCallBack = nullptr);

    /**
    	 调整朝向
    */
    void faceToRight();
    void faceToLeft();

    bool init() override;

    /**
    	 判断当前动画是否已经结束
    */
    bool isNotInAnimation();

    /**
    	 返回当前动作的帧位置
    */
    int getCurrentFrameIndex();

    /**
    	 返回用来检测碰撞的矩形
    */
    Rect getCollisionRect();

    /**
    	 返回该角色的中心坐标，作为其他远程单位的工具目标
    */
    Vec2 getCenterPos();

protected:
    GameCharacterShape(const std::string& fileName, const std::string& armatureName);

    /**
    	 当发生骨骼动画帧事件的时候会回调该函数
    */
    void onFrameEvent(Bone *bone, const string& evt, int originFrameIndex, int currentFrameIndex);

    Armature* _armature;
    string _currentAnimationName;                   // 当前播放的动画的名称

    ActionFrameEventCallback   _frameEventCallBack;    // 当播放动画的时候帧事件的回调函数
};

#endif