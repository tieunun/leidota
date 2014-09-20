#ifndef __GAME_CHARACTER_SHAPE_H__
#define __GAME_CHARACTER_SHAPE_H__

#include "cocos2d.h"
#include "cocostudio/CCArmatureDataManager.h"
#include "cocostudio/CCArmature.h"
#include "ui/UILoadingBar.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;
using namespace std;

/**
	 定义的一些动作在动画文件中的名称
*/
#define IDLE_ACTION             "idle"
#define RUN_ACTION              "run"
#define NORMAL_ATTACK_ACTION    "atk1"
#define WIN_ACTION              "win"

/**
	 游戏中的所有角色的外形，这些是添加到显示列表中的
*/
class GameCharacterShape : public Sprite
{
public:

    typedef std::function<void (string)> ActionFrameEventCallback;

    /**
    * 从人物身上浮动的文字的样式的枚举 
    */
    enum FloatNumberTypeEnum
    {
        FLOAT_NUMBER_GREEN,                     // 绿色
        FLOAT_NUMBER_RED,                       // 红色
        FLOAT_NUMBER_YELLOW                     // 黄色
    };

    /**
    * 被选中的时候显示的光圈的类型 
    */
    enum HaloTypeEnum
    {
        HALO_GREEN,                             // 绿色光圈
        HALO_RED                                // 红色光圈
    };

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

    /**
    * 当想让该角色身上出现数字飘动的时候调用的接口 
    */
    void floatNumber(string numStr, FloatNumberTypeEnum type);

    /**
    *  在人物周围出现一个被选中的光圈
    */
    void showHalo(HaloTypeEnum type);

    /**
    * 如果当前人物身边有光圈，就删除掉 
    */
    void hideHalo();

    /**
    *	设置HP显示的比率，就是还剩百分之几的hp
    *   0~1
    */
    void setHpRatio(float ratio);

    /**
    * 返回当前播放的动画 
    */
    string getCurrentAnimationName();

    /**
    * 临时用的一个东西，就是从外面设置一个数字，可以在角色上显示 
    */
    void setPosNumber(int posNum) 
    {
        char tmpStr[10];
        sprintf(tmpStr, "%d", posNum);
        m_posNumLabel->setString(tmpStr);
    }

protected:
    GameCharacterShape(const std::string& fileName, const std::string& armatureName);

    /**
    	 当发生骨骼动画帧事件的时候会回调该函数
    */
    void onFrameEvent(Bone *bone, const string& evt, int originFrameIndex, int currentFrameIndex);

    /**
    *  当浮动的数字停止动作的时候的回调，这里只用把自己删除就OK了
    */
    void onFloatNumberMoveOver(Node* pNode);

    Armature* _armature;
    string _currentAnimationName;                   // 当前播放的动画的名称

    ActionFrameEventCallback   _frameEventCallBack;    // 当播放动画的时候帧事件的回调函数

    LoadingBar* m_hpBar;                            // 每个角色头上的血条

    /**
    * 一些常量@_@比如关于动画时间之类的 
    */
    const float FLOATNUMBERDIRATION;                // 浮动文字动画时间
    const float FLOATNUMBERMOVEBYY;                 // 移动距离

    /**
    * 临时值 
    */
    Armature*   m_halo;                             // 人物脚部旋转地光圈
    Label*      m_posNumLabel;                      // 数字标签
};

#endif