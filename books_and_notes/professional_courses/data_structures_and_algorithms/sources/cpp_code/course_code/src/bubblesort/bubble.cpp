/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "../_share/util.h"
/*DSA*/#include "../UniPrint/print_int_array.h"

void bubblesort ( int A[], int n ) { //起泡排序算法（版本0）：0 <= n
   /*DSA*/int cmp = 0, swp = 0;
   while ( n-- ) { //在尚未确认已全局排序之前，逐趟进行扫描交换
      for ( int i = 0; i < n; i++ ) { //自左向右逐对检查当前范围A[0, n)内的各相邻元素
         if ( A[i] > A[i+1] ) { //一旦A[i]与A[i+1]逆序，则
            swap ( A[i], A[i+1] ); //交换之，并
            /*DSA*/swp++; printf ( "%3d: ", n ); print ( A, n, i, i + 2 );
         }
         /*DSA*/cmp++;
      }
   }
   /*DSA*/printf ( "#comparison = %d, #swap = %d\n", cmp, swp );
} //蛮力算法，不能及时提前退出，总是必须做n-1趟扫描交换