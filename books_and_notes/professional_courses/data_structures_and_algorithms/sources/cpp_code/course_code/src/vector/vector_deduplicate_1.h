/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> int Vector<T>::deduplicate() { //删除无序向量中重复元素（繁琐版）
   int oldSize = _size; //记录原规模
   int i = -1; //从最前端开始
   while ( ++i < _size - 1 ) { //从前向后，逐一
      int j = i + 1; //assert: _elem[0, i]中不含重复元素
      while ( j < _size )
         if ( _elem[i] == _elem[j] ) remove ( j ); //若雷同，则删除后者
         else j++; //并继续扫描
   }
   return oldSize - _size; //向量规模变化量，即被删除元素总数
}