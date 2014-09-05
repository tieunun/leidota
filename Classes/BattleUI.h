#ifndef __BATTLE_UI_H__
#define __BATTLE_UI_H__

#include "ui/CocosGUI.h"
#include "UIView.h"
#include "GameCharacter.h"
#include "InputManager.h"
#include "JoyStick.h"

using namespace cocos2d;
using namespace ui;

/**
* 战斗时候的UI界面，主要就是一些操作和一些信息的显示，同时它也继承，作为游戏的输入端
*/
class BattleUI : public UIView, public InputManager
{
public:
    bool init() override;

    /**
    * 接受事件的回调函数 
    */
    void onWee(RefreshUIMsg& msg) override;

    void update(float dm) override;

    CREATE_FUNC(BattleUI);

protected:
    BattleUI();

    void setWeeList() override;

    /**
    * 刷新战斗UI中的角色数据的显示 
    */
    void refreshCharacter(GameCharacter* character);

    /**
    * 刷新当前作为主角攻击目标的角色 
    */
    void refreshTargetCharacter(GameCharacter* character);

    /**
    * 以下是接受界面按钮的点击事件 
    */
    void onClickChangeTargetBtn(Ref* target,Widget::TouchEventType type);
    void onClickConvergeBtn(Ref* target,Widget::TouchEventType type);
    void onClickSkillBtn(Ref* target,Widget::TouchEventType type);

    LoadingBar*     m_leaderHpBar;                  // 主角hp条
    ImageView*      m_leaderIcon;                   // 主角的头像

    LoadingBar*     m_enemyHpBar;                   // 当前主角正在打的敌人的hp
    ImageView*      m_enemyIcon;                    // 敌人头像
    Node*           m_enemyPanel;                   // 敌人信息的面板根节点

    Button*         m_convergeBtn;                  // 集中火力的按钮
    Button*         m_changeTargetBtn;              // 更换当前主角的攻击目标
    Button*         m_skillBtn;                     // 技能按钮

    JoyStick*       m_jokStick;                     // 操纵柄
};

#endif