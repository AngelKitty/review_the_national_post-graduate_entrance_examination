/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> void PQ_ComplHeap<T>::heapify ( Rank n ) { //Floyd建堆算法，O(n)时间
   for ( int i = LastInternal ( n ); InHeap ( n, i ); i-- ) //自底而上，依次
/*DSA*/{
      percolateDown ( n, i ); //下滤各内部节点
/*DSA*/for ( int k = 0; k < n; k++ ) {
/*DSA*/  int kk = k; while ( i < kk ) kk = (kk - 1) / 2;
/*DSA*/  i == kk ? print(_elem[k]) : print("    " );
/*DSA*/}; printf("\n");
/*DSA*/}
}
