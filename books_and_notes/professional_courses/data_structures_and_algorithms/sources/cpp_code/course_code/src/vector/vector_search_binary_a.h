/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

// 二分查找算法（版本A）：在有序向量的区间[lo, hi)内查找元素e，0 <= lo <= hi <= _size
template <typename T> static Rank binSearch ( T* A, T const& e, Rank lo, Rank hi ) {
   /*DSA*/printf ( "BIN search (A)\n" );
   while ( lo < hi ) { //每步迭代可能要做两次比较判断，有三个分支
      /*DSA*/ for ( int i = 0; i < lo; i++ ) printf ( "     " ); if ( lo >= 0 ) for ( int i = lo; i < hi; i++ ) printf ( "....^" ); printf ( "\n" );
      Rank mi = ( lo + hi ) >> 1; //以中点为轴点
      if      ( e < A[mi] ) hi = mi; //深入前半段[lo, mi)继续查找
      else if ( A[mi] < e ) lo = mi + 1; //深入后半段(mi, hi)继续查找
      else                return mi; //在mi处命中
      /*DSA*/ if ( lo >= hi ) { for ( int i = 0; i < mi; i++ ) printf ( "     " ); if ( mi >= 0 ) printf ( "....|\n" ); else printf ( "<<<<|\n" ); }
   } //成功查找可以提前终止
   return -1; //查找失败
} //有多个命中元素时，不能保证返回秩最大者；查找失败时，简单地返回-1，而不能指示失败的位置
