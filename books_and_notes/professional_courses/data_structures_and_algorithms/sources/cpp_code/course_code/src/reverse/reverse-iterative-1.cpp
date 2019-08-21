/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "../_share/util.h"
/*DSA*/#include "../UniPrint/print_int_array.h"

void reverse ( int* A, int lo, int hi ) { //数组倒置（规范整理之后的迭代版）
   while ( lo < hi ) //用while替换跳转标志和if，完全等效
      swap ( A[lo++], A[hi--] ); //交换A[lo]和A[hi]，收缩待倒置区间
} //O(hi - lo + 1)