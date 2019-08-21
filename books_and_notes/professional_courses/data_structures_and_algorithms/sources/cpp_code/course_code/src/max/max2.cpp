/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

int maxR ( int A[], int lo, int hi ) { //计算数组区间A[lo, hi)的最大值（二分递归）
   if ( lo + 1 == hi ) //如遇递归基（区间长度已降至1），则
      return A[lo]; //直接返回该元素
   else { //否则（一般情况下lo + 1 < hi），则递归地
      int mi = ( lo + hi ) >> 1; //以中位单元为界，将原区间一分为二：A[lo, mi)和A[mi, hi)
      return max ( maxR ( A, lo, mi ), maxR ( A, mi, hi ) ); //计算子区间的最大值，再从中取大者
   }
} //O(hi - lo)，线性正比于区间的长度
