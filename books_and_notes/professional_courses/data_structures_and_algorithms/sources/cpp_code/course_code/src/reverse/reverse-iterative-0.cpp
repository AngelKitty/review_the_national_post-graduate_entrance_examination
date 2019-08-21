/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "../_share/util.h"
/*DSA*/#include "../UniPrint/print_int_array.h"

void reverse ( int* A, int lo, int hi ) { //数组倒置（直接改造而得的迭代版）
next: //算法起始位置添加跳转标志
   if ( lo < hi ) {
      swap ( A[lo], A[hi] ); //交换A[lo]和A[hi]
      lo++; hi--; //收缩待倒置区间
      goto next; //跳转至算法体的起始位置，迭代地倒置A(lo, hi)
   } //else隐含了迭代的终止
} //O(hi - lo + 1)