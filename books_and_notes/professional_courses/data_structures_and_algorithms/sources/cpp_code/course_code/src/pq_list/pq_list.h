/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

#include "../PQ/PQ.h" //引入优先级队列ADT
#include "../List/List.h" //引入列表
template <typename T> class PQ_List : public PQ<T>, public List<T> { //基于列表实现的优先级队列
   /*DSA*/friend class UniPrint; //演示输出使用，否则不必设置友类
public:
   PQ_List() { } //默认构造
   PQ_List ( T* E, int n ) { while ( 0 < n-- ) insertAsFirst ( ( E[n] ) ); } //批量构造
   void insert ( T e ) { insertAsLast ( e ); } //直接将新元素插至队列末尾
   T getMax() { return selectMax()->data; } //取出优先级最高的元素
   T delMax() { return remove ( selectMax() ); } //删除优先级最高的元素
}; //PQ_List
