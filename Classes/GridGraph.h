#ifndef __GRID_GRAPH_H__
#define __GRID_GRAPH_H__

#include <vector>
#include "SparseGraph.h"
#include "MathTool.h"

using namespace std;

/**
	 很多地方使用网格图，而网格中其中一些格子是障碍无法通行
*/
class GridGraph : public SparseGraph
{
protected:
    int _xNum;
    int _yNum;
    int _gridW;
    int _gridH;

public:
    /**
    	 创建一个全通的网格
         @grieW         单个网格宽度
         @gridH         单个网格高度
         @xNum          水平网格数量
         @yNum          竖直网格数量
    */
    GridGraph(int gridW, int gridH, int xNum, int yNum);

    /**
    *  判断以center为中心，以distance为范围，target是否在范围内
    */
    bool isInScope(int center, int target, int distance);

    /**
    *  返回网格中给定两个下标的距离，单位是网格数
    */
    int getDistanceInGrid(int index1, int index2);

    /**
    *  返回网格中给定下标的几何距离
    */
    int getDistance(int index1, int index2);

    /**
    * 返回以centerIndex为中心，radius格子数为半径的方块内还没有使用的网格索引 
    */
    vector<int> getUnusedGridIndex(int centerIndex, int radius);
};

#endif