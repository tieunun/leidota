#include "GameMap.h"
#include "EntityManager.h"
#include "cocostudio/CCSSceneReader.h"

using namespace cocostudio;

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

    // 网格，所有的角色都是添加到这个上面
    m_mapGrid   =   MapGrid::create(GRIDW, GRIDH, XNUM, YNUM);
    auto tmpMapNode =   m_bg1->getChildByTag(10025);
    tmpMapNode->addChild(m_mapGrid);

    m_initPosX  =   0;

    this->scheduleUpdate();

    return true;
}

void GameMap::placeCharacter1(GameCharacter* character)
{
    placeOneCharacterToIndex(character, 0);
}

void GameMap::placeCharacter2(GameCharacter* character)
{
    placeOneCharacterToIndex(character, 2 * XNUM);
}

void GameMap::placeCharacter3(GameCharacter* character)
{
    placeOneCharacterToIndex(character, 4 * XNUM);
}

void GameMap::placeOneCharacterToIndex(GameCharacter* character, int nodeIndex)
{
    // 这里暂时放在这里
    auto tmpGrid = character->getObjectOnGrid();
    tmpGrid->placeType  =   PLACEHOLDER_TYPE_CROSS;
    tmpGrid->nodeIndex  =   nodeIndex;

    m_mapGrid->addGameCharacter(character);
}

void GameMap::placeEnemyCharacter1(GameCharacter* character)
{
    placeOneCharacterToIndex(character, XNUM - 1);
}

void GameMap::placeEnemyCharacter2(GameCharacter* character)
{
    placeOneCharacterToIndex(character, 3 * XNUM - 1);
}

void GameMap::placeEnemyCharacter3(GameCharacter* character)
{
    placeOneCharacterToIndex(character, 5 * XNUM - 1);
}

GameMap::~GameMap()
{

}

void GameMap::cameraMove(int x)
{
    // 主角相对地图移动了x的距离，@_@ 先假设佣兵主角永不
    // auto tmpCharacterPosX    =   EntityMgr->getEntityFromID(1)->getShape()->getPositionX();
    auto tmpCharacter   =   (GameCharacter*)EntityMgr->getmainEntity();
    if (tmpCharacter != nullptr)
    {
        // 调整当前人物所在
        auto tmpCharacterPosX   =   tmpCharacter->getShape()->getPositionX();
        auto visibleSize        = Director::getInstance()->getVisibleSize();
        float parentX           = visibleSize.width / 2 - tmpCharacterPosX;
        parentX    =    parentX > 0 ? 0 : parentX;
        m_bg1->setPositionX(parentX);

        // 调整山，移动速度慢一些
        m_bg2->setPositionX(parentX * MOUNTAIN_MOVE_SCALE);
        m_bg3->setPositionX(parentX * SKY_MOVE_SCALE);
    }
}

void GameMap::update( float )
{
    cameraMove(0);
}
