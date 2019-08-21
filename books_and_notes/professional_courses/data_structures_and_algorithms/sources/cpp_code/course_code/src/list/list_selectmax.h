/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> //从起始于位置p的n个元素中选出最大者
ListNodePosi(T) List<T>::selectMax ( ListNodePosi(T) p, int n ) {
   ListNodePosi(T) max = p; //最大者暂定为首节点p
   for ( ListNodePosi(T) cur = p; 1 < n; n-- ) //从首节点p出发，将后续节点逐一与max比较
      if ( !lt ( ( cur = cur->succ )->data, max->data ) ) //若当前元素不小于max，则
         max = cur; //更新最大元素位置记录
   return max; //返回最大节点位置
}