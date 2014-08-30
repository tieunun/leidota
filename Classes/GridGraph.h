#ifndef __GRID_GRAPH_H__
#define __GRID_GRAPH_H__

#include "SparseGraph.h"

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
};

#endif