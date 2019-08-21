/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

#include "..\fibonacci\Fib.h" //引入Fib数列类
// Fibonacci查找算法（版本A）：在有序向量的区间[lo, hi)内查找元素e，0 <= lo <= hi <= _size
template <typename T> static Rank fibSearch ( T* A, T const& e, Rank lo, Rank hi ) {
   /*DSA*/printf ( "FIB search (A)\n" );
   Fib fib ( hi - lo ); //用O(log_phi(n = hi - lo)时间创建Fib数列
   while ( lo < hi ) { //每步迭代可能要做两次比较判断，有三个分支
      /*DSA*/ for ( int i = 0; i < lo; i++ ) printf ( "     " ); if ( lo >= 0 ) for ( int i = lo; i < hi; i++ ) printf ( "....^" ); else printf ( "<<<<|" ); printf ( "\n" );
      while ( hi - lo < fib.get() ) fib.prev(); //通过向前顺序查找（分摊O(1)）——至多迭代几次？
      Rank mi = lo + fib.get() - 1; //确定形如Fib(k) - 1的轴点
      if      ( e < A[mi] ) hi = mi; //深入前半段[lo, mi)继续查找
      else if ( A[mi] < e ) lo = mi + 1; //深入后半段(mi, hi)继续查找
      else                return mi; //在mi处命中
      /*DSA*/ if ( lo >= hi ) { for ( int i = 0; i < mi; i++ ) printf ( "     " ); if ( mi >= 0 ) printf ( "....|\n" ); else printf ( "<<<<|\n" ); }
   } //成功查找可以提前终止
   return -1; //查找失败
} //有多个命中元素时，不能保证返回秩最大者；失败时，简单地返回-1，而不能指示失败的位置