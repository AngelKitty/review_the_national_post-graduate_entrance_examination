/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

inline __int64 sqr ( __int64 a ) { return a * a; }
__int64 power2 ( int n ) { //幂函数2^n算法（优化递归版），n >= 0
   if ( 0 == n ) return 1; //递归基；否则，视n的奇偶分别递归
   return ( n & 1 ) ? sqr ( power2 ( n >> 1 ) ) << 1 : sqr ( power2 ( n >> 1 ) );
} //O(logn) = O(r)，r为输入指数n的比特位数