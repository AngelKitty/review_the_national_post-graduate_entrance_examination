/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

int sum ( int A[], int n ) { //数组求和算法（线性递归版）
   if ( 1 > n ) //平凡情况，递归基
      return 0; //直接（非递归式）计算
   else //一般情况
      return sum ( A, n - 1 ) + A[n - 1]; //递归：前n - 1项之和，再累计第n - 1项
} //O(1)*递归深度 = O(1)*(n + 1) = O(n)