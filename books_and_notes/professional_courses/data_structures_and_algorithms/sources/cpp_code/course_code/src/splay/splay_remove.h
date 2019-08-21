/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> bool Splay<T>::remove ( const T& e ) { //从伸展树中删除关键码e
   if ( !_root || ( e != search ( e )->data ) ) return false; //若树空或目标不存在，则无法删除
   BinNodePosi(T) w = _root; //assert: 经search()后节点e已被伸展至树根
   if ( !HasLChild ( *_root ) ) { //若无左子树，则直接删除
      _root = _root->rc; if ( _root ) _root->parent = NULL;
   } else if ( !HasRChild ( *_root ) ) { //若无右子树，也直接删除
      _root = _root->lc; if ( _root ) _root->parent = NULL;
   } else { //若左右子树同时存在，则
      BinNodePosi(T) lTree = _root->lc;
      lTree->parent = NULL; _root->lc = NULL; //暂时将左子树切除
      _root = _root->rc; _root->parent = NULL; //只保留右子树
      search ( w->data ); //以原树根为目标，做一次（必定失败的）查找
///// assert: 至此，右子树中最小节点必伸展至根，且（因无雷同节点）其左子树必空，于是
      _root->lc = lTree; lTree->parent = _root; //只需将原左子树接回原位即可
   }
   release ( w->data ); release ( w ); _size--; //释放节点，更新规模
   if ( _root ) updateHeight ( _root ); //此后，若树非空，则树根的高度需要更新
   return true; //返回成功标志
} //若目标节点存在且被删除，返回true；否则返回false
