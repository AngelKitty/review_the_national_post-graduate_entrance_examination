/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "../UniPrint/print_int_array.h"
/*DSA*/int shift ( int* A, int n, int s, int t ); //从A[s]出发，以t为间隔循环左移，O(3n)

int shift0 ( int* A, int n, int k ) { //蛮力地将数组循环左移k位，O(nk)
   if ( k < 1 ) return 0; int mov = 0;
   while ( k-- ) { //反复以1为间隔循环左移，共迭代k次
      mov += shift ( A, n, 0, 1 );                                       /*DSA*/printf ( "== move *%3d\n", mov ); print ( A, n );
   }
   return mov;
}