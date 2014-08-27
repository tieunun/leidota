#ifndef __GAME_MAIN_MODEL_H__
#define __GAME_MAIN_MODEL_H__

#include "InputManager.h"
#include "GameCharacter.h"

#define GAMEMAINMODEL_SCHEDULE_KEY  "GAMEMAINMODEL_SCHEDULE_KEY"

// 自己的主逻辑循环间隔
#define GAMEMAINMODEL_LOOP_INTERVAL 0

/**
	 游戏的主模型，该模型的用户输入就是用户的一些操作而已
*/
class GameMainModel : public InputManagerDelegate
{
public:
    GameMainModel();
    ~GameMainModel();
    /**
    	 用来操纵人物移动的操作
    */
    virtual void moveToLeft() override;          // 向左
    virtual void moveToRight() override;         // 向右
    virtual void idle() override;                // 不操作
    virtual void changeTarget() override;        // 选择目标

    /**
    	 设置主控制游戏角色，这个角色会收到玩家控制
    */
    void setMainGameCharacter(GameCharacter* character);

private:
    void update(float dm);                      // 游戏主逻辑的更新     
};

#endif