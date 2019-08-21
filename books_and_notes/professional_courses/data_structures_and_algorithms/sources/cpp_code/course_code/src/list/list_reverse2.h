/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> void List<T>::reverse() { //前后倒置
   if ( _size < 2 ) return; //平凡情况
   for ( ListNodePosi(T) p = header; p; p = p->pred ) //自前向后，依次
      swap ( p->pred, p->succ ); //交换各节点的前驱、后继指针
   swap ( header, trailer ); //头、尾节点互换
}