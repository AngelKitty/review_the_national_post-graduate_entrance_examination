/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "../string_pm/string_pm_test.h"

int* buildSS ( char* P ) { //构造最大匹配后缀长度表：O(m)
   int m = strlen ( P ); int* ss = new int[m]; //Suffix Size表
   ss[m - 1]  =  m; //对最后一个字符而言，与之匹配的最长后缀就是整个P串
// 以下，从倒数第二个字符起自右向左扫描P，依次计算出ss[]其余各项
   for ( int lo = m - 1, hi = m - 1, j = lo - 1; j >= 0; j -- )
      if ( ( lo < j ) && ( ss[m - hi + j - 1] <= j - lo ) ) //情况一
         ss[j] =  ss[m - hi + j - 1]; //直接利用此前已计算出的ss[]
      else { //情况二
         hi = j; lo = __min ( lo, hi );
         while ( ( 0 <= lo ) && ( P[lo] == P[m - hi + lo - 1] ) ) //二重循环？
            lo--; //逐个对比处于(lo, hi]前端的字符
         ss[j] = hi - lo;
      }
   /*DSA*/printf ( "-- ss[] Table -------\n" );
   /*DSA*/for ( int i = 0; i < m; i ++ ) printf ( "%4d", i ); printf ( "\n" );
   /*DSA*/printString ( P ); printf ( "\n" );
   /*DSA*/for ( int i = 0; i < m; i ++ ) printf ( "%4d", ss[i] ); printf ( "\n\n" );
   return ss;
}

int* buildGS ( char* P ) { //构造好后缀位移量表：O(m)
   int* ss = buildSS ( P ); //Suffix Size table
   size_t m = strlen ( P ); int* gs = new int[m]; //Good Suffix shift table
   for ( size_t j = 0; j < m; j ++ ) gs[j] = m; //初始化
   for ( size_t i = 0, j = m - 1; j < UINT_MAX; j -- ) //逆向逐一扫描各字符P[j]
      if ( j + 1 == ss[j] ) //若P[0, j] = P[m - j - 1, m)，则
         while ( i < m - j - 1 ) //对于P[m - j - 1]左侧的每个字符P[i]而言（二重循环？）
            gs[i++] = m - j - 1; //m - j - 1都是gs[i]的一种选择
   for ( size_t j = 0; j < m - 1; j ++ ) //画家算法：正向扫描P[]各字符，gs[j]不断递减，直至最小
      gs[m - ss[j] - 1] = m - j - 1; //m - j - 1必是其gs[m - ss[j] - 1]值的一种选择
   /*DSA*/printGS ( P, gs );
   delete [] ss; return gs;
}
