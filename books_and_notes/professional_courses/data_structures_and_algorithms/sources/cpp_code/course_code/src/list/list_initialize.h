/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> void List<T>::init() { //列表初始化，在创建列表对象时统一调用
   header = new ListNode<T>; //创建头哨兵节点
   trailer = new ListNode<T>; //创建尾哨兵节点
   header->succ = trailer; header->pred = NULL;
   trailer->pred = header; trailer->succ = NULL;
   _size = 0; //记录规模
}