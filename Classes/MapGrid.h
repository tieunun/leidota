#ifndef __MAP_GRID_H__
#define __MAP_GRID_H__

#include "cocos2d.h"
#include "GridGraph.h"

class GameCharacter;

using namespace cocos2d;
using namespace std;

/**
	 占位方式，比如只占一个格子，占一个十字架方式等
*/
enum PlaceHolderTypeEnum
{
    PLACEHOLDER_TYPE_ONE,           // 只占有当前一个格子
    PLACEHOLDER_TYPE_CROSS          // 占有一个十字架的
};

/**
	 在网格上移动的物体，会同时占有好几个格子，会有不同的占格子的方式
*/
class ObjectOnMapGrid
{
public:
    int                     nodeIndex;              // 当前所在的节点
    PlaceHolderTypeEnum     placeType;              // 占位方式
};

/**
	 地图网格，用来实现角色的移动，同时又具有搜索路径的功能
*/
class MapGrid : public Layer, public GridGraph
{
public:
    bool init() override;

    static MapGrid* create(int gridW, int gridH, int xNum, int yNum)
    {
        auto pRet   =   new MapGrid(gridW, gridH, xNum, yNum);
        if (pRet && pRet->init())
        {
            pRet->autorelease();
            return pRet;
        }

        return nullptr;
    }

    /**
    	 在网格上添加一个占有网格的对象，主要是一个网格已经不能满足要求了
    */
    void addObjectToGrid(ObjectOnMapGrid* object);

    /**
    	 从网格中移除一个占有网格的对象，比如在某个物体要开始寻路之前，先把自己从网格中移除
         然后在寻路，然后重新在网格上占位置
    */
    void removeObjectFromGrid(ObjectOnMapGrid* object);

    /**
    	 在网格中添加游戏角色
    */
    void addGameCharacter(GameCharacter* character);

    /**
    	 用来获取某个格子的附近的格子序号，如果没有，就返回INVALID_NODE_INDEX
    */
    int getLeftGridIndex(int nodeIndex, int distance = 1);
    int getTopGridIndex(int nodeIndex, int distance = 1);
    int getRightGridIndex(int nodeIndex, int distance = 1);
    int getBottomGridIndex(int nodeIndex, int distance = 1);

    int getLeftTopGridIndex(int nodeIndex);
    int getRightTopGridIndex(int nodeIndex);
    int getRightBottomGridIndex(int nodeIndex);
    int getLeftBottomGridIndex(int nodeIndex);

    /**
    	 返回从这个到目标的路径，最终的目标格子序号肯定不是target的nodeIndex，这个要根据
         占位方式以及当前的周围被占位方式决定的
    */
    vector<int> getRouteToTargetObj(ObjectOnMapGrid* source, ObjectOnMapGrid* target);

    /**
    	 用来计算启发因子的
    */
    static float calculateH(SparseGraph& grap, int sourceIndex, int targetIndex);

    /**
    	 测试两个索引是否在一条水平线上
    */
    bool testTwoIndexInOneHorizon(int nodeIndex1, int nodeIndex2);

    /**
    	 测试nodeIndex1是否在nodeIndex2的左侧
    */
    bool testIsAtLeft(int nodeIndex1, int nodeIndex2);
    /**
    	 测试nodeIndex1是否在nodeIndex2的上面
    */
    bool testIsAtTop(int nodeIndex1, int nodeIndex2);
    /**
    	 测试nodeIndex1是否在nodeIndex2的右边
    */
    bool testIsAtRight(int nodeIndex1, int nodeIndex2);
    /**
    	 测试nodeIndex1是否在nodeIndex2的下面
    */
    bool testIsAtBottom(int nodeIndex1, int nodeIndex2);

protected:
    MapGrid(int gridW, int gridH, int xNum, int yNum);
    ~MapGrid();

    /**
    	 绘制网格，对于不可以使用的格子，填充颜色
    */
    void drawGrid();

    /**
    	 在网格上设置该对象占有的位置不肯通行
    */
    void setObjectOccupyGrid(ObjectOnMapGrid* object);

    /**
    	 因为在人物移动的时候要不停的根据y值调整前后级关系
    */
    void adjustGameCharacterZOrder();

    DrawNode*   m_drawNode;                             // 用来绘制的地方，用来反应网格的
    vector<ObjectOnMapGrid*> m_Objects;                 // 所有在网格中移动的物体

    const Color4F m_gridColor;                          // 网格使用的颜色
};

#endif