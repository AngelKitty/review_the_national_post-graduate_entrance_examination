/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#include <stdio.h>
#include "../_share/rand.h"
#include "../Bitmap/Bitmap.h"
#include "Eratosthenes.h"
#include "primeNLT.h"

/******************************************************************************************
 * 主函数
 ******************************************************************************************/
#define PRIMEFILE "prime-bitmap.txt" //用于记录指定区间内素数分布的比特图文件
int main ( int argc, char* argv[] ) {
   srand ( ( unsigned int ) time ( NULL ) ); //设置随机种子
   if ( 2 > argc ) { printf ( "Usage: %s <maxInt>\n", argv[0] ); return -1; }
   int n = atoi ( argv[1] ); //简化起见，不考虑非正的情况
   Eratosthenes ( n, PRIMEFILE );
   for ( int i = 0; i < 13; i++ ) { //做13次随机测试（限于MAX_RAND，覆盖范围不超过0x7FFF）
      int low = dice ( n );
      printf ( "The smallest prime number no less than %8d (%05X) is %8d\n", low, low, primeNLT ( low, n, PRIMEFILE ) );
   }
   for ( int i = 0; i < 13; i++ ) { //做13次等间距测试
      printf ( "The smallest prime number no less than %8d (%05X) is %8d\n", n * i / 13, n * i / 13, primeNLT ( n * i / 13, n, PRIMEFILE ) );
   }
   return 0;
}