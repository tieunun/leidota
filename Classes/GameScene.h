#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "GameMap.h"
#include "PCInputManager.h"
#include "MobileInputManager.h"
#include "GameMainModel.h"

using namespace cocos2d;

/**
	 游戏主场景
*/
class GameScene : public Scene
{
private:
    GameScene():m_celebrateFrame(90)
    {
        
    }

public:
    bool init() override;

    CREATE_FUNC(GameScene);

protected:
    GameMap*    m_map;                  // 地图

    InputManager* m_inputManager;     // 用来监听输入的
    GameMainModel*  m_mainModel;        // 游戏主逻辑部分

    const int m_celebrateFrame;         // 保持庆祝多少帧
    int m_celebrateFrameCount;          // 计数用的
};

#endif