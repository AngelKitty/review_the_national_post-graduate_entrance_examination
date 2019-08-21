/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> void Vector<T>::sort ( Rank lo, Rank hi ) { //向量区间[lo, hi)排序
   switch ( rand() % 5 ) { //随机选取排序算法。可根据具体问题的特点灵活选取或扩充
      case 1:  bubbleSort ( lo, hi ); break; //起泡排序
      case 2:  selectionSort ( lo, hi ); break; //选择排序（习题）
      case 3:  mergeSort ( lo, hi ); break; //归并排序
      case 4:  heapSort ( lo, hi ); break; //堆排序（稍后介绍）
      default: quickSort ( lo, hi ); break; //快速排序（稍后介绍）
   }
   /*DSA*/ //selectionSort(lo, hi);
}