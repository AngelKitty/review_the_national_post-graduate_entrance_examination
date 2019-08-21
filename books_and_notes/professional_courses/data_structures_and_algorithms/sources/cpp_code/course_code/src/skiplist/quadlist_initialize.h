/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> void Quadlist<T>::init() { //Quadlist初始化，创建Quadlist对象时统一调用
   header = new QuadlistNode<T>; //创建头哨兵节点
   trailer = new QuadlistNode<T>; //创建尾哨兵节点
   header->succ = trailer; header->pred = NULL; //沿横向联接哨兵
   trailer->pred = header; trailer->succ = NULL; //沿横向联接哨兵
   header->above = trailer->above = NULL; //纵向的后继置空
   header->below = trailer->below = NULL; //纵向的前驱置空
   _size = 0; //记录规模
} //如此构造的四联表，不含任何实质的节点，且暂时与其它四联表相互独立