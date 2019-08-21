/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

/******************************************************************************************
 * RedBlack双红调整算法：解决节点x与其父均为红色的问题。分为两大类情况：
 *    RR-1：2次颜色翻转，2次黑高度更新，1~2次旋转，不再递归
 *    RR-2：3次颜色翻转，3次黑高度更新，0次旋转，需要递归
 ******************************************************************************************/
template <typename T> void RedBlack<T>::solveDoubleRed ( BinNodePosi(T) x ) { //x当前必为红
   if ( IsRoot ( *x ) ) //若已（递归）转至树根，则将其转黑，整树黑高度也随之递增
      {  _root->color = RB_BLACK; _root->height++; return;  } //否则，x的父亲p必存在
   BinNodePosi(T) p = x->parent; if ( IsBlack ( p ) ) return; //若p为黑，则可终止调整。否则
   BinNodePosi(T) g = p->parent; //既然p为红，则x的祖父必存在，且必为黑色
   BinNodePosi(T) u = uncle ( x ); //以下，视x叔父u的颜色分别处理
   if ( IsBlack ( u ) ) { //u为黑色（含NULL）时 //*DSA*/printf("  case RR-1:\n");
      if ( IsLChild ( *x ) == IsLChild ( *p ) ) //若x与p同侧（即zIg-zIg或zAg-zAg），则
         p->color = RB_BLACK; //p由红转黑，x保持红
      else //若x与p异侧（即zIg-zAg或zAg-zIg），则
         x->color = RB_BLACK; //x由红转黑，p保持红
      g->color = RB_RED; //g必定由黑转红
///// 以上虽保证总共两次染色，但因增加了判断而得不偿失
///// 在旋转后将根置黑、孩子置红，虽需三次染色但效率更高
      BinNodePosi(T) gg = g->parent; //曾祖父（great-grand parent）
      BinNodePosi(T) r = FromParentTo ( *g ) = rotateAt ( x ); //调整后的子树根节点
      r->parent = gg; //与原曾祖父联接
   } else { //若u为红色 //*DSA*/printf("  case RR-2:\n");
      p->color = RB_BLACK; p->height++; //p由红转黑
      u->color = RB_BLACK; u->height++; //u由红转黑
      if ( !IsRoot ( *g ) ) g->color = RB_RED; //g若非根，则转红
      solveDoubleRed ( g ); //继续调整g（类似于尾递归，可优化为迭代形式）
   }
}
