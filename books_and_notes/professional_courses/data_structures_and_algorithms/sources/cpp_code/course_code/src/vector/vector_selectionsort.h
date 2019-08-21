/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> //向量选择排序
void Vector<T>::selectionSort ( Rank lo, Rank hi ) { //assert: 0 < lo <= hi <= size
   /*DSA*/printf ( "\tSELECTIONsort [%3d, %3d)\n", lo, hi );
   while ( lo < --hi )
      swap ( _elem[max ( lo, hi ) ], _elem[hi] ); //将[hi]与[lo, hi]中的最大者交换
}

template <typename T>
Rank Vector<T>::max ( Rank lo, Rank hi ) { //在[lo, hi]内找出最大者
   Rank mx = hi;
   while ( lo < hi-- ) //逆向扫描
      if ( _elem[hi] > _elem[mx] ) //且严格比较
         mx = hi; //故能在max有多个时保证后者优先，进而保证selectionSort稳定
   return mx;
}
