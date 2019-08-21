/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

/******************************************************************************************
 * BST节点删除算法：删除位置x所指的节点（全局静态模板函数，适用于AVL、Splay、RedBlack等各种BST）
 * 目标x在此前经查找定位，并确认非NULL，故必删除成功；与searchIn不同，调用之前不必将hot置空
 * 返回值指向实际被删除节点的接替者，hot指向实际被删除节点的父亲——二者均有可能是NULL
 ******************************************************************************************/
template <typename T>
static BinNodePosi(T) removeAt ( BinNodePosi(T) & x, BinNodePosi(T) & hot ) {
   BinNodePosi(T) w = x; //实际被摘除的节点，初值同x
   BinNodePosi(T) succ = NULL; //实际被删除节点的接替者
   if ( !HasLChild ( *x ) ) //若*x的左子树为空，则可
      succ = x = x->rc; //直接将*x替换为其右子树
   else if ( !HasRChild ( *x ) ) //若右子树为空，则可
      succ = x = x->lc; //对称地处理——注意：此时succ != NULL
   else { //若左右子树均存在，则选择x的直接后继作为实际被摘除节点，为此需要
      w = w->succ(); //（在右子树中）找到*x的直接后继*w
      swap ( x->data, w->data ); //交换*x和*w的数据元素
      BinNodePosi(T) u = w->parent;
      ( ( u == x ) ? u->rc : u->lc ) = succ = w->rc; //隔离节点*w
   }
   hot = w->parent; //记录实际被删除节点的父亲
   if ( succ ) succ->parent = hot; //并将被删除节点的接替者与hot相联
   release ( w->data ); release ( w ); return succ; //释放被摘除节点，返回接替者
}
