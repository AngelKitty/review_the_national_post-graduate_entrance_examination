/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

__int64 gcdEU_R ( __int64 a, __int64 b ) { //assert: 0 <= min(a, b)
   return ( 0 == b ) ? a : gcdEU_R ( b, a % b );
}

__int64 gcdEU ( __int64 a, __int64 b ) { //assert: 0 <= min(a, b)
   while ( true ) {
      if ( 0 == a ) return b;
      if ( 0 == b ) return a;
      if ( a > b ) a %= b;
      else       b %= a;
   }
}
