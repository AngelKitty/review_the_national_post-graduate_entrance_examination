/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

int sumI ( int A[], int n ) { //数组求和算法（迭代版）
   int sum = 0; //初始化累计器，O(1)
   for ( int i = 0; i < n; i++ ) //对全部共O(n)个元素，逐一
      sum += A[i]; //累计，O(1)
   return sum; //返回累计值，O(1)
} //O(1) + O(n)*O(1) + O(1) = O(n+2) = O(n)