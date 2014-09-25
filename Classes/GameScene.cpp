#include "GameScene.h"
#include "GameCharacter.h"
#include "TeamManager.h"
#include "EntityManager.h"
#include "BattleUI.h"
#include "GoalTeamAttackTargetTeam.h"
#include "GoalTeamRecoverFormation.h"
#include "GoalTeamGuard.h"
#include "GoalTeamThink.h"

//#define ADDPCINPUT

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
    tmpRole1->setType(GAME_ENTITY_TYPE_PLAYER_CHARACTER);

    auto tmpRole2   =   GameCharacter::create(6);
    tmpRole2->setType(GAME_ENTITY_TYPE_PLAYER_CHARACTER);

    auto tmpRole3   =   GameCharacter::create(1);
    tmpRole3->setType(GAME_ENTITY_TYPE_PLAYER_CHARACTER);

    auto tmpRole4   =   GameCharacter::create(6);
    tmpRole4->setType(GAME_ENTITY_TYPE_PLAYER_CHARACTER);

    auto tmpRole5   =   GameCharacter::create(2);
    tmpRole5->setType(GAME_ENTITY_TYPE_PLAYER_CHARACTER);

    auto tmpRole6   =   GameCharacter::create(1);
    tmpRole6->setType(GAME_ENTITY_TYPE_PLAYER_CHARACTER);

    auto tmpTeam1   =   GameTeam::create(GAME_TEAM_TYPE_PLAYER);
    tmpTeam1->getTeamFormation().setFormationAnchor(Vec2(500, 300));
    m_map->placeGameCharacter(tmpRole1);
    m_map->placeGameCharacter(tmpRole2);
    m_map->placeGameCharacter(tmpRole3);
    m_map->placeGameCharacter(tmpRole4);
    m_map->placeGameCharacter(tmpRole5);
    m_map->placeGameCharacter(tmpRole6);
    tmpTeam1->addMember(tmpRole1, 0);
    tmpTeam1->addMember(tmpRole2, 1);
    tmpTeam1->addMember(tmpRole3, 2);
    tmpTeam1->addMember(tmpRole4, 6);
    tmpTeam1->addMember(tmpRole5, 7);
    tmpTeam1->addMember(tmpRole6, 8);

    /**
    * 添加的敌人 
    */
    auto tmpRole7   =   GameCharacter::create(4);
    tmpRole7->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);

    auto tmpRole8   =   GameCharacter::create(4);
    tmpRole8->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);

    auto tmpRole9   =   GameCharacter::create(5);
    tmpRole9->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);

    auto tmpRole10   =   GameCharacter::create(4);
    tmpRole10->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);

    auto tmpRole11   =   GameCharacter::create(4);
    tmpRole11->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);

    auto tmpRole12   =   GameCharacter::create(5);
    tmpRole12->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);

    auto tmpTeam2   =   GameTeam::create(GAME_TEAM_TYPE_ENEMY);
    tmpTeam2->getTeamFormation().setFormationType(Formation::FORMATION_TYPE_LEFT);
    tmpTeam2->getTeamFormation().setFormationAnchor(Vec2(1500, 300));
    m_map->placeGameCharacter(tmpRole7);
    m_map->placeGameCharacter(tmpRole8);
    m_map->placeGameCharacter(tmpRole9);
    m_map->placeGameCharacter(tmpRole10);
    m_map->placeGameCharacter(tmpRole11);
    m_map->placeGameCharacter(tmpRole12);
    tmpTeam2->addMember(tmpRole7, 0);
    tmpTeam2->addMember(tmpRole8, 1);
    tmpTeam2->addMember(tmpRole9, 2);
    tmpTeam2->addMember(tmpRole10, 6);
    tmpTeam2->addMember(tmpRole11, 7);
    tmpTeam2->addMember(tmpRole12, 8);
    
    /**
    *	再添加第三队 
    */
    auto tmpRole13   =   GameCharacter::create(7);
    tmpRole13->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);

    auto tmpRole14   =   GameCharacter::create(7);
    tmpRole14->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);

    auto tmpRole15   =   GameCharacter::create(5);
    tmpRole15->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);

    auto tmpRole16   =   GameCharacter::create(5);
    tmpRole16->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);

    auto tmpRole17   =   GameCharacter::create(2);
    tmpRole17->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);

    auto tmpRole18   =   GameCharacter::create(2);
    tmpRole18->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);

    auto tmpTeam3   =   GameTeam::create(GAME_TEAM_TYPE_ENEMY);
    tmpTeam3->getTeamFormation().setFormationType(Formation::FORMATION_TYPE_LEFT);
    tmpTeam3->getTeamFormation().setFormationAnchor(Vec2(2800, 300));
    m_map->placeGameCharacter(tmpRole13);
    m_map->placeGameCharacter(tmpRole14);
    m_map->placeGameCharacter(tmpRole15);
    m_map->placeGameCharacter(tmpRole16);
    m_map->placeGameCharacter(tmpRole17);
    m_map->placeGameCharacter(tmpRole18);
    tmpTeam3->addMember(tmpRole13, 0);
    tmpTeam3->addMember(tmpRole14, 1);
    tmpTeam3->addMember(tmpRole15, 2);
    tmpTeam3->addMember(tmpRole16, 6);
    tmpTeam3->addMember(tmpRole17, 7);
    tmpTeam3->addMember(tmpRole18, 8);

    // 队伍恢复阵型
    tmpTeam1->getTeamBrain()->addSubgoal(new GoalTeamRecoverFormation(tmpTeam1, 12));
    tmpTeam2->getTeamBrain()->addSubgoal(new GoalTeamRecoverFormation(tmpTeam2, 15));
    tmpTeam3->getTeamBrain()->addSubgoal(new GoalTeamRecoverFormation(tmpTeam3, 35));

    // 设置队伍级别目标
    tmpTeam1->getTeamBrain()->addSubgoal(new GoalTeamAttackTargetTeam(tmpTeam1, tmpTeam2));
    tmpTeam1->getTeamBrain()->addSubgoal(new GoalTeamAttackTargetTeam(tmpTeam1, tmpTeam3));
    tmpTeam2->getTeamBrain()->addSubgoal(new GoalTeamGuard(tmpTeam2));
    tmpTeam3->getTeamBrain()->addSubgoal(new GoalTeamGuard(tmpTeam3));

    m_mainModel =   new GameMainModel();

//#ifdef ADDPCINPUT                   // 如果添加了使用PC键盘输入
//    m_pcInputManager  =   new PCInputManager();
//    m_pcInputManager->setDelegate(m_mainModel);
//    m_pcInputManager->init();
//#endif

    // 战斗UI
    // auto tmpUI  =   BattleUI::create();
    // this->addChild(tmpUI);
    // 允许手机输入的，也就是屏幕输入方式
    // m_mobileInputManager    =   tmpUI;
    // m_mobileInputManager->setDelegate(m_mainModel);

    return true;
}