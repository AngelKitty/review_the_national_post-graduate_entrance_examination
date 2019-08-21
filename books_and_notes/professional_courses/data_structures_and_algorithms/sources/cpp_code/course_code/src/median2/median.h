/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> //向量S1[lo1, lo1 + n1)和S2[lo2, lo2 + n2)分别有序，数据项可能重复
T median ( Vector<T>& S1, int lo1, int n1, Vector<T>& S2, int lo2, int n2 ) { //中位数算法
   if ( n1 > n2 ) return median ( S2, lo2, n2, S1, lo1, n1 ); //确保n1 <= n2
   /*DSA*/for ( int i = 0; i < lo1; i++ ) printf ( "    ." ); for ( int i = 0; i < n1; i++ ) print ( S1[lo1+i] ); for ( int i = lo1 + n1; i < S1.size(); i++ ) printf ( "    ." ); printf ( "\n" );
   /*DSA*/for ( int i = 0; i < lo2; i++ ) printf ( "    ." ); for ( int i = 0; i < n2; i++ ) print ( S2[lo2+i] ); for ( int i = lo2 + n2; i < S2.size(); i++ ) printf ( "    ." );  printf ( "\n--\n" );
   if ( n2 < 6 ) //递归基：1 <= n1 <= n2 <= 5
      return trivialMedian ( S1, lo1, n1, S2, lo2, n2 );
   ///////////////////////////////////////////////////////////////////////
   //                lo1            lo1 + n1/2      lo1 + n1 - 1
   //                 |                 |                 |
   //                 X >>>>>>>>>>>>>>> X >>>>>>>>>>>>>>> X
   // Y .. trimmed .. Y >>>>>>>>>>>>>>> Y >>>>>>>>>>>>>>> Y .. trimmed .. Y
   // |               |                 |                 |               |
   // lo2     lo2 + (n2-n1)/2       lo2 + n2/2     lo2 + (n2+n1)/2    lo2 + n2 -1
   ///////////////////////////////////////////////////////////////////////
   if ( 2 * n1 < n2 ) //若两个向量的长度相差悬殊，则长者（S2）的两翼可直接截除
      return median ( S1, lo1, n1, S2, lo2 + ( n2 - n1 - 1 ) / 2, n1 + 2 - ( n2 - n1 ) % 2 );
   ///////////////////////////////////////////////////////////////////////
   //    lo1                  lo1 + n1/2              lo1 + n1 - 1
   //     |                       |                       |
   //     X >>>>>>>>>>>>>>>>>>>>> X >>>>>>>>>>>>>>>>>>>>> X
   //                             |
   //                            m1
   ///////////////////////////////////////////////////////////////////////
   //                            mi2b
   //                             |
   // lo2 + n2 - 1         lo2 + n2 - 1 - n1/2
   //     |                       |
   //     Y <<<<<<<<<<<<<<<<<<<<< Y ...
   //                                .
   //                               .
   //                              .
   //                             .
   //                            .
   //                           .
   //                          .
   //                         ... Y <<<<<<<<<<<<<<<<<<<<< Y
   //                             |                       |
   //                       lo2 + (n1-1)/2               lo2
   //                             |
   //                            mi2a
   ///////////////////////////////////////////////////////////////////////
   int mi1  = lo1 + n1 / 2;
   int mi2a = lo2 + ( n1 - 1 ) / 2;
   int mi2b = lo2 + n2 - 1 - n1 / 2;
   if ( S1[mi1] > S2[mi2b] ) //取S1左半、S2右半
      return median ( S1, lo1, n1 / 2 + 1, S2, mi2a, n2 - ( n1 - 1 ) / 2 );
   else if ( S1[mi1] < S2[mi2a] ) //取S1右半、S2左半
      return median ( S1, mi1, ( n1 + 1 ) / 2, S2, lo2, n2 - n1 / 2 );
   else //S1保留，S2左右同时缩短
      return median ( S1, lo1, n1, S2, mi2a, n2 - ( n1 - 1 ) / 2 * 2 );
}