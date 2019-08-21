/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "../_share/util.h"
/*DSA*/#include "../UniPrint/print_int_array.h"

void reverse ( int* A, int lo, int hi ) { //Êý×éµ¹ÖÃ£¨¶àµÝ¹é»ùµÝ¹é°æ£©
   if ( lo < hi ) {
      swap ( A[lo], A[hi] ); //½»»»A[lo]ºÍA[hi]
      reverse ( A, lo + 1, hi - 1 ); //µÝ¹éµ¹ÖÃA(lo, hi)
   } //elseÒþº¬ÁËÁ½ÖÖµÝ¹é»ù
} //O(hi - lo + 1)