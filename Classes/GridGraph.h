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
    GridGraph(int gridW, int gridH, int xNum, int yNum)
    {
        _gridW  =   gridW;
        _gridH  =   gridH;
        _xNum   =   xNum;
        _yNum   =   yNum;

        // 先添加节点
        for (int y = 0; y < yNum; y++)
        {
            for (int x = 0; x < xNum; x++)
            {
                this->addNode(NavGraphNode(y * xNum + x, gridW * x + gridW / 2, gridH * y + gridH / 2));
            }
        }

        // 添加边
        for (int y = 0; y < yNum; y++)
        {
            for (int x = 0; x < xNum; x++)
            {
                if (x < xNum - 1)
                {
                    this->addEdge(NavGraphEdge(y * xNum + x, y * xNum + x + 1));
                }
                
                if (y < yNum - 1)
                {
                    this->addEdge(NavGraphEdge(y * xNum + x, (y + 1) * xNum + x));
                }
            }
        }
    }
};

#endif