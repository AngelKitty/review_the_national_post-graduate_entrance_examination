/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

__int64 power2BF_I ( int n ) { //幂函数2^n算法（蛮力迭代版），n >= 0
   __int64 pow = 1; //O(1)：累积器初始化为2^0
   while ( 0 < n -- ) //O(n)：迭代n轮，每轮都
      pow <<= 1; //O(1)：将累积器翻倍
   return pow; //O(1)：返回累积器
} //O(n) = O(2^r)，r为输入指数n的比特位数