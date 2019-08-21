/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#include "queen.h"

void displaySolution ( int* solu, int n ) { //输出n*n的可行布局
   ( Step == runMode ) ? system ( "cls" ) : printf ( "--\n" );
   for ( int i = 0; i < n; i++ ) {
      for ( int j = 0; j < n; j++ ) {
         printf ( ( j == solu[i] ) ? "█" : "[]" );
      }
      printf ( "\n" );
   }
   if ( Step == runMode ) {
      cout  << nSolu << " solution(s) found after " << nCheck << " check(s)\a";
      getchar();
   } else
      printf ( "\n" );
}