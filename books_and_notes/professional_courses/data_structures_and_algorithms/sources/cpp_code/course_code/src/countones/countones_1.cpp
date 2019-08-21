/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

int countOnes1 ( unsigned int n ) { //统计整数二进制展开中数位1的总数：O(ones)正比于数位1的总数
   int ones = 0; //计数器复位
   while ( 0 < n ) { //在n缩减至0之前，反复地
      ones++; //计数（至少有一位为1）
      n &= n - 1; //清除当前最靠右的1
   }
   return ones; //返回计数
} //等效于glibc的内置函数int __builtin_popcount (unsigned int n)
