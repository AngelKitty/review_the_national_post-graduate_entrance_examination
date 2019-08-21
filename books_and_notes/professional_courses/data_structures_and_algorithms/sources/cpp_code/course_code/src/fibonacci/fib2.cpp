/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

__int64 fib ( int n, __int64& prev ) { //计算Fibonacci数列第n项（线性递归版）：入口形式fib(n, prev)
   if ( 0 == n ) //若到达递归基，则
      { prev = 1; return 0; } //直接取值：fib(-1) = 1, fib(0) = 0
   else { //否则
      __int64 prevPrev; prev = fib ( n - 1, prevPrev ); //递归计算前两项
      return prevPrev + prev; //其和即为正解
   }
} //用辅助变量记录前一项，返回数列的当前项，O(n)