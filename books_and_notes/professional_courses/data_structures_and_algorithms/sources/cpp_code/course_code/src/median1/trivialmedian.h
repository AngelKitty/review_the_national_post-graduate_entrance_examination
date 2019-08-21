/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

// 中位数算法蛮力版：效率低，仅适用于max(n1, n2)较小的情况
template <typename T> //子向量S1[lo1, lo1 + n1)和S2[lo2, lo2 + n2)分别有序，数据项可能重复
T trivialMedian ( Vector<T>& S1, int lo1, int n1, Vector<T>& S2, int lo2, int n2 ) {
   /*DSA*/printf ( "trivial median\n" );
   int hi1 = lo1 + n1, hi2 = lo2 + n2;
   Vector<T> S; //将两个有序子向量归并为一个有序向量
   while ( ( lo1 < hi1 ) && ( lo2 < hi2 ) ) {
      while ( ( lo1 < hi1 ) && S1[lo1] <= S2[lo2] ) S.insert ( S1[lo1 ++] );
      while ( ( lo2 < hi2 ) && S2[lo2] <= S1[lo1] ) S.insert ( S2[lo2 ++] );
   }
   while ( lo1 < hi1 ) S.insert ( S1[lo1 ++] );
   while ( lo2 < hi2 ) S.insert ( S1[lo2 ++] ); /*DSA*/print ( S );
   return S[ ( n1 + n2 ) / 2]; //直接返回归并向量的中位数
}