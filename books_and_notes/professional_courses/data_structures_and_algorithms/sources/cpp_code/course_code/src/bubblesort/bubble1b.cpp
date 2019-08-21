/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "../_share/util.h"
/*DSA*/#include "../UniPrint/print_int_array.h"

void bubblesort1B ( int A[], int n ) { //起泡排序算法（版本1B，与版本1A完全等价）：0 <= n
   /*DSA*/int cmp = 0, swp = 0;
   for ( bool sorted = false; sorted = !sorted; n-- ) { //在尚未确认已全局排序之前，逐趟进行扫描交换
      for ( int i = 1; i < n; i++ ) { //自左向右逐对检查当前范围A[0, n)内的各相邻元素
         if ( A[i-1] > A[i] ) { //一旦A[i-1]与A[i]逆序，则
            swap ( A[i-1], A[i] ); //交换之，并
            sorted = false; //因整体排序不能保证，需要清除排序标志
            /*DSA*/swp++; printf ( "%3d: ", n ); print ( A, n, i - 1, i + 1 );
         }
         /*DSA*/cmp++;
      }
   }
   /*DSA*/printf ( "#comparison = %d, #swap = %d\n", cmp, swp );
} //借助布尔型标志位sorted，可及时提前退出，而不致蛮力地做n-1趟扫描交换
//对尾部有序（或接近有序）的输入，算法依然亦步亦趋地收敛，导致元素比较次数过多