/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "../_share/util.h"
/*DSA*/#include "../UniPrint/print_int_array.h"

void bubblesort2 ( int A[], int n ) { //起泡排序算法（版本2）：0 <= n
   /*DSA*/int cmp = 0, swp = 0;
   for ( int m; 1 < n; n = m ) { //在尚未确认已全局排序之前，逐趟进行扫描交换
      for ( int i = m = 1; i < n; i++ ) { //自左向右逐对检查当前范围A[0, m)内的各相邻元素
         if ( A[i-1] > A[i] ) { //一旦A[i-1]与A[i]逆序，则
            swap ( A[i-1], A[i] ); //交换之，并
            m = i; //更新待排序区间的长度
            /*DSA*/swp++; printf ( "%3d: ", n ); print ( A, n, i - 1, i + 1 );
         }
         /*DSA*/cmp++;
      }
   }
   /*DSA*/printf ( "#comparison = %d, #swap = %d\n", cmp, swp );
} //借助整数m尽快地收缩待排序区间：既可提前退出，更可减少每趟（及所有）扫描交换中元素比较操作
//对尾部有序（或接近有序）的输入，算法收敛的速度大大提高
//元素交换的次数仅取决于输入序列，与版本0和版本1相同