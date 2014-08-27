#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "GameMap.h"
#include "PCInputManager.h"
#include "GameMainModel.h"

using namespace cocos2d;

/**
	 游戏主场景
*/
class GameScene : public Scene
{
public:
    bool init() override;

    CREATE_FUNC(GameScene);

protected:
    GameMap*    m_map;                  // 地图

    PCInputManager* m_inputManager;     // 用来监听输入的
    GameMainModel*  m_mainModel;        // 游戏主逻辑部分
};

#endif