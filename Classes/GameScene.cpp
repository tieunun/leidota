#include "GameScene.h"
#include "GameCharacter.h"
#include "TeamManager.h"
#include "GameTeamState.h"

#ifndef PCINPUT
    #define MOBILDINPUT
#endif

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    m_map   =   GameMap::create();
    this->addChild(m_map);

    /**
    	 算了，暂时就此处将游戏角色添加到地图上
    */

    auto tmpRole1 = GameCharacter::create(3);
    tmpRole1->setType(GAME_ENTITY_TYPE_PLAYER_CHARACTER);
    tmpRole1->retain();
    m_map->placeCharacter1(tmpRole1);

    auto tmpRole2 = GameCharacter::create(2);
    tmpRole2->setType(GAME_ENTITY_TYPE_PLAYER_CHARACTER);
    tmpRole2->retain();
    m_map->placeCharacter2(tmpRole2);

    auto tmpRole3 = GameCharacter::create(1);
    tmpRole3->setType(GAME_ENTITY_TYPE_PLAYER_CHARACTER);
    tmpRole3->retain();
    m_map->placeCharacter3(tmpRole3);

    // @_@ 创建为一个队伍
    auto tmpTeam1   =   GameTeam::create();
    tmpTeam1->setLeaderId(tmpRole2->getId());
    tmpTeam1->addMercenaryIds(tmpRole1->getId());
    tmpTeam1->addMercenaryIds(tmpRole3->getId());
    TeamMgr->registerTeam(tmpTeam1);

    m_mainModel     =   new GameMainModel();
    m_mainModel->setMainGameCharacter(tmpRole2);
    
#ifdef PCINPUT
    m_inputManager  =   new PCInputManager();
#endif
    
#ifdef MOBILDINPUT
    m_inputManager  =   new MobileInputManager();
#endif
    
    m_inputManager->setDelegate(m_mainModel);

    /**
    	 这里还需要增加3个敌人
    */
    auto tmpRole4 = GameCharacter::create(4);
    tmpRole4->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);
    tmpRole4->retain();
    tmpRole4->getShape()->faceToLeft();
    m_map->placeEnemyCharacter1(tmpRole4);

    auto tmpRole5 = GameCharacter::create(4);
    tmpRole5->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);
    tmpRole5->retain();
    tmpRole5->getShape()->faceToLeft();
    m_map->placeEnemyCharacter2(tmpRole5);

    auto tmpRole6 = GameCharacter::create(4);
    tmpRole6->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);
    tmpRole6->retain();
    tmpRole6->getShape()->faceToLeft();
    m_map->placeEnemyCharacter3(tmpRole6);

    // 敌人也加为一对
    auto tmpTeam2   =   GameTeam::create();
    tmpTeam2->addMercenaryIds(tmpRole4->getId());
    tmpTeam2->addMercenaryIds(tmpRole5->getId());
    tmpTeam2->addMercenaryIds(tmpRole6->getId());
    TeamMgr->registerTeam(tmpTeam2);

    m_inputManager->init();

#ifdef MOBILDINPUT
    this->addChild(dynamic_cast<Layer*>(m_inputManager));
#endif

    // 启动调用update
    this->getScheduler()->schedule(CC_CALLBACK_1(GameScene::updateScene, this), 
        this, 0, false, "GameScene");
    //this->scheduleUpdate();

    return true;
}

void GameScene::updateScene(float delta)
{
    auto tmpTeam    =   TeamMgr->getTeamFromId(0);
    if (tmpTeam != nullptr)
    {
        auto tmpState   =   dynamic_cast<GameTeamCelebrateState*>(tmpTeam->getFSM()->getCurrentState());
        if (tmpState != nullptr)
        {
            // 说明胜利了，此时开始计数
            m_celebrateFrameCount++;
            if (m_celebrateFrameCount >= m_celebrateFrame)
            {
                // 告诉队伍向后推进
            }
        }
    }
}