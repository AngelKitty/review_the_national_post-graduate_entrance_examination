/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "../string_pm/string_pm_test.h"
/*DSA*/#include "pm_kr.h"

int match ( char* P, char* T ) { //串匹配算法（Karp-Rabin）
   size_t m = strlen ( P ), n = strlen ( T ); //assert: m <= n
   HashCode Dm = prepareDm ( m ), hashP = 0, hashT = 0;
   for ( size_t i = 0; i < m; i++ ) { //初始化
      hashP = ( hashP * R + DIGIT ( P, i ) ) % M; //计算模式串对应的散列值
      hashT = ( hashT * R + DIGIT ( T, i ) ) % M; //计算文本串（前m位）的初始散列值
   } /*DSA*/printf ( "hashP = %I64d", hashP ); getchar();
   for ( size_t k = 0; ; ) { //查找
      /*DSA*/showProgressOfKR ( T, P, k );
      if ( hashT == hashP ) /*DSA*/{ printf ( "Hash matched\n" );
         if ( check1by1 ( P, T, k ) ) return k;
      /*DSA*/}
      if ( ++k > n - m ) return k; //assert: k > n - m，表示无匹配
      else updateHash ( hashT, T, m, k, Dm ); //否则，更新子串散列码，继续查找
   }
}