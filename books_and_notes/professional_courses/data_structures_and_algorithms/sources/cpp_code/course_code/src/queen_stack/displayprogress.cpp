/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#include "queen_stack.h"

int N = 0; //棋盘大小

void displayRow ( Queen& q ) { //打印当前皇后（放置于col列）所在行
   printf ( "%2d: ", q.x );
   int i = 0;
   while ( i++ < q.y ) printf ( "[]" );
   printf ( "█" );
   while ( i++ < N ) printf ( "[]" );
   printf ( "%2d\n", q.y );
}

void displayProgress ( Stack<Queen>& S, int nQueen ) { //在棋盘上显示搜查的进展
   system ( "cls" );
   N = nQueen; S.traverse ( displayRow );
   if ( nQueen <= S.size() )
      cout  << nSolu << " solution(s) found after " << nCheck << " check(s)\a";
   getchar();
}