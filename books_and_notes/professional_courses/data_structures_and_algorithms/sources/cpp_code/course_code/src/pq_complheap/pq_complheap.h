/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

#include "../Vector/Vector.h" //借助多重继承机制，基于向量
#include "../PQ/PQ.h" //按照优先级队列ADT实现的
template <typename T> class PQ_ComplHeap : public PQ<T>, public Vector<T> { //完全二叉堆
   /*DSA*/friend class UniPrint; //演示输出使用，否则不必设置友类
protected:
   Rank percolateDown ( Rank n, Rank i ); //下滤
   Rank percolateUp ( Rank i ); //上滤
   void heapify ( Rank n ); //Floyd建堆算法
public:
   PQ_ComplHeap() { } //默认构造
   PQ_ComplHeap ( T* A, Rank n ) { copyFrom ( A, 0, n ); heapify ( n ); } //批量构造
   void insert ( T ); //按照比较器确定的优先级次序，插入词条
   T getMax(); //读取优先级最高的词条
   T delMax(); //删除优先级最高的词条
}; //PQ_ComplHeap
/*DSA*/#include "PQ_ComplHeap_macro.h"

#include "PQ_ComplHeap_implementation.h"
