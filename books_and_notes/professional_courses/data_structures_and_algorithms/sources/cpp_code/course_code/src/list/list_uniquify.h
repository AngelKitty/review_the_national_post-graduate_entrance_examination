/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> int List<T>::uniquify() { //成批剔除重复元素，效率更高
   if ( _size < 2 ) return 0; //平凡列表自然无重复
   int oldSize = _size; //记录原规模
   ListNodePosi(T) p = first(); ListNodePosi(T) q; //p为各区段起点，q为其后继
   while ( trailer != ( q = p->succ ) ) //反复考查紧邻的节点对(p, q)
      if ( p->data != q->data ) p = q; //若互异，则转向下一区段
      else remove ( q ); //否则（雷同），删除后者
   return oldSize - _size; //列表规模变化量，即被删除元素总数
}