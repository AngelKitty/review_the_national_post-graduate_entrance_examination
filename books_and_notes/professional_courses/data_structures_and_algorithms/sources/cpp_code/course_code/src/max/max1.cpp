/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

int maxR ( int A[], int n ) { //数组求最大值算法（线性递归版）
   if ( 2 > n ) //平凡情况，递归基
      return A[n - 1]; //直接（非递归式）计算
   else //一般情况，递归：在前n - 1项中的最大值与第n - 1项之间，取大者
      return max ( maxR ( A, n - 1 ), A[n - 1] );
} //O(1) * 递归深度 = O(1) * (n + 1) = O(n)
