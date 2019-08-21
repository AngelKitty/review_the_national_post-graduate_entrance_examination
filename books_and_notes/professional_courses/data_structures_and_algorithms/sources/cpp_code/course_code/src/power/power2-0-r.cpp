/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

__int64 power2BF ( int n ) { //幂函数2^n算法（蛮力递归版），n >= 0
   return ( 1 > n ) ? 1 : power2BF ( n - 1 ) << 1; //递归
} //O(n) = O(2^r)，r为输入指数n的比特位数