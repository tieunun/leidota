#include "GameMap.h"
#include "EntityManager.h"
#include "TeamManager.h"
#include "cocostudio/CCSSceneReader.h"

using namespace cocostudio;

GameMap* GameMap::m_instance    =   nullptr;
GameMap* GameMap::instance()
{
    if (m_instance == nullptr)
    {
        m_instance  =   new GameMap();
        m_instance->init();
    }

    return m_instance;
}

bool GameMap::init()
{
    if (!Layer::init())
    {
        return false;
    }

    // 加载场景编辑器编辑的场景
    auto tmpScene   =   SceneReader::getInstance()->createNodeWithSceneFile("leidota.json");
    this->addChild(tmpScene);

    // 背后的天空
    m_bg3   =   tmpScene->getChildByTag(10013);
    // 背后的山
    m_bg2   =   tmpScene->getChildByTag(10010);
    // 人物所在的背景
    m_bg1   =   tmpScene->getChildByTag(10006);

    // 四面的墙
    m_walls.push_back(Wall2D(m_wallLBPos, Vec2(m_wallRTPos.x, 0), Vec2(0, 1)));
    m_walls.push_back(Wall2D(Vec2(m_wallRTPos.x, 0), m_wallRTPos, Vec2(-1, 0)));
    m_walls.push_back(Wall2D(m_wallRTPos, Vec2(0, m_wallRTPos.y), Vec2(0, -1)));
    m_walls.push_back(Wall2D(Vec2(0, m_wallRTPos.y), m_wallLBPos, Vec2(1, 0)));

    // @_@ 所有的角色都是添加到这个上面
    m_playerLayer =   m_bg1->getChildByTag(10025);

    this->scheduleUpdate();

    return true;
}

GameMap::~GameMap()
{

}

void GameMap::cameraMove(int x)
{
    auto tmpTeam    =   TeamMgr->getTeamFromId(0);
    if (tmpTeam != nullptr)
    {
        // 调整当前人物所在
        auto tmpCharacterPosX   =   tmpTeam->getTeamFormation().getFormationAnchor().x;
        auto visibleSize        = Director::getInstance()->getVisibleSize();
        float parentX           = visibleSize.width * 2 / 3 - tmpCharacterPosX;
        parentX    =    parentX > 0 ? 0 : parentX;
        m_bg1->setPositionX(parentX);

        // 调整山，移动速度慢一些
        m_bg2->setPositionX(parentX * MOUNTAIN_MOVE_SCALE);
        m_bg3->setPositionX(parentX * SKY_MOVE_SCALE);
    }
}

void GameMap::update( float x)
{
    cameraMove(0);

    // 同时调整在地图上面的所有角色的z
    auto tmpChildren    =   m_playerLayer->getChildren();
    for (auto tmpIterator = tmpChildren.begin(); tmpIterator != tmpChildren.end(); tmpIterator++)
    {
        (*tmpIterator)->setZOrder(500 - (*tmpIterator)->getPositionY());
    }
}

void GameMap::placeGameCharacter( GameCharacter* player )
{
    m_playerLayer->addChild(player->getShape());
}