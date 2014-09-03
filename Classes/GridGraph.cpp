#include "GridGraph.h"
#include <math.h>

GridGraph::GridGraph( int gridW, int gridH, int xNum, int yNum )
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

bool GridGraph::isInScope( int center, int target, int distance )
{
    // 这里直接判断必须在两个轴上的
    return abs(center / _xNum - target / _xNum) <= distance && abs(center % _xNum - target % _xNum) <= distance;
}

int GridGraph::getDistanceInGrid( int index1, int index2 )
{
    return abs(index1 / _xNum - index2 / _xNum) + abs(index1 % _xNum - index2 % _xNum);
}

vector<int> GridGraph::getUnusedGridIndex( int centerIndex, int radius )
{
    // @_@ 暂时先不用
    return vector<int>();
}