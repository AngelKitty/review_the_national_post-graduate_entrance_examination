/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

//对向量前n个词条中的第i个实施下滤，i < n
template <typename T> Rank PQ_ComplHeap<T>::percolateDown ( Rank n, Rank i ) {
   Rank j; //i及其（至多两个）孩子中，堪为父者
   while ( i != ( j = ProperParent ( _elem, n, i ) ) ) //只要i非j，则
      { swap ( _elem[i], _elem[j] ); i = j; } //二者换位，并继续考查下降后的i
   return i; //返回下滤抵达的位置（亦i亦j）
}
