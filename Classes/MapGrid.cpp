#include "MapGrid.h"
#include "GameCharacter.h"
#include "GraphSearchAStar.h"
#include "EntityManager.h"

#define DRAW_MAP_GRID

/**
* 主要是因为在编译android的时候发现没有定义abs，而应该使用fabs 
*/
#ifndef WIN32
    #define abs(value)  fabs(value)
#endif

bool MapGrid::init()
{
    if (!Layer::init())
    {
        return false;
    }

    // 设置大小
    this->setContentSize(Size(_gridW * _xNum, _gridH * _yNum));

    // 绘制网格
#ifdef DRAW_MAP_GRID
    m_drawNode  =   DrawNode::create();
    this->addChild(m_drawNode);
#endif
    drawGrid();

    return true;
}

MapGrid::MapGrid(int gridW, int gridH, int xNum, int yNum):GridGraph(gridW, gridH, xNum, yNum),m_gridColor(1.0f, 0.0f, 0.0f, 1.0f)
{
    this->setContentSize(Size(gridW * xNum, gridH * yNum));
}

MapGrid::~MapGrid()
{

}

void MapGrid::drawGrid()
{
    // 设置网格的passable
    resetAllNodePassable();
    for (auto tmpObjectIterator  =   m_Objects.begin(); tmpObjectIterator != m_Objects.end(); tmpObjectIterator++)
    {
        setObjectOccupyGrid(*tmpObjectIterator);
    }

#ifdef DRAW_MAP_GRID
    // 首先清除
    m_drawNode->clear();
    // 绘制水平线
    for (int y = 0; y <= _yNum; y++)
    {
        m_drawNode->drawSegment(Vec2(0, y * _gridH), Vec2(_xNum * _gridW, y * _gridH), 1, m_gridColor);
    }

    // 绘制竖直线
    for (int x = 0; x <= _xNum; x++)
    {
        m_drawNode->drawSegment(Vec2(x * _gridW, 0), Vec2(x * _gridW, _yNum * _gridH), 1, m_gridColor);
    }

    // 填充不能使用的格子，这里一个格子一个格子的查找
    auto tmpIterator = m_nodes.begin();
    for (; tmpIterator != m_nodes.end(); tmpIterator++)
    {
        auto tmpGrid = tmpIterator->second;
        if (!tmpGrid.passable())
        {
            // 不可通行的
            Vec2 tmpPoints[4];
            tmpPoints[0].setPoint(tmpGrid.getX() - _gridW / 2, tmpGrid.getY() - _gridH / 2);
            tmpPoints[1].setPoint(tmpGrid.getX() + _gridW / 2, tmpGrid.getY() - _gridH / 2);
            tmpPoints[2].setPoint(tmpGrid.getX() + _gridH / 2, tmpGrid.getY() + _gridH / 2);
            tmpPoints[3].setPoint(tmpGrid.getX() - _gridW / 2, tmpGrid.getY() + _gridH / 2);
            m_drawNode->drawPolygon(tmpPoints, 4, m_gridColor, 0, m_gridColor);
        }
    }
#endif
}

void MapGrid::addObjectToGrid(ObjectOnMapGrid* object)
{
    m_Objects.push_back(object);
    /**
    	 每次修改人物所占位置的时候，都要调用这个函数
    */
    adjustGameCharacterZOrder();
    drawGrid();
}

void MapGrid::removeObjectFromGrid(ObjectOnMapGrid* object)
{
    for (auto tmpIteator = m_Objects.begin(); tmpIteator != m_Objects.end(); tmpIteator++)
    {
        if (object == *tmpIteator)
        {
            m_Objects.erase(tmpIteator);
            break;
        }
    }
    drawGrid();
}

void MapGrid::setObjectOccupyGrid(ObjectOnMapGrid* object)
{
    setNodePassable(object->nodeIndex, false);

    switch (object->placeType)
    {
    case PLACEHOLDER_TYPE_ONE:                          // 只占一个格子
        break;

    case PLACEHOLDER_TYPE_CROSS:                        // 占一个十字格子
        {
            auto tmpGridIndex = getTopGridIndex(object->nodeIndex);
            if (tmpGridIndex != INVALID_NODE_INDEX)
            {
                setNodePassable(tmpGridIndex, false);
            }
            tmpGridIndex = getLeftGridIndex(object->nodeIndex);
            if (tmpGridIndex != INVALID_NODE_INDEX)
            {
                setNodePassable(tmpGridIndex, false);
            }
            tmpGridIndex = getBottomGridIndex(object->nodeIndex);
            if (tmpGridIndex != INVALID_NODE_INDEX)
            {
                setNodePassable(tmpGridIndex, false);
            }
            tmpGridIndex = getRightGridIndex(object->nodeIndex);
            if (tmpGridIndex != INVALID_NODE_INDEX)
            {
                setNodePassable(tmpGridIndex, false);
            }
            break;
        }

    default:
        break;
    }
}

void MapGrid::addGameCharacter(GameCharacter* character)
{
    // 添加角色外形
    auto tmpGridNode    =   this->getNodeByIndex(character->getObjectOnGrid()->nodeIndex);
    auto tmpShape       =   character->getShape();
    tmpShape->setPosition(tmpGridNode.getX(), tmpGridNode.getY());
    this->addChild(tmpShape);

    // 在网格中占用格子
    this->addObjectToGrid(character->getObjectOnGrid());

    // 允许该角色访问该网格
    character->setGridGraph(this);
}

void MapGrid::adjustGameCharacterZOrder()
{
    // 按照网格序号来排列
    auto tmpAllCharacters   =   EntityMgr->getEntityMap();
    for (auto tmpChildIterator = tmpAllCharacters->begin(); tmpChildIterator != tmpAllCharacters->end(); tmpChildIterator++)
    {
        auto tmpCharacter   =   (GameCharacter*)tmpChildIterator->second;
        tmpCharacter->getShape()->setZOrder(_xNum - tmpCharacter->getObjectOnGrid()->nodeIndex / _xNum);
    }
}

int MapGrid::getLeftGridIndex(int nodeIndex, int distance)
{
    if (nodeIndex == INVALID_NODE_INDEX || nodeIndex % _xNum < distance)
    {
        return INVALID_NODE_INDEX;
    }

    return nodeIndex - distance;
}

int MapGrid::getTopGridIndex(int nodeIndex, int distance)
{
    if (nodeIndex == INVALID_NODE_INDEX || nodeIndex >= _xNum * (_yNum - distance))
    {
        return INVALID_NODE_INDEX;
    }

    return nodeIndex + _xNum * distance;
}

int MapGrid::getRightGridIndex(int nodeIndex, int distance)
{
    if (nodeIndex == INVALID_NODE_INDEX || nodeIndex % _xNum >= _xNum - distance)
    {
        return INVALID_NODE_INDEX;
    }

    return nodeIndex + distance;
}

int MapGrid::getBottomGridIndex(int nodeIndex, int distance)
{
    if (nodeIndex == INVALID_NODE_INDEX || nodeIndex < _xNum * distance)
    {
        return INVALID_NODE_INDEX;
    }

    return nodeIndex - _xNum * distance;
}

int MapGrid::getLeftTopGridIndex(int nodeIndex)
{
    return getTopGridIndex(getLeftGridIndex(nodeIndex));
}

int MapGrid::getRightTopGridIndex(int nodeIndex)
{
    return getTopGridIndex(getRightGridIndex(nodeIndex));
}

int MapGrid::getRightBottomGridIndex(int nodeIndex)
{
    return getBottomGridIndex(getRightGridIndex(nodeIndex));
}

int MapGrid::getLeftBottomGridIndex(int nodeIndex)
{
    return getBottomGridIndex(getLeftGridIndex(nodeIndex));
}

vector<int> MapGrid::getRouteToTargetObj(ObjectOnMapGrid* source, ObjectOnMapGrid* target)
{
    // 在寻路前从网格中移除之前的占有的网格
    this->removeObjectFromGrid(source);
    vector<int> retShortRoute;

    // 下面根据目标占有的网格来设置寻路目标
    switch (target->placeType)
    {
    case PLACEHOLDER_TYPE_ONE:
        {
            // 对于只占用一个格子的，只可能是前后两个目标
            vector<int> tmpToFrontRoute;
            vector<int> tmpToBackRoute;
            auto tmpFrontGridIndex   =   getLeftGridIndex(target->nodeIndex);
            if (tmpFrontGridIndex != INVALID_NODE_INDEX)
            {
                // 寻找路径
                tmpToFrontRoute = GraphSearchAStar::search(*this, source->nodeIndex, tmpFrontGridIndex, MapGrid::calculateH);
            }

            auto tmpBackGridIndex    =   getRightGridIndex(target->nodeIndex);
            if (tmpBackGridIndex != INVALID_NODE_INDEX)
            {
                tmpToBackRoute = GraphSearchAStar::search(*this, source->nodeIndex, tmpBackGridIndex, MapGrid::calculateH);
            }

            // 选择最近的
            if (tmpToFrontRoute.size() != 0 && tmpToFrontRoute.size() <= tmpToBackRoute.size())
            {
                retShortRoute = tmpToFrontRoute;
            }
            else
            {
                retShortRoute = tmpToBackRoute;
            }

            break;
        }
        

    case PLACEHOLDER_TYPE_CROSS:
        {
            // 对于十字架，有的可能目标就太多了
            vector<int> tmpTargetGridIndexs;
            auto tmpGridIndex   =   getLeftGridIndex(getLeftGridIndex(target->nodeIndex));
            if (tmpGridIndex != INVALID_NODE_INDEX)
            {
                tmpTargetGridIndexs.push_back(tmpGridIndex);
            }
            tmpGridIndex    =   getTopGridIndex(getLeftGridIndex(target->nodeIndex));
            if (tmpGridIndex != INVALID_NODE_INDEX)
            {
                tmpTargetGridIndexs.push_back(tmpGridIndex);
            }
            tmpGridIndex    =   getBottomGridIndex(getLeftGridIndex(target->nodeIndex));
            if (tmpGridIndex != INVALID_NODE_INDEX)
            {
                tmpTargetGridIndexs.push_back(tmpGridIndex);
            }
            tmpGridIndex    =   getTopGridIndex(getRightGridIndex(target->nodeIndex));
            if (tmpGridIndex != INVALID_NODE_INDEX)
            {
                tmpTargetGridIndexs.push_back(tmpGridIndex);
            }
            tmpGridIndex    =   getBottomGridIndex(getRightGridIndex(target->nodeIndex));
            if (tmpGridIndex != INVALID_NODE_INDEX)
            {
                tmpTargetGridIndexs.push_back(tmpGridIndex);
            }
            tmpGridIndex    =   getRightGridIndex(getRightGridIndex(target->nodeIndex));
            if (tmpGridIndex != INVALID_NODE_INDEX)
            {
                tmpTargetGridIndexs.push_back(tmpGridIndex);
            }
            // 这里做点优化，免得寻路次数太多了
            if (source->nodeIndex % _xNum > target->nodeIndex % _xNum)
            {
                reverse(tmpTargetGridIndexs.begin(), tmpTargetGridIndexs.end());
            }

            // 寻找最短路径的
            for (int i = 0; i < tmpTargetGridIndexs.size(); i++)
            {
                auto tmpRoute   =   GraphSearchAStar::search(*this, source->nodeIndex, tmpTargetGridIndexs[i], MapGrid::calculateH);
                if ((retShortRoute.size() == 0 || tmpRoute.size() < retShortRoute.size()) && tmpRoute.size() > 0)
                {
                    retShortRoute   =   tmpRoute;
                    break;
                }
            }

            break;
        }
        
    default:
        break;
    }

    this->addObjectToGrid(source);
    return retShortRoute;
}

float MapGrid::calculateH(SparseGraph& grap, int sourceIndex, int targetIndex)
{
    MapGrid* tmpMapGrid =   (MapGrid*)&grap;
    auto tmpSourceGrid  =   grap.getNodeByIndex(sourceIndex);
    auto tmpTargetGrid  =   grap.getNodeByIndex(targetIndex);

    // 计算之间的距离
    int tmpXDistance    =   abs(tmpSourceGrid.index() % tmpMapGrid->_xNum - tmpTargetGrid.index() % tmpMapGrid->_xNum);
    int tmpYDistance    =   abs(tmpSourceGrid.index() / tmpMapGrid->_xNum - tmpTargetGrid.index() / tmpMapGrid->_xNum);
    return tmpXDistance + tmpYDistance;
}

bool MapGrid::testTwoIndexInOneHorizon(int nodeIndex1, int nodeIndex2)
{
    return nodeIndex1 / _xNum == nodeIndex2 / _xNum;
}

bool MapGrid::testIsAtLeft(int nodeIndex1, int nodeIndex2)
{
    return nodeIndex1 % _xNum < nodeIndex2 % _xNum;
}

bool MapGrid::testIsAtTop(int nodeIndex1, int nodeIndex2)
{
    return nodeIndex1 / _xNum > nodeIndex2 / _xNum;
}

bool MapGrid::testIsAtRight(int nodeIndex1, int nodeIndex2)
{
    return nodeIndex1 % _xNum > nodeIndex2 % _xNum;
}

bool MapGrid::testIsAtBottom(int nodeIndex1, int nodeIndex2)
{
    return nodeIndex1 / _xNum < nodeIndex2 / _xNum;
}