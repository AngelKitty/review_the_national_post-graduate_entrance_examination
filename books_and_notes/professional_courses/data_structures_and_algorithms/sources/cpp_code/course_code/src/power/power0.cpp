/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

__int64 powerBF ( __int64 a, int n ) { //a^nËã·¨£¨ÂùÁ¦°æ£©£ºn >= 0
   __int64 pow = 1; //O(1)
   while ( 0 < n ) { //O(n)
      pow *= a; //O(1)
      n--; //O(1)
   }
   return pow; //O(1)
} //powerBF()