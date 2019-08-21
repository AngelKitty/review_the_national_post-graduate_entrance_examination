/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> void quickSelect ( Vector<T> & A, Rank k ) { //基于快速划分的k选取算法
   for ( Rank lo = 0, hi = A.size() - 1; lo < hi; ) {
      Rank i = lo, j = hi; T pivot = A[lo];
      while ( i < j ) { //O(hi - lo + 1) = O(n)
         while ( ( i < j ) && ( pivot <= A[j] ) ) j--; A[i] = A[j];
         while ( ( i < j ) && ( A[i] <= pivot ) ) i++; A[j] = A[i];
      } //assert: i == j
      A[i] = pivot;
      if ( k <= i ) hi = i - 1;
      if ( i <= k ) lo = i + 1;
      /*DSA*/print ( A ); for ( int i = 0; i < lo; i++ ) printf ( "     " ); for ( int i = lo; i <= hi; i++ ) printf ( "-----" ); printf ( "\n" );
   } //A[k] is now a pivot
}
