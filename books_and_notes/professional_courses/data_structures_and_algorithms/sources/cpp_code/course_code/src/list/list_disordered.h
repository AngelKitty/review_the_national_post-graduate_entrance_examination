/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

#include "listNode.h"
template <typename T> int List<T>::disordered() const { //统计逆序相邻元素对的总数
   int n = 0; ListNode<T>* p = first();
   for ( int i = 0; i < _size - 1; p = p->succ, i++ )
      if ( p->data > p->succ->data ) n++;
   return n;
}