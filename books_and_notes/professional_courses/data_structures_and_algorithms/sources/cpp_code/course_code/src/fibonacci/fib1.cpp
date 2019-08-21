/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

__int64 fib ( int n ) { //计算Fibonacci数列的第n项（二分递归版）：O(2^n)
   return ( 2 > n ) ?
          ( __int64 ) n //若到达递归基，直接取值
          : fib ( n - 1 ) + fib ( n - 2 ); //否则，递归计算前两项，其和即为正解
}