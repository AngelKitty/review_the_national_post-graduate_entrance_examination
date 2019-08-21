/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> BinNodePosi(T) Splay<T>::insert ( const T& e ) { //将关键码e插入伸展树中
   if ( !_root ) { _size++; return _root = new BinNode<T> ( e ); } //处理原树为空的退化情况
   if ( e == search ( e )->data ) return _root; //确认目标节点不存在
   _size++; BinNodePosi(T) t = _root; //创建新节点。以下调整<=7个指针以完成局部重构
   if ( _root->data < e ) { //插入新根，以t和t->rc为左、右孩子
      t->parent = _root = new BinNode<T> ( e, NULL, t, t->rc ); //2 + 3个
      if ( HasRChild ( *t ) ) { t->rc->parent = _root; t->rc = NULL; } //<= 2个
   } else { //插入新根，以t->lc和t为左、右孩子
      t->parent = _root = new BinNode<T> ( e, NULL, t->lc, t ); //2 + 3个
      if ( HasLChild ( *t ) ) { t->lc->parent = _root; t->lc = NULL; } //<= 2个
   }
   updateHeightAbove ( t ); //更新t及其祖先（实际上只有_root一个）的高度
   return _root; //新节点必然置于树根，返回之
} //无论e是否存在于原树中，返回时总有_root->data == e
