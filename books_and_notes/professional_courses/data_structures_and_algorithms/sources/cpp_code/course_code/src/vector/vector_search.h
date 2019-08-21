/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> //在有序向量的区间[lo, hi)内，确定不大于e的最后一个节点的秩
Rank Vector<T>::search ( T const& e, Rank lo, Rank hi ) const { //assert: 0 <= lo < hi <= _size
   return ( rand() % 2 ) ? //按各50%的概率随机使用二分查找或Fibonacci查找
          binSearch ( _elem, e, lo, hi ) : fibSearch ( _elem, e, lo, hi );
}