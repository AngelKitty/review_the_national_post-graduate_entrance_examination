/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

int shift ( int* A, int n, int s, int k ) { //从A[s]出发，以k为间隔循环左移，O(n / GCD(n, k))
   int bak = A[s]; //备份起始元素
   int i = s, j = ( s + k ) % n; //从该元素出发
   int mov = 0; //移动次数
   while ( s != j ) { //以k为间隔
      A[i] = A[j]; //依次左移k位
      i = j; j = ( j + k ) % n; mov++;
   }
   A[i] = bak; //将起始元素转入对应位置
   return mov + 1;
}