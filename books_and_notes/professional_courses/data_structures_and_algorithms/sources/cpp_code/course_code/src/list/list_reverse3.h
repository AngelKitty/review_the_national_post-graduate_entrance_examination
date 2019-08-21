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
   ListNodePosi(T) p; ListNodePosi(T) q;
   for ( p = header, q = p->succ; p != trailer; p = q, q = p->succ )
      p->pred = q; //自前向后，依次颠倒各节点的前驱指针
   trailer->pred = NULL; //单独设置尾节点的前驱指针
   for ( p = header, q = p->pred; p != trailer; p = q, q = p->pred )
      q->succ = p; //自前向后，依次颠倒各节点的后继指针
   header->succ = NULL; //单独设置头节点的后继指针
   swap ( header, trailer ); //头、尾节点互换
}