/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> void List<T>::reverse() { //前后倒置
   ListNodePosi(T) p = header; ListNodePosi(T) q = trailer; //头、尾节点
   for ( int i = 1; i < _size; i += 2 ) //（从首、末节点开始）由外而内，捉对地
      swap ( ( p = p->succ )->data, ( q = q->pred )->data ); //交换对称节点的数据项
}