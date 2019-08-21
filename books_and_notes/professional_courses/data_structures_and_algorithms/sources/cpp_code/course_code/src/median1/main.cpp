/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../_share/util.h"
#include "../UniPrint/print.h"
#include "../vector/vector.h"
#include "../random/randomSortedVector.h"
#include "trivialMedian.h"
#include "median.h"

/******************************************************************************************
 * 中位数算法测试
 ******************************************************************************************/
int main ( int argc, char* argv[] ) {
// 解析命令行
   if ( 7 > argc ) { printf ( "Usage:\n\t%s <#tests> <n> <min1> <max1> <min2> <max2>\a\a\n", argv[0] ); return 1; }
   int nTest = atoi ( argv[1] ); //测试轮数
   int n = atoi ( argv[2] ); //向量长度
   int min1 = atoi ( argv[3] ), max1 = atoi ( argv[4] ); //数值范围
   int min2 = atoi ( argv[5] ), max2 = atoi ( argv[6] ); //数值范围
   if ( ( nTest < 1 ) || ( n < 1 ) || ( max1 + 1 < min1 + n ) || ( max2 + 1 < min2 + n ) )
      { printf ( "arguments error\a\a\n" ); return 1; }
// 随机种子
   srand ( ( unsigned int ) time ( NULL ) );
// 每次测试100组
   for ( int i = 0; i < nTest; i++ ) {
      // 随机生成向量
      Vector<int> A1 = randomSortedVector ( n, min1, max1 ); print ( A1 );
      Vector<int> A2 = randomSortedVector ( n, min2, max2 ); print ( A2 );
      // 计算中位数，并与蛮力算法（更不易出错）的结果对比
      int m1 = median ( A1, 0, A2, 0, n );
      int m2 = trivialMedian ( A1, 0, n, A2, 0, n );
      if ( m1 == m2 ) { printf ( "\nmedian: %d = %d\a\n\n\n", m1, m2 ); }
      else          { printf ( "\nmedian: %d <Error> %d\a\a\n\n\n", m1, m2 ); getchar(); }
   }
   // 返回
   return 0;
}