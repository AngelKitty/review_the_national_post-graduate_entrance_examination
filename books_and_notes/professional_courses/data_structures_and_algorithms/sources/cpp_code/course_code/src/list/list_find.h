/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> //在无序列表内节点p（可能是trailer）的n个（真）前驱中，找到等于e的最后者
ListNodePosi(T) List<T>::find ( T const& e, int n, ListNodePosi(T) p ) const {
   while ( 0 < n-- ) //（0 <= n <= rank(p) < _size）对于p的最近的n个前驱，从右向左
      if ( e == ( p = p->pred )->data ) return p; //逐个比对，直至命中或范围越界
   return NULL; //p越出左边界意味着区间内不含e，查找失败
} //失败时，返回NULL