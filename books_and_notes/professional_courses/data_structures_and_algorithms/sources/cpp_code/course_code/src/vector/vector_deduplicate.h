/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> int Vector<T>::deduplicate() { //删除无序向量中重复元素（高效版）
   int oldSize = _size; //记录原规模
   Rank i = 1; //从_elem[1]开始
   while ( i < _size ) //自前向后逐一考查各元素_elem[i]
      ( find ( _elem[i], 0, i ) < 0 ) ? //在其前缀中寻找与之雷同者（至多一个）
      i++ : remove ( i ); //若无雷同则继续考查其后继，否则删除雷同者
   return oldSize - _size; //向量规模变化量，即被删除元素总数
}