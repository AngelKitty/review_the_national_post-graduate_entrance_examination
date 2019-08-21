/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> //关键码插入后若节点上溢，则做节点分裂处理
void BTree<T>::solveOverflow ( BTNodePosi(T) v ) {
   if ( _order >= v->child.size() ) return; //递归基：当前节点并未上溢
   Rank s = _order / 2; //轴点（此时应有_order = key.size() = child.size() - 1）
   BTNodePosi(T) u = new BTNode<T>(); //注意：新节点已有一个空孩子
   for ( Rank j = 0; j < _order - s - 1; j++ ) { //v右侧_order-s-1个孩子及关键码分裂为右侧节点u
      u->child.insert ( j, v->child.remove ( s + 1 ) ); //逐个移动效率低
      u->key.insert ( j, v->key.remove ( s + 1 ) ); //此策略可改进
   }
   u->child[_order - s - 1] = v->child.remove ( s + 1 ); //移动v最靠右的孩子
   if ( u->child[0] ) //若u的孩子们非空，则
      for ( Rank j = 0; j < _order - s; j++ ) //令它们的父节点统一
         u->child[j]->parent = u; //指向u
   BTNodePosi(T) p = v->parent; //v当前的父节点p
   if ( !p ) { _root = p = new BTNode<T>(); p->child[0] = v; v->parent = p; } //若p空则创建之
   Rank r = 1 + p->key.search ( v->key[0] ); //p中指向u的指针的秩
   p->key.insert ( r, v->key.remove ( s ) ); //轴点关键码上升
   p->child.insert ( r + 1, u );  u->parent = p; //新节点u与父节点p互联
   solveOverflow ( p ); //上升一层，如有必要则继续分裂——至多递归O(logn)层
}