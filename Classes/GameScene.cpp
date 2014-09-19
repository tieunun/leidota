#include "GameScene.h"
#include "GameCharacter.h"
#include "TeamManager.h"
#include "EntityManager.h"
#include "BattleUI.h"
#include "GoalTeamAttackTargetTeam.h"
#include "GoalTeamThink.h"

#define ADDPCINPUT

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    m_celebrateFrameCount   =   0;
    m_map   =   GameMap::instance();
    this->addChild(m_map);

    /**
    	 算了，暂时就此处将游戏角色添加到地图上
    */
    auto tmpRole1   =   GameCharacter::create(1);
    tmpRole1->getMovingEntity().setPosition(Vec2(100, 100));
    auto tmpTeam1   =   GameTeam::create();
    tmpTeam1->addMember(tmpRole1, 0);
    m_map->placeGameCharacter(tmpRole1);

    auto tmpRole2   =   GameCharacter::create(1);
    tmpRole2->getMovingEntity().setPosition(Vec2(660, 100));
    tmpTeam1->addMember(tmpRole2, 1);
    m_map->placeGameCharacter(tmpRole2);

    auto tmpRole3   =   GameCharacter::create(1);
    tmpRole3->getMovingEntity().setPosition(Vec2(1000, 50));
    tmpTeam1->addMember(tmpRole3, 2);
    m_map->placeGameCharacter(tmpRole3);

    auto tmpRole4   =   GameCharacter::create(1);
    tmpRole4->getMovingEntity().setPosition(Vec2(700, 30));
    tmpTeam1->addMember(tmpRole4, 3);
    m_map->placeGameCharacter(tmpRole4);

    auto tmpRole5   =   GameCharacter::create(1);
    tmpRole5->getMovingEntity().setPosition(Vec2(50, 30));
    tmpTeam1->addMember(tmpRole5, 4);
    m_map->placeGameCharacter(tmpRole5);

    auto tmpRole6   =   GameCharacter::create(1);
    tmpRole6->getMovingEntity().setPosition(Vec2(0, 30));
    tmpTeam1->addMember(tmpRole6, 5);
    m_map->placeGameCharacter(tmpRole6);

    tmpTeam1->getTeamFormation().setFormationAnchor(Vec2(500, 240));
    
    /**
    * 添加的敌人 
    */
    auto tmpRole7   =   GameCharacter::create(1);
    tmpRole7->getMovingEntity().setPosition(Vec2(1000, 200));

    auto tmpTeam2   =   GameTeam::create();
    tmpTeam2->getTeamFormation().setFormationType(Formation::FORMATION_TYPE_LEFT);
    tmpTeam2->getTeamFormation().setFormationAnchor(Vec2(1000, 240));
    m_map->placeGameCharacter(tmpRole7);
    tmpTeam2->addMember(tmpRole7, 1);

    // 设置队伍级别目标
    tmpTeam1->getTeamBrain()->setGoal(new GoalTeamAttackTargetTeam(tmpTeam1, tmpTeam2));
    tmpTeam2->getTeamBrain()->setGoal(new GoalTeamAttackTargetTeam(tmpTeam2, tmpTeam1));

    m_mainModel =   new GameMainModel();

#ifdef ADDPCINPUT                   // 如果添加了使用PC键盘输入
    m_pcInputManager  =   new PCInputManager();
    m_pcInputManager->setDelegate(m_mainModel);
    m_pcInputManager->init();
#endif

    // 战斗UI
    auto tmpUI  =   BattleUI::create();
    //this->addChild(tmpUI);
    // 允许手机输入的，也就是屏幕输入方式
    m_mobileInputManager    =   tmpUI;
    m_mobileInputManager->setDelegate(m_mainModel);

    return true;
}