/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename K, typename V> bool Skiplist<K, V>::put ( K k, V v ) { //跳转表词条插入算法
   Entry<K, V> e = Entry<K, V> ( k, v ); //待插入的词条（将被随机地插入多个副本）
   if ( empty() ) insertAsFirst ( new Quadlist<Entry<K, V>> ); //插入首个Entry
   ListNode<Quadlist<Entry<K, V>>*>* qlist = first(); //从顶层四联表的
   QuadlistNode<Entry<K, V>>* p = qlist->data->first(); //首节点出发
   if ( skipSearch ( qlist, p, k ) ) //查找适当的插入位置（不大于关键码k的最后一个节点p）
      while ( p->below ) p = p->below; //若已有雷同词条，则需强制转到塔底
   qlist = last(); //以下，紧邻于p的右侧，一座新塔将自底而上逐层生长
   QuadlistNode<Entry<K, V>>* b = qlist->data->insertAfterAbove ( e, p ); //新节点b即新塔基座
   while ( rand() & 1 ) { //经投掷硬币，若确定新塔需要再长高一层，则
      while ( qlist->data->valid ( p ) && !p->above ) p = p->pred; //找出不低于此高度的最近前驱
      if ( !qlist->data->valid ( p ) ) { //若该前驱是header
         if ( qlist == first() ) //且当前已是最顶层，则意味着必须
            insertAsFirst ( new Quadlist<Entry<K, V>> ); //首先创建新的一层，然后
         p = qlist->pred->data->first()->pred; //将p转至上一层Skiplist的header
      } else //否则，可径自
         p = p->above; //将p提升至该高度
      qlist = qlist->pred; //上升一层，并在该层
      b = qlist->data->insertAfterAbove ( e, p, b ); //将新节点插入p之后、b之上
   }//课后：调整随机参数，观察总体层高的相应变化
   return true; //Dictionary允许重复元素，故插入必成功——这与Hashtable等Map略有差异
}