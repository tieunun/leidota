#ifndef __PRIORITY_Q_LOW_H__
#define __PRIORITY_Q_LOW_H__

#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

/**
	用来返回最小值的优先级队列
*/
class PriorityQLow
{
private:
    vector<float>&   m_priorityValues;           // 依赖这个值
    vector<int>     m_highToLow;                // 从大到小排序

public:
    PriorityQLow(vector<float>& priorityValues):m_priorityValues(priorityValues)
    {
        m_highToLow.clear();
    }

    /**
    	加入一个新的节点
    */
    void insert(int index)
    {
        m_highToLow.push_back(index);
        reOrder();
    }

    /**
    	判断是否为空
    */
    bool empty()
    {
        return m_highToLow.empty();
    }

    /**
    	取出一个最小的
    */
    int pop()
    {
        int tmpRet  =   m_highToLow.back();
        m_highToLow.pop_back();
        return tmpRet;
    }

    /**
    	重新排一次序
    */
    void reOrder()
    {
        std::function<bool (const int&, const int&)> ff = 
            std::bind(&PriorityQLow::sortFunc, this, std::placeholders::_1, std::placeholders::_2);
        sort(m_highToLow.begin(), m_highToLow.end(), ff);
    }

protected:
    bool sortFunc(const int& m1, const int& m2)
    {
        return m_priorityValues[m1] > m_priorityValues[m2];
    }
};

#endif