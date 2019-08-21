/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

__int64 power ( __int64 a, int n ) { //a^nËã·¨£ºn >= 0
   __int64 pow = 1; //O(1)
   __int64 p = a; //O(1)
   while ( 0 < n ) { //O(logn)
      if ( n & 1 ) //O(1)
         pow *= p; //O(1)
      n >>= 1; //O(1)
      p *= p; //O(1)
   }
   return pow; //O(1)
} //power()
