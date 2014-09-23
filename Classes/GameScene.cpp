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
    tmpRole1->getMovingEntity().setPosition(Vec2(100, 100));
    auto tmpTeam1   =   GameTeam::create(GAME_TEAM_TYPE_PLAYER);
    tmpTeam1->addMember(tmpRole1, 0);
    m_map->placeGameCharacter(tmpRole1);

    auto tmpRole2   =   GameCharacter::create(1);
    tmpRole2->setType(GAME_ENTITY_TYPE_PLAYER_CHARACTER);
    tmpRole2->getMovingEntity().setPosition(Vec2(660, 100));
    tmpTeam1->addMember(tmpRole2, 1);
    m_map->placeGameCharacter(tmpRole2);

    auto tmpRole3   =   GameCharacter::create(1);
    tmpRole3->setType(GAME_ENTITY_TYPE_PLAYER_CHARACTER);
    tmpRole3->getMovingEntity().setPosition(Vec2(1000, 50));
    tmpTeam1->addMember(tmpRole3, 2);
    m_map->placeGameCharacter(tmpRole3);

    auto tmpRole4   =   GameCharacter::create(1);
    tmpRole4->setType(GAME_ENTITY_TYPE_PLAYER_CHARACTER);
    tmpRole4->getMovingEntity().setPosition(Vec2(700, 30));
    tmpTeam1->addMember(tmpRole4, 3);
    m_map->placeGameCharacter(tmpRole4);

    auto tmpRole5   =   GameCharacter::create(2);
    tmpRole5->setType(GAME_ENTITY_TYPE_PLAYER_CHARACTER);
    tmpRole5->getMovingEntity().setPosition(Vec2(50, 30));
    tmpTeam1->addMember(tmpRole5, 4);
    m_map->placeGameCharacter(tmpRole5);

    auto tmpRole6   =   GameCharacter::create(2);
    tmpRole6->setType(GAME_ENTITY_TYPE_PLAYER_CHARACTER);
    tmpRole6->getMovingEntity().setPosition(Vec2(0, 30));
    tmpTeam1->addMember(tmpRole6, 5);
    m_map->placeGameCharacter(tmpRole6);

    tmpTeam1->getTeamFormation().setFormationAnchor(Vec2(500, 240));
    
    /**
    * 添加的敌人 
    */
    auto tmpRole7   =   GameCharacter::create(1);
    tmpRole7->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);
    tmpRole7->getMovingEntity().setPosition(Vec2(1300, 200));

    auto tmpRole8   =   GameCharacter::create(1);
    tmpRole8->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);
    tmpRole8->getMovingEntity().setPosition(Vec2(1300, 200));

    auto tmpRole9   =   GameCharacter::create(1);
    tmpRole9->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);
    tmpRole9->getMovingEntity().setPosition(Vec2(1300, 200));

    auto tmpRole10   =   GameCharacter::create(1);
    tmpRole10->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);
    tmpRole10->getMovingEntity().setPosition(Vec2(1300, 200));

    auto tmpRole11   =   GameCharacter::create(1);
    tmpRole11->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);
    tmpRole11->getMovingEntity().setPosition(Vec2(1300, 200));

    auto tmpRole12   =   GameCharacter::create(1);
    tmpRole12->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);
    tmpRole12->getMovingEntity().setPosition(Vec2(1300, 200));

    auto tmpTeam2   =   GameTeam::create(GAME_TEAM_TYPE_ENEMY);
    tmpTeam2->getTeamFormation().setFormationType(Formation::FORMATION_TYPE_LEFT);
    tmpTeam2->getTeamFormation().setFormationAnchor(Vec2(1500, 240));
    m_map->placeGameCharacter(tmpRole7);
    m_map->placeGameCharacter(tmpRole8);
    m_map->placeGameCharacter(tmpRole9);
    m_map->placeGameCharacter(tmpRole10);
    m_map->placeGameCharacter(tmpRole11);
    m_map->placeGameCharacter(tmpRole12);
    tmpTeam2->addMember(tmpRole7, 1);
    tmpTeam2->addMember(tmpRole8, 2);
    tmpTeam2->addMember(tmpRole9, 3);
    tmpTeam2->addMember(tmpRole10, 4);
    tmpTeam2->addMember(tmpRole11, 5);
    tmpTeam2->addMember(tmpRole12, 0);
    
    /**
    *	再添加第三队 
    */
    auto tmpRole13   =   GameCharacter::create(1);
    tmpRole13->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);
    tmpRole13->getMovingEntity().setPosition(Vec2(1300, 200));

    auto tmpRole14   =   GameCharacter::create(1);
    tmpRole14->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);
    tmpRole14->getMovingEntity().setPosition(Vec2(1300, 200));

    auto tmpRole15   =   GameCharacter::create(1);
    tmpRole15->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);
    tmpRole15->getMovingEntity().setPosition(Vec2(1300, 200));

    auto tmpRole16   =   GameCharacter::create(1);
    tmpRole16->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);
    tmpRole16->getMovingEntity().setPosition(Vec2(1300, 200));

    auto tmpRole17   =   GameCharacter::create(1);
    tmpRole17->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);
    tmpRole17->getMovingEntity().setPosition(Vec2(1300, 200));

    auto tmpRole18   =   GameCharacter::create(1);
    tmpRole18->setType(GAME_ENTITY_TYPE_ENEMY_CHARACTER);
    tmpRole18->getMovingEntity().setPosition(Vec2(1300, 200));

    auto tmpTeam3   =   GameTeam::create(GAME_TEAM_TYPE_ENEMY);
    tmpTeam3->getTeamFormation().setFormationType(Formation::FORMATION_TYPE_LEFT);
    tmpTeam3->getTeamFormation().setFormationAnchor(Vec2(2800, 240));
    m_map->placeGameCharacter(tmpRole13);
    m_map->placeGameCharacter(tmpRole14);
    m_map->placeGameCharacter(tmpRole15);
    m_map->placeGameCharacter(tmpRole16);
    m_map->placeGameCharacter(tmpRole17);
    m_map->placeGameCharacter(tmpRole18);
    tmpTeam3->addMember(tmpRole13, 1);
    tmpTeam3->addMember(tmpRole14, 2);
    tmpTeam3->addMember(tmpRole15, 3);
    tmpTeam3->addMember(tmpRole16, 4);
    tmpTeam3->addMember(tmpRole17, 5);
    tmpTeam3->addMember(tmpRole18, 0);

    // 队伍恢复阵型
    tmpTeam1->getTeamBrain()->addSubgoal(new GoalTeamRecoverFormation(tmpTeam1, 12));
    tmpTeam2->getTeamBrain()->addSubgoal(new GoalTeamRecoverFormation(tmpTeam2, 15));
    tmpTeam3->getTeamBrain()->addSubgoal(new GoalTeamRecoverFormation(tmpTeam3, 30));

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